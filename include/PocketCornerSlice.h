#ifndef POCKETCORNERSLICE_H
#define POCKETCORNERSLICE_H

#include "Slice.h"
#include "TopoDS_Wire.hxx"
#include "Standard_Transient.hxx"
#include "BRepAdaptor_HCompCurve.hxx"

class PocketCornerSlice : public Slice
{
public:

  PocketCornerSlice(const gp_Pnt& pnt,const double& alpha0,
    const double& alpha1,TopoDS_Wire& outerWire,const double& toolR
  );

private:
  gp_Pnt pivotPnt; //pivot point
  TopoDS_Wire outerWire;
  double r; //tool radius
  gp_Dir wireNormal;
  Handle(BRepAdaptor_HCompCurve) cc;
  virtual void calc(const std::list<double>& params, 
    std::list<gp_Pnt>& points, std::list<double>& alphas);
};

#endif//POCKETCORNERSLICE_H
