#include <iostream>
#include <sstream>
#include <fstream>
#include "TemplateEngine.h"

int main (int argc, char** argv)
{
  if (argc > 1)
  {
    std::ifstream in(argv[1]);
    std::stringstream data;
    data << in.rdbuf();
    TemplateEngine engine(data.str());
    engine.run();
    return 0;
  }
  else
  {
    std::cerr << "\n";
    return 1;
  }  
}
