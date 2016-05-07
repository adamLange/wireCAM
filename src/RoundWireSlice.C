#include "RoundWireSlice.h"
#include "gp_Ax1.hxx"
#include "BRep_Tool.hxx"
#include "Geom_Curve.hxx"
using namespace std;

RoundWireSlice::RoundWireSlice(const TopoDS_Edge& edge,
   const TopoDS_Face& face, const bool flipSurface,
   const bool performNow, const double baseTol
  ):
  Slice3D(edge,face,performNow,baseTol),
  flipSurface(flipSurface)
{
  if (performNow)
  {
    performBaseTessilation(params,points);
    calc(params,points,alphas);
  }
}

Slice*
RoundWireSlice::emptyCopy()
{
  return new RoundWireSlice(this->edge,this->face,false,this->baseTol);
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
    gp_Dir norm(surfaceNormal(*it));
    if (flipSurface)
    {
      norm = -norm;
    }
    gp_Vec s(norm.XYZ());
    double alpha = atan2(travelVec.Y(),travelVec.X());
    gp_Ax1 ax(gp_Pnt(0,0,0),gp_Dir(0,0,1));
    gp_Trsf trsf;
    trsf.SetRotation(ax,alpha);
    gp_Vec xPrime(1,0,0);
    xPrime.Transform(trsf);
    gp_Vec zPrime(0,0,1);
    
    gp_Vec sPrime = gp_Vec(s.Dot(xPrime),s.Dot(zPrime),0);
    sPrime.Transform(trsf);
    sPrime *= toolRadius;

    gp_Vec pntVec(pnt.XYZ());
    pntVec += sPrime;
    points.emplace_back(pntVec.XYZ());
    alphas.push_back(alpha);
  }
}
