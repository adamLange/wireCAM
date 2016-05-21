#ifndef EDGESLICE_H
#define EDGESLICE_H

#include "Slice.h"

class EdgeSlice : public Slice
{

public:

  EdgeSlice(TopoDS_Edge edge, bool performNow = true,
    double baseTol = 1e-3, double alphaVertical = 0);

  virtual TopoDS_Shape shape();

private:

  TopoDS_Edge edge;
  double baseTol;
  virtual Slice* emptyCopy();
  virtual void updateGeometry();
  virtual void calc(std::list<double>& params,
    std::list<gp_Pnt>& points, std::list<double>& alphas);
  void performBaseTessilation(std::list<double>& params,
    std::list<gp_Pnt>& points);
  double alphaVertical;

};

#endif//EDGESLICE_H
