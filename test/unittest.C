#include "gtest/gtest.h"
#include <list>
#include <BRepPrimAPI_MakeBox.hxx>
#include "gp_Pnt.hxx"
#include "gp_Dir.hxx"
#include "gp_Pln.hxx"
#include "TopoDS_Face.hxx"
#include "TopoDS_Edge.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "TopoDS_Face.hxx"
#include "TargetSurface.h"
#include "Slice.h"
#include "SurfaceNormalSplitter.h"
#include "TraverseAngleSplitter.h"
#include "WorkingBoxSplitter.h"

using namespace std;

TEST(OCE, MakeBox) {
  BRepPrimAPI_MakeBox mb(10,10,10);
}

TEST(TargetSurface, ConstructFromFace) {
  gp_Pnt pnt(0,0,0);
  gp_Dir dir(1,0,0);
  gp_Pln pln(pnt,dir);
  BRepBuilderAPI_MakeFace mf(pln,0,10,0,10);
  TopoDS_Face face=mf.Face();
  TargetSurface surf(face);
}

TEST(TargetSurface, ConstructFromSTEP) {
  TargetSurface surf("single_surface.STEP");
}

class TargetSurfaceMemberFunctionTest : public ::testing::Test{
protected:
  virtual void SetUp(){
    gp_Pnt pnt(0,0,0);
    gp_Dir dir(0,0,1);
    gp_Pln pln(pnt,dir);
    BRepBuilderAPI_MakeFace mf(pln,-100,100,-100,100);
    TopoDS_Face face=mf.Face();
    tgtsurf = TargetSurface(face);
  }
  TargetSurface tgtsurf;
};

TEST_F(TargetSurfaceMemberFunctionTest, IntersectWithPlane){
  gp_Pnt pnt(0,0,0);
  gp_Dir dir(1,0,0);
  list<TopoDS_Edge> edges = tgtsurf.intersectWithPlane(pnt,dir);
  EXPECT_TRUE(edges.size() >= 1);
}

TEST_F(TargetSurfaceMemberFunctionTest, IntersectWithPlanes){
  gp_Pnt pnt(0,0,0);
  gp_Dir dir(1,0,0);
  gp_Vec vec(1,0,0);
  int n = 5;
  list<TopoDS_Edge> edges = tgtsurf.intersectWithPlanes(pnt,dir,vec,n);
  EXPECT_TRUE(edges.size() >= 1);
}

TEST_F(TargetSurfaceMemberFunctionTest, slice_default){
  gp_Dir dir(1,0,0);
  float spacing = 1.5;
  list<TopoDS_Edge> edges = tgtsurf.slice(dir,spacing);
  EXPECT_TRUE(edges.size() >= 1);
}

TEST_F(TargetSurfaceMemberFunctionTest, slice_allOptions){
  gp_Dir dir(1,0,0);
  float spacing = 1.5;
  list<TopoDS_Edge> edges = tgtsurf.slice(dir,10,0.08,false);
  EXPECT_TRUE(edges.size() >= 1);
}

TEST(Slice,construct){
  TargetSurface tgtsurf("single_surface.STEP");
  gp_Dir dir(1,1,0);
  list<TopoDS_Edge> edges = tgtsurf.slice(dir,25);
  Slice mySlice(edges.front(),tgtsurf.face);
  TopoDS_Edge e = mySlice.edge;
  TopoDS_Face f = mySlice.face;
  list<double> params = mySlice.params;
  list<gp_Pnt> pts = mySlice.points;
  list<gp_Vec> normals = mySlice.normals;
  list<double> alphas = mySlice.alphas;
}

class SliceOperations : public ::testing::Test{
protected:
  virtual void SetUp()
  {
    TargetSurface tgtsurf("single_surface.STEP");
    gp_Dir dir(1,1,0);
    list<TopoDS_Edge> edges = tgtsurf.slice(dir,5);
    for (list<TopoDS_Edge>::iterator edge_it = edges.begin();
         edge_it != edges.end();
         ++edge_it
        )
    {
      slices.emplace(slices.end(),*edge_it,tgtsurf.face);
    }
  }
  list<Slice> slices;
};

TEST_F(SliceOperations,SurfaceNormalSplitter){
  SurfaceNormalSplitter sns;
  sns.setZLimits(0,1.1);
  for (list<Slice>::iterator slice_it = slices.begin();
       slice_it != slices.end();
       ++slice_it
      )
  {
    slice_it->split(sns);
  }
}

TEST_F(SliceOperations,TraverseAngleSplitter){
  TraverseAngleSplitter sns;
  for (list<Slice>::iterator slice_it = slices.begin();
       slice_it != slices.end();
       ++slice_it
      )
  {
    slice_it->split(sns);
  }
}

TEST_F(SliceOperations,WorkingBoxSplitter){
  WorkingBoxSplitter sns;
  sns.setWorkingBox(400,-500,-200,1000,500,200);
  for (list<Slice>::iterator slice_it = slices.begin();
       slice_it != slices.end();
       ++slice_it
      )
  {
    slice_it->split(sns);
  }
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
