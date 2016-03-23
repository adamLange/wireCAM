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
#include "Bnd_Box.hxx"
#include "BRepBndLib.hxx"
#include "Standard_Real.hxx"
#include <cmath>
#include "STEPControl_Reader.hxx"
#include "IFSelect_ReturnStatus.hxx"
#include "Standard_CString.hxx"
#include <exception>
#include <stdexcept>

using namespace std;

TargetSurface::TargetSurface() :
  face(TopoDS_Face())
{
}

TargetSurface::TargetSurface(TopoDS_Face &faceIn) :
  face(faceIn)
{
}

TargetSurface::TargetSurface(const Standard_CString& filepath)
{
  STEPControl_Reader reader;
  IFSelect_ReturnStatus status;
  status = reader.ReadFile(filepath);
  TopoDS_Shape shape;
  if (status == IFSelect_RetDone)
  {
    bool failsonly = false;
    reader.PrintCheckLoad(failsonly, IFSelect_ItemsByEntity);
    reader.PrintCheckTransfer(failsonly, IFSelect_ItemsByEntity);
    bool ok = reader.TransferRoot(1);
    int nbs = reader.NbShapes();
    shape = reader.Shape(1);
  }
  else
  {
    throw runtime_error("Failed to read file");
    //cout<<"Failed to read file"<<endl;
  }
  TopExp_Explorer exp;
  list<TopoDS_Face> faces;
  for (exp.Init(shape,TopAbs_FACE); exp.More(); exp.Next()) {
    TopoDS_Face face = TopoDS::Face(exp.Current());
    faces.push_back(face);
  }
  face = faces.front();
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
  for (int i = 0; i<n; ++i){
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

  Bnd_Box boundingBox;
  boundingBox.SetGap(0.0);
  BRepBndLib::Add(face,boundingBox);
  Standard_Real x0,y0,z0,x1,y1,z1;
  boundingBox.Get(x0,y0,z0,x1,y1,z1);

  x0 -= boundingBoxPadding;
  x1 += boundingBoxPadding;
  y0 -= boundingBoxPadding;
  y1 += boundingBoxPadding;
  z0 -= boundingBoxPadding;
  z1 += boundingBoxPadding;

  gp_Pnt pnt(0,0,0);

  if (dir.X() <= 0)
  {pnt.SetX(x1);}
  else
  {pnt.SetX(x0);}

  if (dir.Y() <=0)
  {pnt.SetY(y1);}
  else
  {pnt.SetY(y0);}

  if (dir.Z() <= 0)
  {pnt.SetZ(z1);}
  else
  {pnt.SetZ(z0);}

  gp_Vec vx(x1-x0,0,0);
  gp_Vec vy(0,y1-y0,0);
  gp_Vec vz(0,0,z1-z0);

  gp_Vec nx(1,0,0);
  gp_Vec ny(0,1,0);
  gp_Vec nz(0,0,1);

  gp_Vec dirVec(dir.XYZ());

  double dotx = abs(vx.Dot(dirVec));
  double doty = abs(vy.Dot(dirVec));
  double dotz = abs(vz.Dot(dirVec));

  gp_Vec vdelta = dirVec.Multiplied(spacing);

  int n;
  double dot;
  if ((dotx >= doty)&(dotx >= dotz)) 
  {
    dot = nx.Dot(vdelta);
    n = abs(int(ceil((x1-x0)/dot)));
  }
  else if ((doty >= dotx)&(doty >= dotz))
  {
    dot = ny.Dot(vdelta);
    n = abs(int(ceil((y1-y0)/dot)));
  }
  else
  {
    dot = nz.Dot(vdelta);
    n = abs(int(ceil((z1-z0)/dot)));
  }

  return intersectWithPlanes(pnt,dir,vdelta,n);
}
