#include "AlphaJumpMarker.h"
#include <cmath>
#define _USE_MATH_DEFINES

AlphaJumpMarker::AlphaJumpMarker():
  maxAlphaJump(2*M_PI)
{
}

void
AlphaJumpMarker::setMaxAlphaJump(const double& a)
{
  maxAlphaJump = a;
}

bool
AlphaJumpMarker::evaluate(Slice& slice,
    double& paramHead, gp_Pnt& pointHead,
    double& alphaHead,
    double& paramTail, gp_Pnt& pointTail,
    double& alphaTail
  ) 
{
  return std::abs(alphaHead - alphaTail) > maxAlphaJump;
}
