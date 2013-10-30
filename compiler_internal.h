#ifndef __COMPILER_INTERNAL_H
#define __COMPILER_INTERNAL_H

#include <string>

enum severity {error,warning};

void yyerror(const char *s,const severity t=error);

extern unsigned int curline;
extern std::string curfile;

#endif
