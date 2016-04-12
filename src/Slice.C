#include "Slice.h"
#include "Splitter.h"
#include <list>
#include <cmath>
#include "gp_Pnt.hxx"
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
#include <utility>

using namespace std;

Slice::Slice()
{
}

Slice*
Slice::emptyCopy()
{
  return new Slice;
}

list<unique_ptr<Slice>>
Slice::split(Splitter& splitter)
{
  list<unique_ptr<Slice>> newSlices;
  //unique_ptr<Slice> activeSlice(emptyCopy());
  unique_ptr<Slice> activeSlice;
  activeSlice.reset(emptyCopy());
  list<double>::iterator it_params = params.begin();
  list<gp_Pnt>::iterator it_points = points.begin();
  list<double>::iterator it_alphas = alphas.begin();
  
  bool valid = splitter.evaluate(*this,*it_params,*it_points,
      *it_alphas);
  bool validOld;
  if (valid)
  {
    activeSlice->params.push_back(*it_params);
    activeSlice->points.push_back(*it_points);
    activeSlice->alphas.push_back(*it_alphas);
  }
  ++it_params;
  ++it_points;
  ++it_alphas;
  for (;it_params != params.end();
       ++it_params,
       ++it_points,
       ++it_alphas
      )
  {
    validOld = valid;
    valid = splitter.evaluate(*this,*it_params,*it_points,
      *it_alphas);
    if (validOld == valid) {
      if (valid)
      {
        activeSlice->params.push_back(*it_params);
        activeSlice->points.push_back(*it_points);
        activeSlice->alphas.push_back(*it_alphas);
      }
      continue;
    }
    else
    {
      if(!validOld) //beginning of a slice
      {
        activeSlice->params.push_back(*it_params);
        activeSlice->points.push_back(*it_points);
        activeSlice->alphas.push_back(*it_alphas);
      }
      else //end of a slice
      {
        if (activeSlice->params.size() >1)
        {
          activeSlice->updateGeometry();
          newSlices.push_back(move(activeSlice));
        }
        activeSlice.reset(emptyCopy());
      }
    }
  }
  if (activeSlice->params.size() > 1)
  {
    activeSlice->updateGeometry();
    newSlices.push_back(move(activeSlice));
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
        *params_head,*points_head,*alphas_head,
        *params_tail,*points_tail,*alphas_tail
      );
      if (!converged)
      {
        //add a point in the middle
        //move the head back to it
        list<double> newParams;
        newParams.push_back((*params_head+*params_tail)/2.);
        list<gp_Pnt> newPoints;
        list<double> newAlphas;
        calc(newParams,newPoints,newAlphas);
        params.splice(params_head,newParams);
        points.splice(points_head,newPoints);
        alphas.splice(alphas_head,newAlphas);
        --params_head;
        --points_head;
        --alphas_head;
      }
    }
    
    ++params_head;
    ++points_head;
    ++alphas_head;
    ++params_tail;
    ++points_tail;
    ++alphas_tail;
    converged = false;
  }
}

void
Slice::updateGeometry()
{
}

void
Slice::calc(const list<double> params, list<gp_Pnt> points,
  list<double> alphas)
{
  points.clear();
  alphas.clear();
  for (list<double>::const_iterator it = params.begin();
       it != params.end();
       ++it)
  {
    points.push_back(gp_Pnt(0,0,0));
    alphas.push_back(0.);
  }
}
