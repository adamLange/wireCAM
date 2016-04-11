#ifndef POCKETEDGESLICE_H
#define POCKETEDGESLICE_H

#include "Slice.h"

class PocketEdgeSlice : public Slice
{
public:
  PocketEdgeSlice();

private:
  TopoDSEdge& edge;

};

#endif//POCKETEDGESLICE_H
