#ifndef WORKINGBOXSPLITTER_H
#define WORKINGBOXSPLITTER_H

#include "Splitter.h"

class Slice;

class WorkingBoxSplitter : public Splitter
{
public:

  WorkingBoxSplitter();

  virtual bool
  evaluate(Slice& slice, const double& param, const gp_Pnt& point,
                       const double& alpha);

  void
  setWorkingBox(const double& x0, const double& y0, const double& z0,
                const double& x1, const double& y1, const double& z1);

private:

  bool infinite;
  double minX,minY,minZ,maxX,maxY,maxZ;

};

#endif//WORKINGBOXSPLITTER_H
