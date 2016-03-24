#ifndef NORMALCROSSINGHORIZONTALMARKER_H
#define NORMALCROSSINGHORIZONTALMARKER_H
  
#include "Marker.h"

class NormalCrossingHorizontalMarker : public Marker
{
public:

  NormalCrossingHorizontalMarker();

  void setMaxDz(const double& dz);

  virtual bool evaluate(Slice& slice,
      double& paramHead, gp_Pnt& pointHead,
      gp_Vec& normalHead, double& alphaHead,
      double& paramTail, gp_Pnt& pointTail,
      gp_Vec& normalTail, double& alphaTail
    );

private:

  double maxDz;
};

#endif//NORMALCROSSINGHORIZONTALMARKER_H
