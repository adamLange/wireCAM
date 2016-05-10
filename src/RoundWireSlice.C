#include "RoundWireSlice.h"
#include "gp_Ax1.hxx"
#include "gp_Ax2.hxx"
#include "gp_Dir.hxx"
#include "gp_Trsf.hxx"
#include "gp_Circ.hxx"
#include "BRep_Tool.hxx"
#include "Geom_Curve.hxx"
#include "TopoDS_Builder.hxx"
#include "TopoDS_Compound.hxx"
#include "gp_Ax2.hxx"
#include "Geom_Circle.hxx"
#include "Handle_Geom_Circle.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "TopoDS_Edge.hxx"

#define _USE_MATH_DEFINES
using namespace std;

RoundWireSlice::RoundWireSlice(const TopoDS_Edge& edge,
   const TopoDS_Face& face, 
   const double toolRadius,
   const bool flipSurface,
   const bool performNow, const double baseTol
  ):
  Slice3D(edge,face,performNow,baseTol),
  flipSurface(flipSurface),
  toolRadius(toolRadius)
{
  if (performNow)
  {
    performBaseTessilation(params,points);
    calc(params,points,alphas);
    orderAlphas();
  }
}

Slice*
RoundWireSlice::emptyCopy()
{
  return new RoundWireSlice(this->edge,this->face,this->toolRadius,flipSurface,false,this->baseTol);
}

void
RoundWireSlice::calc(
    const std::list<double>& params,
    std::list<gp_Pnt>& points,
    std::list<double>& alphas)
{
  points.clear();
  alphas.clear();
  
  BRep_Tool bt;
  double p0,p1;
  Handle(Geom_Curve) curve = bt.Curve(edge,p0,p1);
  
  for (list<double>::const_iterator it = params.begin();
       it != params.end();
       ++it)
  {
    gp_Pnt pnt;
    gp_Vec travelVec;
    curve->D1(*it,pnt,travelVec);
    travelVec.Normalize();
    gp_Vec pntVec(pnt.XYZ());
    gp_Dir norm(surfaceNormal(*it));
    if (flipSurface)
    {
      norm = -norm;
    }
    gp_Vec s(norm.XYZ());

    gp_Vec uPrime = (travelVec.Crossed(gp_Vec(0,0,-1))).Normalized();
    gp_Vec vPrime(0,0,1);

    gp_Vec sPrime = s.Dot(uPrime)*uPrime + s.Dot(vPrime)*vPrime;

    double alpha = atan2(travelVec.Y(),travelVec.X());
    //gp_Ax1 ax(gp_Pnt(0,0,0),gp_Dir(0,0,1));
    //gp_Trsf trsf;
    //trsf.SetRotation(ax,alpha);
    //gp_Vec yPrime(0,1,0);
    //yPrime.Transform(trsf);
    //gp_Vec zPrime(0,0,1);
    
    //gp_Vec sPrime = gp_Vec(s.Dot(yPrime),s.Dot(zPrime),0);
    //sPrime.Transform(trsf);
    sPrime *= toolRadius;

    points.emplace_back((pntVec + sPrime).XYZ());

    //gp_Vec centerVec((s*toolRadius+pntVec).XYZ());
    //gp_Vec surfToCenter = centerVec - pntVec;
    //std::cout<<surfToCenter.Magnitude()<<std::endl;
    //points.emplace_back(centerVec.XYZ());
    alphaPushBack(alpha);
    
  }
}

TopoDS_Shape
RoundWireSlice::shape()
{
  TopoDS_Compound comp;
  TopoDS_Builder builder;
  builder.MakeCompound(comp);

  list<double>::iterator it_params = params.begin();
  list<gp_Pnt>::iterator it_points = points.begin();
  list<double>::iterator it_alphas = alphas.begin();

  for (;it_params != params.end();
         ++it_params,
         ++it_points,
         ++it_alphas
      )
  {
    gp_Ax1 zAxis(gp_Pnt(0,0,0),gp_Dir(0,0,1));
    gp_Trsf trsf;
    trsf.SetRotation(zAxis,*it_alphas);
    gp_Dir travelDir(1,0,0);
    gp_Dir planeVDir(0,1,0);
    travelDir.Transform(trsf);
    planeVDir.Transform(trsf);
    
    gp_Ax2 ax(*it_points,travelDir,planeVDir);
    //gp_Ax2 ax(*it_points,gp_Dir(1,0,0),gp_Dir(0,1,0));
    gp_Circ circ(ax,toolRadius);

    //Handle(Geom_Circle) circ = new Geom_Circle(ax,toolRadius);
    BRepBuilderAPI_MakeEdge me(circ,0,2*M_PI);
    TopoDS_Edge edge = me.Edge();
    builder.Add(comp,edge);
    //BRepBuilderAPI_MakeVertex mv(*it_points);
    //builder.Add(comp,mv.Vertex());
  }
  builder.Add(comp,this->edge);
  return comp;
}
