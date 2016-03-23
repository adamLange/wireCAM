#ifndef SURFACENORMALSPLITTER_H
#define SURFACENORMALSPLITTER_H

#include "Splitter.h"

class SurfaceNormalSplitter : public Splitter
{
public:
  
  SurfaceNormalSplitter();

  virtual bool
  evaluate(const double& param, const gp_Pnt& point,
                       const gp_Vec& normal, const double& alpha);

  void setXLimits(const double& xMin,const double& xMax);
  void setYLimits(const double& yMin,const double& yMax);
  void setZLimits(const double& zMin,const double& zMax);

private:

  double xMin,yMin,zMin,xMax,yMax,zMax;
  
};

#endif//SURFACENORMALSPLITTER_H
