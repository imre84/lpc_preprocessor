#ifndef __PREPROC_DEFINE
#define __PREPROC_DEFINE

#include <string>

struct define_r
 {
  std::string paramlist,content;
  define_r(const std::string &paramlist,const std::string &content):paramlist(paramlist),content(content){}
 };

extern std::string strconst,define_id,define_parlist;
void define_register_my_id(const char *id);
void define_register_parameter(const char *id);
void define_register_content_junk(const char *junk);
void define_register_content_id(const char *id);
void define_register_me();
void define_invalidate();
void define_undefine(const std::string &id);

bool defined(const char *id);
//const std::string &get_content(const char *id);
const define_r *get_entry(const char *id);
bool has_params(const char *id);

void init_define_table();
std::string preproc_routine(const char *id,const std::string &paramlist,bool &ok);

#endif
