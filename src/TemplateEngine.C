#include "TemplateEngine.h"
#include "TargetSurface.h"
#include "Slice.h"
#include "WorkingBoxSplitter.h"
#include "AlphaJumpMarker.h"
#include "TraverseAngleSplitter.h"
#include "NormalCrossingHorizontalMarker.h"
#include "WorkingBoxSplitter.h"
#include "PointToPointDistanceMarker.h"
#include "SurfaceNormalSplitter.h"
#include "gp_Dir.hxx"
#include <list>
#include <iostream>
#include "TopoDS_Edge.hxx"

TemplateEngine::TemplateEngine(const std::string& input)
{
  Json::Reader reader;
  reader.parse(input,root);
}

std::string
TemplateEngine::run()
{
  //std::cout<<root["file"]<<std::endl;
  Json::StyledWriter writer;//debug
  //std::cout<<writer.write(root)<<std::endl;
  TargetSurface surf(root["file"].asString());
  std::list<TopoDS_Edge> edges;
  for (Json::ValueIterator it = root["slices"].begin();
       it != root["slices"].end();
       ++it)
  {
    //std::cout << it->asString();
    //std::cout<<writer.write(*it)<<std::endl;
    double x = (*it)["dir"][0].asFloat();
    double y = (*it)["dir"][1].asFloat();
    double z = (*it)["dir"][2].asFloat();
    double step = (*it)["step"].asFloat();
    gp_Dir dir(x,y,z);
    edges.splice(edges.end(),surf.slice(dir,step));
  }

  std::list<Slice> slices;
  for (std::list<TopoDS_Edge>::iterator edge_it = edges.begin();
       edge_it != edges.end();
       ++edge_it)
  {
    slices.emplace(slices.end(),*edge_it,surf.face);
  }

  for (Json::ValueIterator it = root["operations"].begin();
       it != root["operations"].end();
       ++it)
  {
    std::string opType = (*it)["class"].asString();

    if (opType == "WorkingBoxSplitter")
    {
      std::list<Slice> newSlices;
      std::cout<<"wbs"<<std::endl;
      WorkingBoxSplitter wbs;
      double x0 = (*it)["xMin"].asFloat();
      double y0 = (*it)["yMin"].asFloat();
      double z0 = (*it)["zMin"].asFloat();
      double x1 = (*it)["xMax"].asFloat();
      double y1 = (*it)["yMax"].asFloat();
      double z1 = (*it)["zMax"].asFloat();
      wbs.setWorkingBox(x0,y0,z0,x1,y1,z1);
      for (std::list<Slice>::iterator slice_it = slices.begin();
           slice_it != slices.end();
           ++slice_it)
      {
        newSlices.splice(newSlices.end(),slice_it->split(wbs));
      }
      slices = newSlices;
    }

    else if (opType == "NormalCrossingHorizontalMarker")
    {
      std::cout<<"nchm"<<std::endl;
    }
    else if (opType == "SurfaceNormalSplitter")
    {
      std::cout<<"sns"<<std::endl;
    }
    else if (opType == "TraverseAngleSplitter")
    {
        std::cout<<"tas"<<std::endl;
    }
    else if (opType == "AlphaJumpMarker")
    {
        std::cout<<"ajm"<<std::endl;
    }
    else if (opType == "PointToPointDistanceMarker")
    {
        std::cout<<"p2pdm"<<std::endl;
    }
    else
    {
      std::cout<<"Warning: skipping unsupported operation of class: "<<
        opType << std::endl;
    }
    
  }

  std::string str;
  str += "poof\n";
  return str;
}
