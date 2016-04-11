#ifndef POCKETCORNERSLICE_H
#define POCKETCORNERSLICE_H

#include "Slice.h"

class PocketCornerSlice : public Slice
{
public:

  PocketCornerSlice();
  PocketCornerSlice(gp_Pnt& pnt,double alpha0,double alpha1,TopoDS_Wire& outerWire);
  init(gp_Pnt& pnt,double alpha0,double alpha1,TopoDS_Wire);
  virtual calc();

  Tool tool;
}

#endif//POCKETCORNERSLICE_H
