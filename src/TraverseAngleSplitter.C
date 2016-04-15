#include "TraverseAngleSplitter.h"
#include <cmath>
#include "BRep_Tool.hxx"
#include "Slice.h"
#include "Slice3D.h"
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
TraverseAngleSplitter::evaluate(Slice& slice,
    const double& param, const gp_Pnt& point,
    const double& alpha
  )
{
  if (typeid(slice) == typeid(Slice3D))
  {
    Slice3D* slicePtr = dynamic_cast<Slice3D*>(&slice);
    BRep_Tool bt;
    double u_min,u_max;
    Handle(Geom_Curve) curve = bt.Curve(slicePtr->edge,u_min,u_max);
    gp_Vec v = -curve->DN(param,1);
    double beta = atan2(v.Y(),v.X());
    if (fmod(abs(alpha-beta),2*M_PI) < maxTraverseAngle)
    {return true;}
    else
    {return false;}
  }
  else
  {
    return true;
  }
}
