#ifndef ROUNDWIRESLICE_H
#define ROUNDWIRESLICE_H

#include "Slice3D.h"

class RoundWireSlice : public Slice3D
{
public:

  RoundWireSlice(const TopoDS_Edge& edge, const TopoDS_Face& face,
      const bool flipSurface = false,
      const bool performNow=true, const double baseTol=0.05
    );

protected:

  Slice* emptyCopy();

  virtual void calc(const std::list<double>& params,
    std::list<gp_Pnt>& points,
    std::list<double>& alphas);

  bool flipSurface;
  double toolRadius;

};

#endif//ROUNDWIRESLICE_H
