#include "NormalCrossingHorizontalMarker.h"
#include <cmath>
#include "Slice.h"

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
    double& alphaHead,
    double& paramTail, gp_Pnt& pointTail,
    double& alphaTail
  )
{
  double z1 = slice.surfaceNormal(paramHead).Z();
  double z2 = slice.surfaceNormal(paramTail).Z();
  return (z1*z2 < 0) && (std::abs(z2-z1)>maxDz);
}
