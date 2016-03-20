#include "gtest/gtest.h"
#include <list>
#include <BRepPrimAPI_MakeBox.hxx>
#include "gp_Pnt.hxx"
#include "gp_Dir.hxx"
#include "gp_Pln.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "TopoDS_Face.hxx"
#include "TargetSurface.h"

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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
