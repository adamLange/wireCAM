#include "SurfaceNormalSplitter.h"

SurfaceNormalSplitter::SurfaceNormalSplitter()
{
  xMin = yMin = zMin = -1.1;
  xMax = yMax = zMax =  1.1;
}

void 
SurfaceNormalSplitter::setXLimits(const double& xMin,const double& xMax)
{
  this->xMin = xMin;
  this->xMax = xMax;
}

void 
SurfaceNormalSplitter::setYLimits(const double& yMin,const double& yMax)
{
  this->yMin = yMin;
  this->yMax = yMax;
}

void 
SurfaceNormalSplitter::setZLimits(const double& zMin,const double& zMax)
{
  this->zMin = zMin;
  this->zMax = zMax;
}

bool
SurfaceNormalSplitter::evaluate(
    const Slice& slice,
    const double& param, const gp_Pnt& point,
    const gp_Vec& normal, const double& alpha
  )
{
  double x = normal.X();
  double y = normal.Y();
  double z = normal.Z();
  return (x>=xMin)&&(x<=xMax)&&
         (y>=yMin)&&(y<=yMax)&&
         (z>=zMin)&&(z<=zMax);
}
