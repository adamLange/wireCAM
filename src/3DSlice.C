#include "3DSlice.h"
#include "Splitter.h"
#include <list>
#include <cmath>
#include "gp_Pnt.hxx"
#include "BRepMesh_IncrementalMesh.hxx"
#include "TopLoc_Location.hxx"
#include "BRep_Tool.hxx"
#include <exception>
#include <stdexcept>
#include "TColgp_Array1OfPnt.hxx"
#include "TColStd_Array1OfReal.hxx"
#include "Poly_Polygon3D.hxx"
#include "Geom_Curve.hxx"
#include "TopoDS_Vertex.hxx"
#include "BRepExtrema_ExtPF.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "Marker.h"

#include <iostream>

using namespace std;

3DSlice::3DSlice(const TopoDS_Edge& edge, const TopoDS_Face& face,
    const bool performNow, const double baseTol
  ):
  edge(edge),
  face(face),
  baseTol(baseTol)
{
  if (performNow)
  {
    //list<double> params;
    //list<gp_Pnt> points;
    performBaseTessilation(params,points);
    calc(params,points,normals,alphas);
  }
}

3DSlice::emptyCopy()
{
}

void 3DSlice::performBaseTessilation(list<double>& params,
   list<gp_Pnt>& points)
{
  BRepMesh_IncrementalMesh meshOp(edge,baseTol);
  TopLoc_Location tl;
  BRep_Tool bt;
  Handle(Poly_Polygon3D) ply = bt.Polygon3D(edge,tl);

  if (ply.IsNull())
  {
    throw runtime_error("empty polygon data!");
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

void 3DSlice::calc(const list<double>& params,
      list<gp_Pnt>& points,
      list<gp_Vec>& normals,
      list<double>& alphas
    )
{
  points.clear();
  normals.clear();
  alphas.clear();
  BRep_Tool bt;
  double p0,p1;
  Handle(Geom_Curve) curve = bt.Curve(edge,p0,p1);
  Handle(Geom_Surface) surface = bt.Surface(face);
  for (list<double>::const_iterator it = params.begin();
       it != params.end();
       ++it)
  {
    gp_Pnt pnt = curve->Value(*it);
    BRepBuilderAPI_MakeVertex mv(pnt);
    TopoDS_Vertex v = mv.Vertex();
    BRepExtrema_ExtPF extr(v,face);
    int minExtIndex = -1;
    double minDSq = -1234.0;
    for (int i = 1; i <= extr.NbExt(); ++i)
    {
      double di = extr.SquareDistance(i);
      if ((di<minDSq)|(minDSq < 0))
      {
        minExtIndex = i;
        minDSq = di;
      }
    }
    double u_s,v_s,alpha;
    extr.Parameter(minExtIndex,u_s,v_s);
    gp_Vec du, dv, norm;
    gp_Pnt pnt_s;

    surface->D1(u_s,v_s,pnt_s,du,dv);
    norm = dv.Crossed(du).Normalized();
    alpha = atan2(norm.Y(),norm.X());

    points.push_back(pnt_s);
    normals.push_back(norm);
    alphas.push_back(alpha);
  }
}
