#ifndef __MUDCONFIG_H
#define __MUDCONFIG_H

#include <string>

void eat_config(const char *fn);
std::string getval(const std::string &var);
bool check_config();

#endif
