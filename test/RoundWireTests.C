#include "RoundWireSlice.h"

TEST(RoundWireSlice,Construct){
  TargetSurface tgtsurf("single_surface.STEP");
  gp_Dir dir(1,1,0);
  list<unique_ptr<Slice>> slices;
  list<TopoDS_Edge> edges = tgtsurf.slice(dir,5);
  for (list<TopoDS_Edge>::iterator edge_it = edges.begin();
       edge_it != edges.end();
       ++edge_it
      )
  {
    slices.emplace_back(new RoundWireSlice(*edge_it,tgtsurf.face,10));
  }
}
