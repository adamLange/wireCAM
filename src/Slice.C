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
    activeSlice->alphaPushBack(*it_alphas);
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
        activeSlice->alphaPushBack(*it_alphas);
      }
      continue;
    }
    else
    {
      if(!validOld) //beginning of a slice
      {
        activeSlice->params.push_back(*it_params);
        activeSlice->points.push_back(*it_points);
        activeSlice->alphaPushBack(*it_alphas);
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
  int iterations(0);
  bool failed = false;
  while((params_head != params.end()) && (!failed))
  {
    iterations = 0;
    while(!converged&&(!failed))
    {
      //cout<<"  parameters "<<*params_tail<<" -> "<<*params_head<<endl;
      //std::cout<<"iterations: "<<iterations<<std::endl;
      converged = !marker.evaluate(
        *this,
        *params_head,*points_head,*alphas_head,
        *params_tail,*points_tail,*alphas_tail
      );
      if (iterations > 100)
      {
        std::cout<<"Warning! 100 iterations exceded with no\
 convergence, refinement of this slice terminated."
          <<std::endl;
        failed = true;
      }
      if ((*params_head - *params_tail)<1e-6)
      {
        failed = true;
        std::cout<<"Warning! refinement terminated on delta\
 parameter less than 1e-6"<<std::endl;
      }
      if ((!converged) && (!failed))
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
        alphaSplice(alphas_head,newAlphas);
        --params_head;
        --points_head;
        --alphas_head;
      }
      ++iterations;
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
Slice::calc(const list<double>& params, list<gp_Pnt>& points,
  list<double>& alphas)
{
  points.clear();
  alphas.clear();
  for (list<double>::const_iterator it = params.begin();
       it != params.end();
       ++it)
  {
    points.push_back(gp_Pnt(0,0,0));
    alphaPushBack(0.);
  }
}

TopoDS_Shape
Slice::shape()
{
  BRepBuilderAPI_MakeEdge me(gp_Pnt(0,0,0),gp_Pnt(1,0,0));
  return me.Edge();
}

gp_Dir
Slice::surfaceNormal(const double& u)
{
  return gp_Dir(0,0,1);
}

double
Slice::alphaModulator(const double& alpha0, const double& alpha1)
{
  double remainder = fmod((alpha1-alpha0),2.0*M_PI);
  double delta;

  if (abs(remainder) < M_PI)
  {
    delta = remainder;
  }
  else if (remainder > M_PI)
  {
    delta = remainder - 2*M_PI;
  }
  else
  {
    delta = remainder + 2*M_PI;
  }
  return alpha0 + delta;
}

void
Slice::alphaSplice(list<double>::iterator& postSplice,
  list<double> alphaList)
{
  list<double>::iterator preSplice = postSplice;
  --preSplice;
  list<double>::iterator head = alphaList.begin();
  list<double>::iterator tail = alphaList.begin();
  *head = alphaModulator(*preSplice,*head);
  ++head;
  for (; head != alphaList.end(); ++head, ++tail)
  {
    *head = alphaModulator(*tail,*head);
  }
  alphas.splice(postSplice,alphaList);
}

void
Slice::alphaPushBack(const double& alpha)
{
  alphas.push_back(alphaModulator(alphas.back(),alpha));
}

void
Slice::orderAlphas()
{
  list<double>::iterator head = alphas.begin();
  list<double>::iterator tail = alphas.begin();
  ++head;
  for (; head != alphas.end(); ++head, ++tail)
  {
    *head = alphaModulator(*tail,*head);
  }
}
