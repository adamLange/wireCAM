#ifndef POCKETCORNERSLICE_H
#define POCKETCORNERSLICE_H

#include "Slice.h"
#include "Pocket.h"
#include "TopoDS_Wire.hxx"
#include "Standard_Transient.hxx"
#include "BRepAdaptor_HCompCurve.hxx"

class PocketCornerSlice : public Slice
{
public:

  PocketCornerSlice(Pocket& pocket, const gp_Pnt& pnt,
    const double& alpha0, const double& alpha1,bool performNow = true
  );
  //virtual TopoDS_Shape shape();

private:
  Pocket& pocket;
  gp_Pnt pivotPnt; //pivot point
  double alpha0,alpha1;
  gp_Dir wireNormal;
  Handle(BRepAdaptor_HCompCurve) cc;
  virtual void calc(const std::list<double>& params, 
    std::list<gp_Pnt>& points, std::list<double>& alphas);

  //virtual Slice* emptyCopy();
  //virtual void updateGeometry();
};

#endif//POCKETCORNERSLICE_H
