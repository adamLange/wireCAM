#include "TopoDS_Shape.hxx"
#include "TargetSurface.h"
#include "TopoDS_Edge.hxx"
#include "gp_Pnt.hxx"
#include <iostream>
#include "TopoDS.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "gp_Pln.hxx"
#include "BRepAlgo_Section.hxx"
#include "TopExp_Explorer.hxx"
#include "Handle_TopoDS_TEdge.hxx"
#include "BRepMesh_IncrementalMesh.hxx"

using namespace std;

TargetSurface::TargetSurface() :
  face(TopoDS_Face())
{
}

TargetSurface::TargetSurface(TopoDS_Face &faceIn) :
  face(faceIn)
{
}

list<TopoDS_Edge> TargetSurface::intersectWithPlane(const gp_Pnt &pnt, const gp_Dir &dir)
{
  gp_Pln pln(pnt,dir);
  BRepAlgo_Section ms(face,pln);
  TopoDS_Shape S = ms.Shape();
  list<TopoDS_Edge> edges;
  TopExp_Explorer Ex;
  for (Ex.Init(S,TopAbs_EDGE); Ex.More(); Ex.Next()) {
    TopoDS_Edge edge = TopoDS::Edge(Ex.Current());
    edges.push_back(edge);
  } 

  return edges;
}

list<TopoDS_Edge> TargetSurface::intersectWithPlanes(
      const gp_Pnt& pnt,
      const gp_Dir& dir,
      const gp_Vec& vec,
      int n
    )
{
  gp_Vec v_base(pnt.XYZ());
  list<TopoDS_Edge> edges;
  for (int i = 0; i<n; i++) {
    gp_Pnt pnt_i((v_base + vec.Multiplied(i)).XYZ());
    list<TopoDS_Edge> newEdges;
    newEdges = intersectWithPlane(pnt_i,dir);
    edges.splice(edges.end(),newEdges);
  }
  return edges;
}


list<TopoDS_Edge> TargetSurface::slice(
    const gp_Dir& dir,
    const float& spacing,
    const float& boundingBoxPadding,
    const float& tol,
    const bool& performIncMesh
  )
{
  if(performIncMesh)
  {
    BRepMesh_IncrementalMesh(face,tol);
  }
  gp_Pnt pnt(0,0,0);
  gp_Vec vdelta(dir.XYZ());
  int n = 10;
  return intersectWithPlanes(pnt,dir,vdelta,n);
}
