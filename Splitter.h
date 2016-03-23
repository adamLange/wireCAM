#ifndef SPLITTER_H
#define SPLITTER_H

class Splitter{
public:

  virtual bool
  evaluate(const double& param, const gp_Pnt& point,
                       const gp_Vec& normal, const double& alpha) = 0;

};
#endif //SPLITTER_H
