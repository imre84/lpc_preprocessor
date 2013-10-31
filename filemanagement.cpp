#include "filemanagement.h"
#include "mudconfig.h"

#include <cstdio>
#include <string>

using namespace std;

FILE *myfopen(const string &fn,const char *mode)
 {
  return fopen((getval("mudlib directory")+fn).c_str(),mode);
 }
