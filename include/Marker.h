#ifndef MARKER_H
#define MARKER_H

#include "gp_Pnt.hxx"
#include "gp_Vec.hxx"

class Slice;

class Marker
{
public:

  
  virtual bool evaluate(Slice& slice,
      double& paramHead, gp_Pnt& pointHead,
      double& alphaHead,
      double& paramTail, gp_Pnt& pointTail,
      double& alphaTail
    ) = 0;

};

#endif//MARKER_H
