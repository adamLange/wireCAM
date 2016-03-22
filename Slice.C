#include "Slice.h"
#include <list>
#include "gp_Pnt.hxx"
#include "BRepMesh_IncrementalMesh.hxx"
#include "TopLoc_Location.hxx"
#include "BRep_Tool.hxx"
#include <exception>
#include <stdexcept>
#include "TColgp_Array1OfPnt.hxx"
#include "TColStd_Array1OfReal.hxx"
#include "Poly_Polygon3D.hxx"

using namespace std;

Slice::Slice(const TopoDS_Edge& edge, const TopoDS_Face& face,
    const bool performNow, const double baseTol
  ):
  edge(edge),
  face(face),
  baseTol(baseTol)
{
  if (performNow)
  {
    list<double> params;
    list<gp_Pnt> points;
    performBaseTessilation(params,points);
  }
}

void Slice::performBaseTessilation(list<double>& params,
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
  for (int i = 1; i <= nodesArr.Upper(); i++ )
  {
    points.push_back(nodesArr.Value(i));
    params.push_back(parametersArr.Value(i));
  }
}

void Slice::calc(const list<double>& params,
      list<gp_Pnt>& points,
      list<gp_Vec>& normals,
      list<double>& alphas
    )
{
  cout<<"Pretending to calculate"<<endl;
}
