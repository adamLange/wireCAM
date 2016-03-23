#include "TraverseAngleSplitter.h"
#include <cmath>
#include "BRep_Tool.hxx"
#include "Slice.h"
#include "Geom_Curve.hxx"
#define _USE_MATH_DEFINES
using namespace std;

TraverseAngleSplitter::TraverseAngleSplitter():
  maxTraverseAngle(M_PI/4.)
{
}

void
TraverseAngleSplitter::setMaxTraverseAngle(const double& angle)
{
  maxTraverseAngle = angle;
}

bool
TraverseAngleSplitter::evaluate(const Slice& slice,
    const double& param, const gp_Pnt& point,
    const gp_Vec& normal, const double& alpha
  )
{
  BRep_Tool bt;
  double u_min,u_max;
  Handle(Geom_Curve) curve = bt.Curve(slice.edge,u_min,u_max);
  gp_Vec v = -curve->DN(param,1);
  double beta = atan2(v.Y(),v.X());
  if (fmod(abs(alpha-beta),M_PI) < maxTraverseAngle)
  {return true;}
  else
  {return false;}
}
