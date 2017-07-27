#include "SplitterFactory.h"
#include "Splitter.h"
#include <iostream>

Splitter*
SplitterFactory::NewSplitter(const std::string& str)
{
  std::cout << str << std::endl;
  return new Splitter;
}
