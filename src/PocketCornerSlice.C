#include "PocketCornerSlice.h"
#include "BRep_Tool.hxx"
#include "GeomAPI_IntCS.hxx"
#include <math>

PocketCornerSlice::PocketCornerSlice()
{
}

void 
PocketCornerSlice::calc(const list<double>& params,
      list<gp_Pnt>& points,
      list<gp_Vec>& normals,
      list<double>& alphas
    )
{
  points.clear();
  normals.clear();
  alphas.clear();
  BRep_Tool bt;
  //double p0,p1;
  //Handle(Geom_Curve) curve = bt.Curve(edge,p0,p1);
  //Handle(Geom_Surface) surface = bt.Surface(face);
  for (list<double>::const_iterator u = params.begin();
       u != params.end();
       ++u)
  {

    gp_Dir tang(std::cos(*u),std::sin(*u),0);
    gp_Vec v_z(0,0,1);
    gp_Vec vec_plane_x = tang.Crossed(v_z);
    gp_Dir dir_plane_x(vec_plane_x.XYZ());
    gp_Ax3 ax(pnt,tang,dir_plane_x);
    Handle(Geom_Plane) new pln(ax);
    TopExp_Explorer exp;
    for (exp.Init(outerWire,TopABS_EDGE);exp.More();exp.Next())
    {
      BRepExtrema_ExtCF extcf(exp.current(),tool.face);
      int i_min_dist;
      double min_dist = -1;
      for (int i=1;i<extcf.NbExt();++i)
      {
        dist = extcf.SquareDistance(i);
        if ( (dist < min_dist) | (min_dist<0.))
        {
          i_min_dist = i;
          min_dist = dist;
        }
      }
    }

    GeomAPI_IntCS intersection(curve,pln);

  }
}
