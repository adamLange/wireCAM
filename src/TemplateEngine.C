#include "TemplateEngine.h"
#include "TargetSurface.h"
#include "Slice.h"
#include "Pocket.h"
#include "Slice3D.h"
#include "RoundWireSlice.h"
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
#include "TopoDS_Compound.hxx"
#include "BRepTools.hxx"
#include "BRep_Tool.hxx"
#include "BRep_Builder.hxx"
#include "PostProcessor.h"
#include "TopoDS.hxx"
#include <stdexcept>
#include <exception>
#include "TopExp_Explorer.hxx"

#define _USE_MATH_DEFINES

TemplateEngine::TemplateEngine(const std::string& input)
{
  Json::Reader reader;
  reader.parse(input,root);
}

void
TemplateEngine::run()
{
  //read in the tools
  //create the slices
  if ((root["class"]=="SquareWireCarve")
     |(root["class"]=="RoundWireCarve"))
  {

    //std::cout<<root["file"]<<std::endl;
    //Json::StyledWriter writer;//debug
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

    std::cout<< "Creating slices..." << std::endl;

    std::list<std::unique_ptr<Slice>> slices;
    for (std::list<TopoDS_Edge>::iterator edge_it = edges.begin();
         edge_it != edges.end();
         ++edge_it)
    {
      if (root["class"]=="SquareWireCarve")
      {
        slices.emplace_back(new Slice3D(*edge_it,surf.face));
      }
      else if (root["class"]=="RoundWireCarve")
      {
        //hardcode !
        slices.emplace_back(new RoundWireSlice(*edge_it,surf.face,10));
      }
      else
      {
        throw std::runtime_error("Unsupported slice type");
      }
    }

    std::cout<<"  "<<slices.size()<<" slices created"<< std::endl;

    std::cout<<"Applying slice modification operations..."<<std::endl;
    for (Json::ValueIterator it = root["operations"].begin();
         it != root["operations"].end();
         ++it)
    {
      std::string opType = (*it)["class"].asString();

      if (opType == "WorkingBoxSplitter")
      {
        std::list<std::unique_ptr<Slice>> newSlices;
        std::cout<<"  WorkingBoxSplitter"<< std::endl;
        WorkingBoxSplitter wbs;
        double x0 = (*it)["xMin"].asFloat();
        double y0 = (*it)["yMin"].asFloat();
        double z0 = (*it)["zMin"].asFloat();
        double x1 = (*it)["xMax"].asFloat();
        double y1 = (*it)["yMax"].asFloat();
        double z1 = (*it)["zMax"].asFloat();
        wbs.setWorkingBox(x0,y0,z0,x1,y1,z1);
        for (std::list<std::unique_ptr<Slice>>::iterator
             slice_it = slices.begin();
             slice_it != slices.end();
             ++slice_it)
        {
          std::list<std::unique_ptr<Slice>> dbgSlices = (*slice_it)->split(wbs);
          newSlices.splice(newSlices.end(),dbgSlices);
        }
        int initSize = slices.size();
        slices = std::move(newSlices);
        std::cout<<"    "<<initSize<<"->"<<slices.size()<<" slices"<< std::endl;
      }

      else if (opType == "NormalCrossingHorizontalMarker")
      {
        std::cout<<"  NormalCrossingHorizontalMarker"<<std::endl;
        NormalCrossingHorizontalMarker nchm;
        nchm.setMaxDz((*it)["maxDz"].asFloat());
        for (std::list<std::unique_ptr<Slice>>::iterator slice_it = slices.begin();
             slice_it != slices.end();
             ++slice_it)
        {
          long initSize = (*slice_it)->params.size();
          (*slice_it)->refine(nchm);
          long finalSize = (*slice_it)->params.size();
          std::cout<<"    "<<initSize<<" -> "<<finalSize<<std::endl;
        }
      }
      else if (opType == "SurfaceNormalSplitter")
      {
        std::cout<<"  SurfaceNormalSplitter"<<std::endl;
        SurfaceNormalSplitter sns;
        std::list<std::unique_ptr<Slice>> newSlices;
        double x0 = (*it)["xMin"].asFloat();
        double y0 = (*it)["yMin"].asFloat();
        double z0 = (*it)["zMin"].asFloat();
        double x1 = (*it)["xMax"].asFloat();
        double y1 = (*it)["yMax"].asFloat();
        double z1 = (*it)["zMax"].asFloat();
        sns.setXLimits(x0,x1);
        sns.setYLimits(y0,y1);
        sns.setZLimits(z0,z1);
        for (std::list<std::unique_ptr<Slice>>::iterator slice_it = slices.begin();
             slice_it != slices.end();
             ++slice_it)
        {
          newSlices.splice(newSlices.end(),(*slice_it)->split(sns));
        }
        int size_i = slices.size();
        slices = std::move(newSlices);
        int size_f = slices.size();
        std::cout<<"    "<<size_i<<" -> "<<size_f<<std::endl;
      }
      else if (opType == "TraverseAngleSplitter")
      {
        std::cout<<"  TraverseAngleSplitter"<<std::endl;
        std::list<std::unique_ptr<Slice>> newSlices;
        TraverseAngleSplitter tas;
        double alpha = (*it)["maxTraverseAngle"].asFloat()*M_PI/180.0;
        tas.setMaxTraverseAngle(alpha);
        for (std::list<std::unique_ptr<Slice>>::iterator slice_it = slices.begin();
             slice_it != slices.end();
             ++slice_it)
        {
          newSlices.splice(newSlices.end(),(*slice_it)->split(tas));
        }
        int size_i = slices.size();
        slices = std::move(newSlices);
        int size_f = slices.size();
        std::cout<<"    "<<size_i<<" -> "<<size_f<<std::endl;
      }
      else if (opType == "AlphaJumpMarker")
      {
        std::cout<<"  AlphaJumpMarker"<<std::endl;
        AlphaJumpMarker ajm;
        double j = (*it)["maxAlphaJump"].asFloat()*M_PI/180.0;
        ajm.setMaxAlphaJump(j);
        for (std::list<std::unique_ptr<Slice>>::iterator slice_it = slices.begin();
             slice_it != slices.end();
             ++slice_it)
        {
          long initSize = (*slice_it)->params.size();
          (*slice_it)->refine(ajm);
          long finalSize = (*slice_it)->params.size();
          std::cout<<"    "<<initSize<<" -> "<<finalSize<<std::endl;
        }
      }
      else if (opType == "PointToPointDistanceMarker")
      {
        std::cout<<"  PointToPointDistanceMarker"<<std::endl;
        PointToPointDistanceMarker p2pdm;
        double d = (*it)["maxDistance"].asFloat();
        p2pdm.setMaxDistance(d);
        for (std::list<std::unique_ptr<Slice>>::iterator slice_it = slices.begin();
             slice_it != slices.end();
             ++slice_it)
        {
          long initSize = (*slice_it)->params.size();
          (*slice_it)->refine(p2pdm);
          long finalSize = (*slice_it)->params.size();
          std::cout<<"    "<<initSize<<" -> "<<finalSize<<std::endl;
        }
      }
      else
      {
        std::cout<<"Warning: skipping unsupported operation of class: "<<
          opType << std::endl;
      }

    }

    double lScale = root["postProcessor"]["linearScale"].asFloat();
    double aScale = root["postProcessor"]["angularScale"].asFloat();
    double rapidZ = root["postProcessor"]["rapidZ"].asFloat();

    PostProcessor pp(rapidZ,lScale,aScale);

    string gcodeFile = root["output"]["machineCode"].asString();

    ofstream outFile(gcodeFile);
    //std::string str = pp.postProcess(slices);
    outFile << pp.postProcess(slices);
    outFile.close();

    TopoDS_Compound outShape;
    BRep_Builder builder;
    builder.MakeCompound(outShape);
    builder.Add(outShape,surf.face);
    for (std::list<std::unique_ptr<Slice>>::iterator slice_it = slices.begin();
           slice_it != slices.end();
           ++slice_it)
    {
      builder.Add(outShape,(*slice_it)->shape());
    }

    string geomFileName = root["output"]["geometry"].asString();
    ofstream geomFile(geomFileName);
    BRepTools bt;
    bt.Write(outShape,geomFile);
    geomFile.close();
  }
  else if (root["class"].asString()=="Pocket")
  {
    BRepTools bt;
    BRep_Builder bb;
    TopoDS_Shape shape;
    std::string filepath = root["file"].asString();
    double offset(root["offset"].asFloat());
    double zOffset(root["zOffset"].asFloat());
    double toolR(root["toolR"].asFloat());
    double step(root["step"].asFloat());
    bool squareCorners(root["squareCorners"].asBool());
    bool orderOut(root["orderOut"].asBool());

    double lScale = root["postProcessor"]["linearScale"].asFloat();
    double aScale = root["postProcessor"]["angularScale"].asFloat();
    double rapidZ = root["postProcessor"]["rapidZ"].asFloat();

    PostProcessor pp(rapidZ,lScale,aScale);

    bt.Read(shape,filepath.c_str(),bb);
    std::stringstream str;
    TopExp_Explorer exp;
    TopoDS_Compound comp;
    bb.MakeCompound(comp);
    for (exp.Init(shape,TopAbs_WIRE);exp.More();exp.Next())
    {
      TopoDS_Wire w = TopoDS::Wire(exp.Current());
      Pocket p(w,offset,zOffset,toolR,step,squareCorners,orderOut);
      str<<p.postProcess(pp);
      bb.Add(comp,p.dumpWires());
    }
    string gcodeFile = root["output"]["machineCode"].asString();
    ofstream outFile(gcodeFile);
    outFile << str.str();
    outFile.close();

    string geomFileName = root["output"]["geometry"].asString();
    ofstream geomFile(geomFileName.c_str());
    bt.Write(comp,geomFile);
    geomFile.close();
  } //make transitions

  return;
}
