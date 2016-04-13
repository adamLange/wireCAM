#ifndef POINTTOPOINTDISTANCEMARKER_H
#define POINTTOPOINTDISTANCEMARKER_H

#include "Marker.h"

class PointToPointDistanceMarker : public Marker
{
public:

  PointToPointDistanceMarker();

  bool
  virtual evaluate(Slice& slice,
      double& paramHead, gp_Pnt& pointHead,
      double& alphaHead,
      double& paramTail, gp_Pnt& pointTail,
      double& alphaTail
    );

  void setMaxDistance(const double& limit);

private:

  double maxDistance;
};

#endif//POINTTOPOINTDISTANCEMARKER_H
