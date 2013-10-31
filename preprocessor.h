#ifndef __PREPROCESSOR_H
#define __PREPROCESSOR_H

#include <cstdio>
#include <string>

extern FILE *yyin;
extern unsigned int curline;
extern std::string curfile;
extern bool skipping;

#define YY_DECL extern "C" int yylex (void)
YY_DECL;
void set_preprocess_only(bool);
int yyparse();
FILE *include_file(std::string &,bool other);
void set_real_file(FILE *,const std::string &);
void report_file();

#endif
