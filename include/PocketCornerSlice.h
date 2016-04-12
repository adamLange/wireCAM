#ifndef POCKETCORNERSLICE_H
#define POCKETCORNERSLICE_H

#include "Slice.h"
class TopoDS_Wire();

class PocketCornerSlice : public Slice
{
public:

  PocketCornerSlice(const gp_Pnt& pnt,const double& alpha0,
    const double& alpha1,TopoDS_Wire& outerWire,const double& toolR
  );
  virtual calc();

private:
  gp_Pnt pivotPnt; //pivot point
  TopoDS_Wire outerWire;
  double r; //tool radius
  gp_Dir wireNormal;
  Handle(BRepAdaptor_CompCurve) cc;
}

#endif//POCKETCORNERSLICE_H
