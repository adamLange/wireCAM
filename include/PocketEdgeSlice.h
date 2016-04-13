#ifndef POCKETEDGESLICE_H
#define POCKETEDGESLICE_H

#include "Slice.h"

class PocketEdgeSlice : public Slice
{

public:

  PocketEdgeSlice(TopoDS_Edge edge, bool performNow = true,
    double baseTol = 1e-3);

  virtual TopoDS_Shape shape();

private:

  TopoDS_Edge& edge;
  double baseTol;
  virtual Slice* emptyCopy();
  virtual void updateGeometry();
  virtual void calc(std::list<double>& params,
    std::list<gp_Pnt>& points, std::list<double>& alphas);
  void performBaseTessilation(std::list<double>& params,
    std::list<gp_Pnt>& points);

};

#endif//POCKETEDGESLICE_H
