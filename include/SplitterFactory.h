#ifndef SPLITTERFACTORY_H
#define SPLITTERFACTORY_H

#include <string>

class Splitter;

using namespace std;

class SplitterFactory
{
public:
  static Splitter *NewSplitter(const string& str);
};

#endif//SPLITTERFACTORY_H
