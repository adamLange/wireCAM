#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include <list>
#include <string>
#include <memory>
using namespace std;

class Slice;

class PostProcessor
{
public:
  PostProcessor(const double& z, const double& l, const double& a);

  string postProcess(list<unique_ptr<Slice>>& s);

private:
  double rapidZ,lScale,aScale;
};

#endif//POSTPROCESSOR_H
