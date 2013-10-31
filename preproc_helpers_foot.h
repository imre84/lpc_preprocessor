#ifndef __PREPROC_HELPERS_FOOT_H
#define __PREPROC_HELPERS_FOOT_H

#include "mudconfig.h"
#include "filemanagement.h"

struct xbufstate
 {
  YY_BUFFER_STATE bufstate;
  std::string filename;
  unsigned int curline;
  xbufstate(YY_BUFFER_STATE bufstate,std::string filename,int curline):
                   bufstate(bufstate),filename(filename),curline(curline){}
 };

std::stack<xbufstate> bufstack;

void my_switch_to_buffer(YY_BUFFER_STATE x,const std::string &fn,const int ln)
 {
  curline=ln;
  curfile=fn;
  yy_switch_to_buffer(x);
 }

void my_switch_to_buffer(FILE *x,const std::string &fn)
 {
  my_switch_to_buffer(yy_create_buffer(x,YY_BUF_SIZE),fn,1);
 }

void my_switch_to_buffer(xbufstate &x)
 {
  my_switch_to_buffer(x.bufstate,x.filename,x.curline);
 }

void push_current_buffer()
 {
  bufstack.push(xbufstate(YY_CURRENT_BUFFER,curfile,curline));
 }

FILE *include_try(const std::string &fn)
 {
  FILE *x=myfopen(fn,"r");
  return x;
 }

string slice_one(string &s,char delim)
 {
  string::size_type x=s.find_first_of(delim);
  string ret=s.substr(0,x);
  if(x!=string::npos)
    s=s.substr(x+1);
   else
    s="";
  return ret;
 }

FILE *include_file(std::string &fn,bool other)
 { //other syntax=#include <anything>
  FILE *ok=0;
  string curfn;

  if(fn[0]!='/')
    {
     string curdir=curfile.substr(0,curfile.find_last_of('/'));
     curfn=curdir+'/'+fn;

     if(!other) ok=include_try(curfn);
     string dirs=getval("include directories");
     while((!ok)&&dirs.length())
      {
       string mydir=slice_one(dirs,':');
       curfn=mydir+'/'+fn;
       ok=include_try(curfn);
      }
     if((!ok)&&other)
      {
       curfn=curdir+'/'+fn;
       ok=include_try(curfn);
      }
    }
   else
    ok=include_try(curfn=fn);

  if(!ok)
    {
     --curline;
     yyerror("no such file.",error);
     ++curline;
    }
   else
    fn=curfn;

  return ok;
 }

bool include(std::string fn,bool other)
 {
  FILE *x=include_file(fn,other);
  if(!x) return false;
  push_current_buffer();
  my_switch_to_buffer(x,fn);
  report_file();
  return true;
 }

extern "C" int yywrap()
 {
  if(!bufstack.empty())
   {
    yy_delete_buffer(YY_CURRENT_BUFFER);
    bool l=(bufstack.top().filename!=curfile)||(bufstack.top().curline!=curline);
    my_switch_to_buffer(bufstack.top());
    if(l) report_file();
    bufstack.pop();
    return 0;
   }

  if(real_file)
   {
    yyin=real_file;
    curline=1;
    curfile=real_curfile;
    real_file=0;
    report_file();
    return 0;
   }

  return 1;
 }

std::string get_param(std::string &l,std::string::size_type &p,char delim)
 {
  std::string::size_type p1=l.find_first_of(delim,p);
  std::string apar=l.substr(p,p1-p);
  p=p1+1;
  return apar;
 }

#define UNSUCCESSFUL_PREPROCESS {BEGIN(err_id);return unsuccessful;}
std::string preprocess_id(const char *id,const char *paramlist,const char *unsuccessful)
 {
  const define_r &x=*get_entry(id);
  std::string replaced=x.content;

  if(replaced.substr(0,3)=="\n\n\n")
   {
    bool ok;
    replaced=preproc_routine(replaced.substr(3).c_str(),paramlist,ok);
    if(ok)
     return replaced;

    UNSUCCESSFUL_PREPROCESS;
   }
  if(paramlist)
   {
    std::string fparlist=x.paramlist;
    std::string aparlist=paramlist;
    std::string::size_type ap=0;
    std::string::size_type fp=0;

    while(aparlist.length()>ap)
     {
      std::string apar=get_param(aparlist,ap,'\n');
      std::string fpar="\n\n"+get_param(fparlist,fp,',')+"\n";

      std::string::size_type x;
      while((x=replaced.find(fpar))!=std::string::npos)
       replaced=replaced.substr(0,x)+apar+replaced.substr(x+fpar.length());
     }

    if(replaced.find('\n')!=std::string::npos)
     UNSUCCESSFUL_PREPROCESS;
   }

  //elvileg replaced ezen a ponton nem tartalmazhat enter karaktert:
  return replaced;
 }

void buffer(const char *buf)
 {
  push_current_buffer();
  yy_scan_string(buf);
 }

void handle_id(/*const char *x,*/int err_id_state)
 {
  err_id=err_id_state;
  previous=YY_START;
  if(!defined(yytext))
    { //http://flex.sourceforge.net/manual/Actions.html -- yyleng
     yyless(0);
     BEGIN(err_id_state);
    }
   else if(has_params(yytext))
    {
     BEGIN(aplist);
     define_id=yytext;
    }
   else
    {
     buffer(preprocess_id(yytext).c_str());
    }
 }

void skip_then_branch()
 {
  ifstack.push(false);
  iffdepth=1;
  BEGIN(fbranch);
 }

void take_then_branch(int nexts)
 {
  ifstack.push(false);
  //Elvileg a köv. sorokat fel kell dolgoznunk:
  BEGIN(nexts);
 }

void take_then_branch()
 {
  take_then_branch(eat_nl);
 }

void nextline()
 {
  ++curline;
 }

void report_file()
 {
  if(!preprocess_only) return;
  if(!yyout) return;//XXX fixme hack... :P
  fprintf(yyout,"\n#line %d \"",curline);
  std::string::size_type p=0;
  std::string::size_type p1;
  while((p1=curfile.find('"',p))!=std::string::npos)
   {
    fprintf(yyout,"%s\\\"",curfile.substr(p,p1-p-1).c_str());
    p=p1+1;
   }
  fprintf(yyout,"%s\"\n",curfile.substr(p).c_str());
 }

bool finish_include()
 {
  bool l=true;
  if(parsing==p_include)
    {
     if(preprocess_only) fprintf(yyout,"\n");
     NEXTLINE;
     l=include(strconst);
    }
   else
    {
     curfile=strconst;
     report_file();
    }
  BEGIN(MYINITIAL);
  strconst="";
  return l;
 }

void set_preprocess_only(bool x)
 {
  preprocess_only=x;
  MYINITIAL=x?preprocessing:compiling;
  BEGIN(MYINITIAL);
 }

bool other_include(char *line)
 {
  while(*(line++)!='<') ;

  char *tmp=line+strlen(line);

  while(*tmp!='>')
   --tmp;

  *tmp='\0';

  return include(line,1);
 }

void set_real_file(FILE *x,const string &y)
 {
  real_file=x;
  real_curfile=y;
 }

void undefine()
 {
  char *x=yytext;
  while(*(x++)!='u') ;
  x+=4;
  while((*x=='\t')||(*x==' ')) ++x;
  char *y=yytext+yyleng-1;
  while((*y=='\t')||(*y==' ')) --y;
  y[1]='\0';
  define_undefine(x);
 }

#include "comment_stripper_foot.h"

#endif
