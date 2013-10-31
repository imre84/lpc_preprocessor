#include "mudconfig.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

map<string,string> cfg;

void eat_config(const char *fn)
 {
  ifstream f(fn);
  if(f.fail())
   {
    cerr<<"The specified config file does not exist";
    exit(1);
   }

  string s;
  int line=0;

  while(++line,getline(f,s))
   if(s.length()&&(s[0]!='#'))
    {
     string a,b;
     string::size_type x,y;
     y=s.find_first_of(':');
     x=s.find_last_not_of(" \t",y-1);
     if(x==string::npos)
      {
       cerr<<"malformed line: "<<line<<endl;
       continue;
      }

     a=s.substr(0,x+1);
     b=s.substr(s.find_first_not_of(" \t",y+1));
     y=b.find_last_not_of(" \t");
     if(y==string::npos)
       b="";
      else
       b=b.substr(0,y+1);

     cfg[a]=b;
    }
 }

string getval(const string &var)
 {
  return cfg[var];
 }

bool check_config()
 {
  string s=getval("global include file");
  char last=s[s.length()-1];
  if(((last!='>')&&(last!='"'))||
     ((s[0]!='<')&&(s[0]!='"'))||
     ((last=='>')&&(s[0]!='<'))||
     ((last=='"')&&(s[0]!='"')))
      {
       cerr<<"Seems like you have an error in the global include file "
           <<"config directive.";
       return false;
      }

  return true;
 }
