#ifndef TRAVERSEANGLESPLITTER_H
#define TRAVERSEANGLESPLITTER_H

#include "Splitter.h"

class TraverseAngleSplitter : public Splitter
{
public:

  TraverseAngleSplitter();

  virtual bool
  evaluate(Slice& slice, const double& param, const gp_Pnt& point,
                       const double& alpha);

  void
  setMaxTraverseAngle(const double& angle);

private:

  double maxTraverseAngle;

};

#endif//TRAVERSEANGLESPLITTER_H
