#include "NormalCrossingHorizontalMarker.h"
#include <cmath>

NormalCrossingHorizontalMarker::NormalCrossingHorizontalMarker() :
  maxDz(12345)
{
}

void
NormalCrossingHorizontalMarker::setMaxDz(const double& dz)
{
  maxDz = dz;
}

bool
NormalCrossingHorizontalMarker::evaluate(
    Slice& slice,
    double& paramHead, gp_Pnt& pointHead,
    gp_Vec& normalHead, double& alphaHead,
    double& paramTail, gp_Pnt& pointTail,
    gp_Vec& normalTail, double& alphaTail
  )
{
  double z1 = normalHead.Z();
  double z2 = normalTail.Z();
  return (z1*z2 < 0) && (abs(z2-z1)>maxDz);
}
