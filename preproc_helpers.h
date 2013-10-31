#ifndef __PREPROC_HELPERS_H
#define __PREPROC_HELPERS_H

#include <string>
#include <iostream>
#include <stack>

#define NEXTLINE       nextline()

#include "compiler_internal.h"

enum state {p_ifdef, p_ifndef, p_include, p_line};

unsigned int curline;
std::string curfile;
std::string strconst,define_id,define_parlist;
bool skipping; //Ha az inputot csak eldobjuk.
int previous; //Melyik állapotba kell visszalépni ha tudtam helyettesíteni egy ID-t.
int err_id; //Melyik állapotba kell visszalépni ha nem tudtam helyettesíteni egy ID-t.
std::stack<bool> ifstack;
int iffdepth=0;
state parsing;
bool preprocess_only;
unsigned int MYINITIAL;
FILE *real_file;
std::string real_curfile;
bool was_nl=false;

bool include(std::string,bool other=false);
bool other_include(char *line);//#include <anything>
void buffer(const char *buf);
std::string preprocess_id(const char *id,const char *paramlist=0,const char *unsuccessful=0);
void handle_id(int err_id_state);
void skip_then_branch();
//void take_then_branch(int nexts=eat_nl);
void take_then_branch(int nexts);
void take_then_branch();
void nextline();
void report_file();
bool finish_include();
void undefine();

//XXX fixme unput,duplikált unput
//XXX fixme az értéket is visszaadni...
#define STRING_ENDS \
     unput(yytext[0]); \
     BEGIN((YY_START!=npid_after_stringconstant)?MYINITIAL: \
                (preprocess_only?npid_preprocessing:npid_compiling)); \
     if(was_nl) \
      { \
       unput('\n'); \
       was_nl=false; \
      } \
     if(preprocess_only) \
       fputs("\" ",yyout); \
      else \
       { \
        strconst=""; \
        return STRCONST; \
       }

#endif
