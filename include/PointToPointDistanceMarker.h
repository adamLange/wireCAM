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
      gp_Vec& normalHead, double& alphaHead,
      double& paramTail, gp_Pnt& pointTail,
      gp_Vec& normalTail, double& alphaTail
    );

  void setMaxDistance(const double& limit);

private:

  double maxDistance;
};

#endif//POINTTOPOINTDISTANCEMARKER_H
