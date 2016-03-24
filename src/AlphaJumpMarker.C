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
    gp_Vec& normalHead, double& alphaHead,
    double& paramTail, gp_Pnt& pointTail,
    gp_Vec& normalTail, double& alphaTail
  ) 
{
  return abs(alphaHead - alphaTail) > maxAlphaJump;
}
