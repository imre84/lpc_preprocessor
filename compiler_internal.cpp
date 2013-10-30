#include "compiler_internal.h"

#include <iostream>

using namespace std;

void yyerror(const char *s,const severity /*t*/)
 {
  cerr<<curfile<<':'<<curline<<": "<<s/*<<'+'*/<<endl;
 }
