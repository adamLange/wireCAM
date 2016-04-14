#include "PocketCornerSlice.h"
#include "BRep_Tool.hxx"

#include "gp_Vec.hxx"
#include "gp_Pnt.hxx"
#include "gp_Ax3.hxx"

#include "Geom_Plane.hxx"
#include "Geom_Curve.hxx"

#include "Extrema_ExtCS.hxx"
#include "Extrema_POnCurv.hxx"
#include "Extrema_POnSurf.hxx"

#include "BRepAdaptor_CompCurve.hxx"
#include "IntTools_FClass2d.hxx"

#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepAdaptor_Surface.hxx"
#include "Extrema_ExtPS.hxx"
#include "TopAbs_State.hxx"

#include "Standard_Transient.hxx"
#include "GeomAPI_PointsToBSpline.hxx"
#include "TColgp_Array1OfPnt.hxx"

#include <exception>
#include <stdexcept>
#include <cmath>
#include <iostream> //debug
#define _USE_MATH_DEFINES


PocketCornerSlice::PocketCornerSlice(Pocket& pocket,
    const gp_Pnt& pivotPnt,const double& alpha0,
    const double& alpha1, bool performNow
  ):
  pocket(pocket),
  pivotPnt(pivotPnt),
  alpha0(alpha0),
  alpha1(alpha1)
{
  cc = new BRepAdaptor_HCompCurve(pocket.offsetWire);
  /*
  BRepBuilderAPI_MakeFace mf(pocket.offsetWire);
  if (not mf.IsDone())
  {
    throw std::runtime_error("PocketCornerSlice failure to make face");
  }
  TopoDS_Face face(mf.Face());
  //Check if normal to z axis
  IntTools_FClass2d pointClassifier(face,1e-6);
  gp_Pnt pnt;
  gp_Vec tang;
  cc->D1(cc->FirstParameter(),pnt,tang);
  gp_Ax1 ax(gp_Pnt(0,0,0),gp_Dir(0,0,1));
  gp_Pnt checkPnt((tang.Rotated(ax,M_PI/2.).Normalized()*pocket.toolR/5.
      +gp_Vec(pivotPnt.XYZ())).XYZ()
    );
  Extrema_ExtPS ext(checkPnt,BRepAdaptor_Surface(mf.Face()),1e-6,1e-6);
  double min_dsq = -1.0;
  int min_dsq_i;
  std::cout<<"nb_extrema "<<ext.NbExt()<<std::endl; //debug
  for (int i=1;i<=ext.NbExt();++i)
  {
    double dsq(ext.SquareDistance(i));
    if ((dsq < min_dsq)|(min_dsq < 0))
    {
      min_dsq = dsq;
      min_dsq_i = i;
    }
  }
  double u_check,v_check;
  ext.Point(min_dsq_i).Parameter(u_check,v_check);
  TopAbs_State pntState(
      pointClassifier.Perform(gp_Pnt2d(u_check,v_check))
    );
  switch (pntState)
  {
    case TopAbs_IN:
      wireNormal = gp_Dir(0,0,1);
      break;
    case TopAbs_OUT:
      wireNormal = gp_Dir(0,0,-1);
      break;
    case TopAbs_ON:
      throw std::runtime_error(
        "point landed on face edge when checking outer wire\
         orientation.");
      break;
  }*/
  //std::cout<<"direction "<<wireNormal.Z()<<std::endl;
  wireNormal = gp_Dir(0,0,-1);
  
  if (wireNormal.Z() > 0)
  {
    while (this->alpha1 < this->alpha0)
    {
      this->alpha1 += 2.0*M_PI;
    }
  }
  else
  {
    while (this->alpha1 > this->alpha0)
    {
      this->alpha1 -= 2.0*M_PI;
    }
  }

  params.clear();
  for (int i = 0; i<=100; ++i)
  {
    params.push_back(i*(this->alpha1-this->alpha0)/100.+this->alpha0);
  }
  calc(params,points,alphas);
}

void 
PocketCornerSlice::calc(const std::list<double>& params,
      std::list<gp_Pnt>& points,
      std::list<double>& alphas
    )
{
  points.clear();
  alphas.clear();
  BRep_Tool bt;
  //double p0,p1;
  //Handle(Geom_Curve) curve = bt.Curve(edge,p0,p1);
  //Handle(Geom_Surface) surface = bt.Surface(face);
  for (std::list<double>::const_iterator alpha = params.begin();
       alpha != params.end();
       ++alpha)
  {

    gp_Dir tang(std::cos(*alpha),std::sin(*alpha),0);
    gp_Vec vec_plane_x = -1.0*tang.Crossed(gp_Vec(wireNormal.XYZ()));
    gp_Dir dir_plane_x(vec_plane_x.XYZ());
    gp_Ax3 ax(pivotPnt,tang,dir_plane_x);
    Handle(Geom_Plane) pln = new Geom_Plane(ax);

    Extrema_ExtCS inter(cc->GetCurve(),GeomAdaptor_Surface(pln),
      1e-6,1e-6);
    // search for the intersection closest to the pivot point
    // with u_c > 0
    int i_min_dist;
    double min_dist = -1;
    double w_int;
    gp_Pnt intPnt;
    for (int i=1;i<=inter.NbExt();++i)
    {
      Extrema_POnCurv poc;
      Extrema_POnSurf pos;
      inter.Points(i,poc,pos);
      double u,v;
      pos.Parameter(u,v);
      if (((u<min_dist)&(u>0))|(min_dist < 0))
      {
        min_dist = u;
        intPnt = pos.Value();
      }
    }
    gp_Vec intVec(intPnt.XYZ());
    std::cout<<intPnt.X()<<" "<<intPnt.Y()<<" "<<intPnt.Z()<<std::endl;
    gp_Vec pivotVec(pivotPnt.XYZ());
    gp_Vec rVec(
        pocket.toolR*std::cos(*alpha-M_PI/2.),
        pocket.toolR*std::sin(*alpha-M_PI/2.),
        0
      );
    gp_Vec toolPosVec(intVec-rVec);
    points.push_back(toolPosVec.XYZ());
    alphas.push_back(*alpha);
  }
}

TopoDS_Shape
PocketCornerSlice::shape()
{
  int low = 1;
  int high = points.size();
  TColgp_Array1OfPnt pnts(low,high);
  int i = 1;
  for (std::list<gp_Pnt>::iterator it = points.begin();
      it!=points.end();++it)
  {
    pnts.SetValue(i,*it);
    ++i;
  }
  GeomAPI_PointsToBSpline p2b(pnts);
  Handle(Geom_Curve) curve = p2b.Curve();
  BRepBuilderAPI_MakeEdge me(curve,curve->FirstParameter(),curve->LastParameter());
  std::cout<<"HIHEOIlk;;ldafs"<<std::endl;
  return me.Edge();
  
}

