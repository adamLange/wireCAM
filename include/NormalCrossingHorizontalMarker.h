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
      double& alphaHead,
      double& paramTail, gp_Pnt& pointTail,
      double& alphaTail
    );

private:

  double maxDz;
};

#endif//NORMALCROSSINGHORIZONTALMARKER_H
