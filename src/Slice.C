#include "Slice.h"
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
#include "Marker.h"

#include <iostream>

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
    //list<double> params;
    //list<gp_Pnt> points;
    performBaseTessilation(params,points);
    calc(params,points,normals,alphas);
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
  for (int i = 1; i <= nodesArr.Upper(); ++i )
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

list<Slice>
Slice::split(Splitter& splitter)
{
  list<Slice> newSlices;
  Slice activeSlice(edge,face,false);
  list<double>::iterator it_params = params.begin();
  list<gp_Pnt>::iterator it_points = points.begin();
  list<gp_Vec>::iterator it_normals = normals.begin();
  list<double>::iterator it_alphas = alphas.begin();
  
  bool valid = splitter.evaluate(*this,*it_params,*it_points,
      *it_normals,*it_alphas);
  bool validOld;
  if (valid)
  {
    activeSlice.params.push_back(*it_params);
    activeSlice.points.push_back(*it_points);
    activeSlice.normals.push_back(*it_normals);
    activeSlice.alphas.push_back(*it_alphas);
  }
  ++it_params;
  ++it_points;
  ++it_normals;
  ++it_alphas;
  for (;it_params != params.end();
       ++it_params,
       ++it_points,
       ++it_normals,
       ++it_alphas
      )
  {
    validOld = valid;
    valid = splitter.evaluate(*this,*it_params,*it_points,
      *it_normals,*it_alphas);
    if (validOld == valid) {
      if (valid)
      {
        activeSlice.params.push_back(*it_params);
        activeSlice.points.push_back(*it_points);
        activeSlice.normals.push_back(*it_normals);
        activeSlice.alphas.push_back(*it_alphas);
      }
      continue;
    }
    else
    {
      if(!validOld) //beginning of a slice
      {
        activeSlice.params.push_back(*it_params);
        activeSlice.points.push_back(*it_points);
        activeSlice.normals.push_back(*it_normals);
        activeSlice.alphas.push_back(*it_alphas);
      }
      else //end of a slice
      {
        newSlices.push_back(activeSlice);
        activeSlice = Slice(edge,face,false);
      }
    }
  }
  if (activeSlice.params.size() > 0)
  {
    newSlices.push_back(activeSlice);
  }
  return newSlices;
}

void
Slice::refine(Marker& marker)
{
  list<double>::iterator params_tail = params.begin();
  list<double>::iterator params_head = params.begin();
  ++params_head;
  list<gp_Pnt>::iterator points_tail = points.begin();
  list<gp_Pnt>::iterator points_head = points.begin();
  ++points_head;
  list<gp_Vec>::iterator normals_tail = normals.begin();
  list<gp_Vec>::iterator normals_head = normals.begin();
  ++normals_head;
  list<double>::iterator alphas_tail = alphas.begin();
  list<double>::iterator alphas_head = alphas.begin();
  ++alphas_head;
  
  bool converged = false;
  while(params_head != params.end())
  {
    while(!converged)
    {
      //cout<<"  parameters "<<*params_tail<<" -> "<<*params_head<<endl;
      converged = !marker.evaluate(
        *this,
        *params_head,*points_head,*normals_head,*alphas_head,
        *params_tail,*points_tail,*normals_tail,*alphas_tail
      );
      if (!converged)
      {
        //add a point in the middle
        //move the head back to it
        list<double> newParams;
        newParams.push_back((*params_head+*params_tail)/2.);
        list<gp_Pnt> newPoints;
        list<gp_Vec> newNormals;
        list<double> newAlphas;
        calc(newParams,newPoints,newNormals,newAlphas);
        params.splice(params_head,newParams);
        points.splice(points_head,newPoints);
        normals.splice(normals_head,newNormals);
        alphas.splice(alphas_head,newAlphas);
        --params_head;
        --points_head;
        --normals_head;
        --alphas_head;
      }
    }
    
    ++params_head;
    ++points_head;
    ++normals_head;
    ++alphas_head;
    ++params_tail;
    ++points_tail;
    ++normals_tail;
    ++alphas_tail;
    converged = false;
  }
}
