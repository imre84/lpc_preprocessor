#include "compiler_main.h"
#include "preprocessor.h"
#include "preproc_define.h"
#include "mudconfig.h"
#include "filemanagement.h"

#include <iostream>

#include <string>

using namespace std;

int compile(const char *fn,bool preprocess_only)
 {
#ifdef MY_YY_INPUT_INIT
  MY_YY_INPUT_INIT;
#endif

  curfile=fn+string(".c");

  FILE *real_file;
  real_file=myfopen(curfile,"r");
  if(!real_file)
   {
    std::cerr<<"There is no such file "<<curfile<<std::endl;
    return 1;
   }
  set_real_file(real_file,curfile);

  string globalinclude=getval("global include file");
  bool other=globalinclude[0]=='<';
  globalinclude=globalinclude.substr(1,globalinclude.length()-2);
  yyin=include_file(globalinclude,other);
  curfile=globalinclude;

  init_define_table();
  curline=1;
  skipping=false;
  set_preprocess_only(preprocess_only);

  report_file();

  return preprocess_only?yylex():yyparse();
 }
