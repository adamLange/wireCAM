#ifndef TEMPLATEENGINE_H
#define TEMPLATEENGINE_H

#include <string>
#include "json/json.h"

class TemplateEngine
{
public:

TemplateEngine(const std::string& input);

void run();

private:
Json::Value root;

};

#endif//TEMPLATEENGINE_H
