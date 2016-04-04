#include "WorkingBoxSplitter.h"

WorkingBoxSplitter::WorkingBoxSplitter():
  infinite(true)
{
}

void
WorkingBoxSplitter::setWorkingBox(
    const double& x0, const double& y0, const double& z0,
    const double& x1, const double& y1, const double& z1
  )
{
  infinite = false;
  minX = x0; minY = y0; minZ = z0;
  maxX = x1; maxY = y1; maxZ = z1;
}

bool
WorkingBoxSplitter::evaluate(const Slice& slice,
    const double& param, const gp_Pnt& point,
    const gp_Vec& normal, const double& alpha
  )
{
  bool rval;
  if (infinite) {rval = true;}
  else {
    rval = ( point.X() >= minX ) &&
           ( point.X() <= maxX ) &&
           ( point.Y() >= minY ) &&
           ( point.Y() <= maxY ) &&
           ( point.Z() >= minZ ) &&
           ( point.Z() <= maxZ );
  }
  return rval;
}
