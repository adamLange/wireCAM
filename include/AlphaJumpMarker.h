#ifndef ALPHAJUMPMARKER_H
#define ALPHAJUMPMARKER_H

#include "Marker.h"

class AlphaJumpMarker : public Marker
{
public:

  AlphaJumpMarker();
  
  void setMaxAlphaJump(const double& a);

  bool
  virtual evaluate(Slice& slice,
      double& paramHead, gp_Pnt& pointHead,
      double& alphaHead,
      double& paramTail, gp_Pnt& pointTail,
      double& alphaTail
    );

private:

  double maxAlphaJump;

};

#endif//ALPHAJUMPMARKER_H
