#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include "compiler_main.h"
//#include "preprocessor_expression_evaluation.h"
#include "mudconfig.h"
//XXX fixme debug:
#include <string.h>

using namespace std;

int main(int argc,char *argv[])
 {
  eat_config("/home/imi/Munkaasztal/ds2.8.4/bin/mudos.cfg");
  if(!check_config()) return 1;
  //return compile("/obj/stargate",0);
  //return compile("/secure/daemon/master",0);
  //return compile("/parser_tests/11",1);

  argv[1][strlen(argv[1])-2]='\0';
  argv[1]++;
  cerr<<argv[1]<<endl;
  return compile(argv[1],0);
  //return compile(getval("master file").c_str(),0);
  return 0;
  /*prep_eval::reset();
  prep_eval::receive_int(3);
  prep_eval::receive_op("&&");
  cerr<<__FILE__<<':'<<__LINE__<<endl;
  bool l=prep_eval::is_valid();
  cerr<<__FILE__<<':'<<__LINE__<<endl;
  cerr<<l<<endl;
  cerr<<prep_eval::result()<<endl;
  return 0;*/

  if(argc>=2)
    eat_config(argv[1]);//return compile(argv[1],1);
   else
    {
     cerr<<"you should provide a config file as a driver parameter.";
     return 1;
    }

  if(!check_config()) return 1;
 }
