#include "EdgeSlice.h"
#include "BRepMesh_IncrementalMesh.hxx"
#include "BRep_Tool.hxx"
#include <stdexcept>
#include "TColgp_Array1OfPnt.hxx"
#include "Poly_Polygon3D.hxx"
#include "TColStd_Array1OfReal.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "Standard_Transient.hxx"
#include "Geom_Curve.hxx"
#include <cmath>


EdgeSlice::EdgeSlice(TopoDS_Edge edge, bool performNow,
    double baseTol, double alphaVertical
  ):
    edge(edge),
    baseTol(baseTol),
    alphaVertical(alphaVertical)
{
  
  if (performNow)
  {
    performBaseTessilation(params,points);
    calc(params,points,alphas);
  }
}

void 
EdgeSlice::performBaseTessilation(std::list<double>& params,
   std::list<gp_Pnt>& points)
{
  BRepMesh_IncrementalMesh meshOp(edge,baseTol);
  TopLoc_Location tl;
  BRep_Tool bt;
  Handle(Poly_Polygon3D) ply = bt.Polygon3D(edge,tl);

  if (ply.IsNull())
  {
    throw std::runtime_error("empty polygon data!");
  }

  const TColgp_Array1OfPnt& nodesArr = ply->Nodes();
  const TColStd_Array1OfReal& parametersArr = ply->Parameters();

  params.clear();
  points.clear();
  for (int i = 1; i <= nodesArr.Upper(); ++i )
  {
    points.push_back(nodesArr.Value(i));
    params.push_back(parametersArr.Value(i));
  }
}

TopoDS_Shape
EdgeSlice::shape()
{
  return edge;
}

Slice*
EdgeSlice::emptyCopy()
{
  return new EdgeSlice(edge,false,baseTol,alphaVertical);
}

void
EdgeSlice::updateGeometry()
{
  BRep_Tool bt;
  double p0,p1;
  Handle(Geom_Curve) curve = bt.Curve(edge,p0,p1);
  BRepBuilderAPI_MakeEdge me(curve,params.front(),params.back());
  edge = me.Edge();
  return;
}


void
EdgeSlice::calc(std::list<double>& params,
    std::list<gp_Pnt>& points, std::list<double>& alphas)
{
  points.clear();
  alphas.clear();

  BRep_Tool bt;
  double p0,p1;
  Handle(Geom_Curve) curve = bt.Curve(edge,p0,p1);
  for (std::list<double>::iterator param = params.begin();
        param != params.end(); ++param
      )
  {
    gp_Pnt pnt;
    gp_Vec tang;
    curve->D1(*param,pnt,tang);
    tang.Normalize();
    points.push_back(pnt);
    if ( ( std::abs(tang.Y()) < 1e-6 ) && (std::abs(tang.X()) < 1e-6) )
    {
      alphaPushBack(alphaVertical);
    }
    else
    {
      alphaPushBack(std::atan2(tang.Y(),tang.X()));
    }
  }
}
