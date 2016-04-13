#include "gtest/gtest.h"
#include <list>
#include <memory>
#include <BRepPrimAPI_MakeBox.hxx>
#include "gp_Pnt.hxx"
#include "gp_Dir.hxx"
#include "gp_Pln.hxx"
#include "TopoDS_Face.hxx"
#include "TopoDS_Edge.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "TopoDS_Face.hxx"
#include "TargetSurface.h"
#include "Slice3D.h"
#include "SurfaceNormalSplitter.h"
#include "TraverseAngleSplitter.h"
#include "WorkingBoxSplitter.h"
#include "PointToPointDistanceMarker.h"
#include "AlphaJumpMarker.h"
#include "NormalCrossingHorizontalMarker.h"
#include "PostProcessor.h"
#include "Marker.h"
#include "SplitterFactory.h"
#include "json/json.h"
#include "TemplateEngine.h"
#include <sstream>
#define _USE_MATH_DEFINES

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

TEST(Slice3D,construct){
  TargetSurface tgtsurf("single_surface.STEP");
  gp_Dir dir(1,1,0);
  list<TopoDS_Edge> edges = tgtsurf.slice(dir,25);
  Slice3D mySlice(edges.front(),tgtsurf.face);
  TopoDS_Edge e = mySlice.edge;
  TopoDS_Face f = mySlice.face;
  list<double> params = mySlice.params;
  list<gp_Pnt> pts = mySlice.points;
  list<double> alphas = mySlice.alphas;
}

class Slice3DOperations : public ::testing::Test{
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
      slices.emplace_back(new Slice3D(*edge_it,tgtsurf.face));
    }
  }
  list<unique_ptr<Slice>> slices;
};

TEST_F(Slice3DOperations,SurfaceNormalSplitter){
  SurfaceNormalSplitter sns;
  sns.setZLimits(0,1.1);
  for (list<unique_ptr<Slice>>::iterator slice_it = slices.begin();
       slice_it != slices.end();
       ++slice_it
      )
  {
    (*slice_it)->split(sns);
  }
}

TEST_F(Slice3DOperations,TraverseAngleSplitter){
  TraverseAngleSplitter sns;
  for (list<unique_ptr<Slice>>::iterator slice_it = slices.begin();
       slice_it != slices.end();
       ++slice_it
      )
  {
    (*slice_it)->split(sns);
  }
}

TEST_F(Slice3DOperations,WorkingBoxSplitter){
  WorkingBoxSplitter sns;
  sns.setWorkingBox(400,-500,-200,1000,500,200);
  for (list<unique_ptr<Slice>>::iterator slice_it = slices.begin();
       slice_it != slices.end();
       ++slice_it
      )
  {
    (*slice_it)->split(sns);
  }
}

TEST_F(Slice3DOperations,PointToPointDistanceMarker){
  PointToPointDistanceMarker m;
  m.setMaxDistance(4);
  for (list<unique_ptr<Slice>>::iterator slice_it = slices.begin();
       slice_it != slices.end();
       ++slice_it
      )
  {
    (*slice_it)->refine(m);
  }
}

TEST_F(Slice3DOperations,AlphaJumpMarker){
  AlphaJumpMarker m;
  m.setMaxAlphaJump(M_PI/100);
  for (list<unique_ptr<Slice>>::iterator slice_it = slices.begin();
       slice_it != slices.end();
       ++slice_it
      )
  {
    (*slice_it)->refine(m);
  }
}

TEST_F(Slice3DOperations,NormalCrossingHorizontalMarker){
  NormalCrossingHorizontalMarker m;
  m.setMaxDz(0.05);
  for (list<unique_ptr<Slice>>::iterator slice_it = slices.begin();
       slice_it != slices.end();
       ++slice_it
      )
  {
    (*slice_it)->refine(m);
  }
}

TEST(PostProcessor,postProcess){
  TargetSurface tgtsurf("single_surface.STEP");
  gp_Dir dir(1,1,0);
  list<unique_ptr<Slice>> slices;
  list<TopoDS_Edge> edges = tgtsurf.slice(dir,5);
  for (list<TopoDS_Edge>::iterator edge_it = edges.begin();
       edge_it != edges.end();
       ++edge_it
      )
  {
    slices.emplace_back(new Slice3D(*edge_it,tgtsurf.face));
  }

  PostProcessor pp(50,1/25.4,180/M_PI);
  cout<<pp.postProcess(slices);
}

TEST(FullStack,wingToGCode){
  TargetSurface tgtsurf("single_surface.STEP");
  gp_Dir dir(1,1,0);
  list<unique_ptr<Slice>> slices;
  list<TopoDS_Edge> edges = tgtsurf.slice(dir,5);
  for (list<TopoDS_Edge>::iterator edge_it = edges.begin();
       edge_it != edges.end();
       ++edge_it
      )
  {
    slices.emplace_back(new Slice3D(*edge_it,tgtsurf.face));
  }

  PostProcessor pp(10,1/25.4,180/M_PI);
  cout<<pp.postProcess(slices);

}

TEST(JSONCPP,ParseJson){
  ifstream in("fullStack.json");
  Json::Value book_json;
  in >> book_json;
  in.close();
}

TEST(TEMPLATEENGINE,DOIT){
  ifstream in("./fullStack.json");
  stringstream raw;
  raw << in.rdbuf();
  //cout << raw.str() <<endl;
  in.close();
  TemplateEngine engine(raw.str());
  engine.run();
}

#include "PocketTests.C"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
