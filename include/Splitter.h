#ifndef SPLITTER_H
#define SPLITTER_H

#include "gp_Vec.hxx"
#include "gp_Pnt.hxx"

class Slice;

class Splitter{
public:

  virtual bool
  evaluate(const Slice& slice, const double& param, const gp_Pnt& point,
                       const gp_Vec& normal, const double& alpha);

};
#endif //SPLITTER_H
