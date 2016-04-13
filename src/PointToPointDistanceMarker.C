#include "PointToPointDistanceMarker.h"
#include <iostream>
using namespace std;

PointToPointDistanceMarker::PointToPointDistanceMarker():
  maxDistance(123e45)
{
}


bool
PointToPointDistanceMarker::evaluate(Slice& slice,
      double& paramHead, gp_Pnt& pointHead,
      double& alphaHead,
      double& paramTail, gp_Pnt& pointTail,
      double& alphaTail
    )
{
  gp_Vec v1(pointHead.XYZ());
  gp_Vec v2(pointTail.XYZ());
  double delta = (v2-v1).Magnitude();
  return delta > maxDistance;
}

void
PointToPointDistanceMarker::setMaxDistance(const double& limit)
{
  maxDistance = limit;
}
