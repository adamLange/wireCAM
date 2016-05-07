#include "RoundWireSlice.h"

RoundWireSlice::RoundWireSlice(const TopoDS_Edge& edge, const TopoDS_Face& face,
    const bool performNow, const double baseTol
  ):
  Slice3D(edge,face,performNow,baseTol)
{
  if (performNow)
  {
    performBaseTessilation(params,points);
    calc(params,points,alphas);
  }
}

Slice*
RoundWireSlice::emptyCopy()
{
  return new RoundWireSlice(this->edge,this->face,false,this->baseTol);
}

RoundWireSlice::calc()
{
}
