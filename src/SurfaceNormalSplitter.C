#include "SurfaceNormalSplitter.h"
#include "Slice3D.h"

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
    Slice& slice,
    const double& param, const gp_Pnt& point,
    const double& alpha
  )
{
  if (typeid(slice)==typeid(Slice3D))
  {
    Slice3D* s3d = dynamic_cast<Slice3D*>(&slice);
    gp_Dir normal(s3d->surfaceNormal(param));
    double x = normal.X();
    double y = normal.Y();
    double z = normal.Z();
    return (x>=xMin)&&(x<=xMax)&&
         (y>=yMin)&&(y<=yMax)&&
         (z>=zMin)&&(z<=zMax);
  }
}
