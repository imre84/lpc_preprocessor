#include "preproc_define.h"
#include <map>

using namespace std;

typedef std::map<std::string,define_r> dtmap;

dtmap define_table;

void define_register_my_id(const char *id)
 {
  define_id=id;
 }

void define_register_content_junk(const char *junk)
 {
  strconst+=junk;
 }

void define_register_parameter(const char *id)
 {
  define_parlist=define_parlist+id+',';
 }

void define_register_me()
 {
  if(define_id!="")
    {
     define_table.insert(make_pair(define_id,define_r(define_parlist,strconst)));
     define_id="";
    }

  define_parlist="";
  strconst="";
 }

void define_invalidate()
 {
  define_id="";
 }

void define_register_content_id(const char *id)
 {
  int x=define_parlist.find(string(id)+',');
  if(x&&(define_parlist[x-1]!=','))
   {
    define_register_content_junk(id);
    return;
   }

  strconst=strconst+"\n\n"+id+"\n";
 }

const define_r *get_entry(const char *id)
 {
  const dtmap &x=define_table;
  const dtmap::const_iterator &y=x.find(id);

  if(y==x.end()) return 0;

  return &y->second;
 }

bool defined(const char *id)
 {
  return get_entry(id);
 }

const string &get_content(const char *id)
 {
  return get_entry(id)->content;
 }

bool has_params(const char *id)
 {
  return get_entry(id)->paramlist!="";
 }

void init_define_table()
 {
  define_table.clear();
  //XXX fixme __DIR__ __FILE__ __LINE__ :
  define_table.insert(make_pair("__DIR__",define_r(""," \"/whatever/\"")));
  define_table.insert(make_pair("__SAVE_EXTENSION__",define_r(""," \".o\"")));

  //this one is the internal defined() routine in the table:
  define_table.insert(make_pair("defined",define_r("x,","\n\n\n0")));
 }

string preproc_routine(const char *id,const string &pl,bool &ok)
 {
  switch(*id)
   {
    case '0':
     ok=(pl.find('\n')==(pl.length()-1));
     return defined(pl.substr(0,pl.length()-1).c_str())?"1":"0";
    default:
     ok=false;
     return "";
   }
 }

void define_undefine(const string &id)
 {
  define_table.erase(id);
 }
