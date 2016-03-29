#include "PostProcessor.h"
#include "Slice.h"
#include <sstream>

using namespace std;

PostProcessor::PostProcessor(
    const double& z,
    const double& l, const double& a) :

  rapidZ(z),
  lScale(l),
  aScale(a)

{
}

string PostProcessor::postProcess(list<Slice>& s)
{
  stringstream gcode;
  gcode.precision(5);
  
  for (list<Slice>::iterator sliceIt = s.begin();
       sliceIt != s.end();
       ++sliceIt)
  {
    list<double>::iterator it_params = sliceIt->params.begin();
    list<gp_Pnt>::iterator it_points = sliceIt->points.begin();
    list<gp_Vec>::iterator it_normals = sliceIt->normals.begin();
    list<double>::iterator it_alphas = sliceIt->alphas.begin();

    gcode << "G0 X" << it_points->X()*lScale << " "
             << "Y" << it_points->Y()*lScale << " "
             << "Z" << rapidZ*lScale << " "
             << "A" << *it_alphas*aScale
             << endl;
    for (;it_params != sliceIt->params.end();
         ++it_params,++it_points,++it_normals,++it_alphas)
    {

       gcode << "G1 X" << it_points->X()*lScale << " "
           << "Y" << it_points->Y()*lScale << " "
           << "Z" << it_points->Z()*lScale << " "
           << "A" << *it_alphas*aScale << " "
           << "F" << 20
           << endl;
    }
    gcode << "G0 Z" << rapidZ*lScale << endl;
    
  }
  return gcode.str();
}
