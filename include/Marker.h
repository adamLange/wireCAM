#ifndef MARKER_H
#define MARKER_H

class Slice;

class Marker
{
public:

  bool
  virtual evaluate(Slice& slice,
      double& paramHead, gp_Pnt& pointHead,
      gp_Vec& normalHead, double& alphaHead,
      double& paramTail, gp_Pnt& pointTail,
      gp_Vec& normalTail, double& alphaTail
    ) = 0;

};

#endif//MARKER_H
