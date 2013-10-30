#ifdef __COMMENT_STRIPPER_H
#ifndef __COMMENT_STRIPPER_FOOT_H
#define __COMMENT_STRIPPER_FOOT_H

string end_id;//lehetne karaktertömb
//int debugger;//XXX fixme debug

#define MY_IDCHAR(x) ((x>='0' && x<='9') || \
                      (x>='A' && x<='Z') || \
                      (x>='a' && x<='z') || \
                      (x=='_'))

#define MY_START           0
#define MY_SLASH           1
#define MY_AT              2
#define MY_STR             3
#define MY_EAT_LINE        4
#define MY_COMMENT_ML      5
#define MY_COMMENT_ML_STAR 6
#define MY_AT_NL           7
#define MY_AT_INSIDE       8
#define MY_ESCAPED_CHAR    9

struct state_t
 {
  unsigned int state,pos;
  string end_id;
 };

using namespace std;

void sub_my_yy_input(char *buf,int &result,const size_t max_size)
 {
  errno=0;
  while ( (result = fread(buf, 1, max_size, yyin))==0 && ferror(yyin))
   {
    if( errno != EINTR)
     {
      YY_FATAL_ERROR( "input in flex scanner failed" );
      break;
     }

     errno=0;
     clearerr(yyin);
   }
 }

void stripper(char *const buf,int &i,int &result,state_t &state)
 {
  int eat_start=i;

  while(i<result)
   {
    switch(state.state)
     {
      case MY_START:
       if(buf[i]=='/')
        {state.state=MY_SLASH;eat_start=i;}
       else if(buf[i]=='@')
        state.state=MY_AT;
       else if(buf[i]=='"')
        state.state=MY_STR;
       break;
      case MY_SLASH:
       if(buf[i]=='/')
        state.state=MY_EAT_LINE;
       else if(buf[i]=='*')
        state.state=MY_COMMENT_ML;
       else
        state.state=MY_START;
       break;
      case MY_AT:
       if(MY_IDCHAR(buf[i]))
        end_id+=buf[i];
       else if(!end_id.length())
        state.state=MY_START;
       else if(buf[i]=='\n')
        state.state=MY_AT_NL;
       else
        state.state=MY_START;
       break;
      case MY_STR:
       if(buf[i]=='\\')
         state.state=MY_ESCAPED_CHAR;
       else if(buf[i]=='"')
        state.state=MY_START;
       break;
      case MY_ESCAPED_CHAR:
       state.state=MY_STR;
       break;
      case MY_EAT_LINE:
       if(buf[i]=='\n')
        {
         if(eat_start!=i)
          memmove(buf+eat_start,buf+i,result-i);
         result-=(i-eat_start);
         i=eat_start;
         state.state=MY_START;
        }
       break;
      case MY_COMMENT_ML:
       if(buf[i]=='*')
        state.state=MY_COMMENT_ML_STAR;
       else if(buf[i]=='\n')
        buf[eat_start++]='\n';
       break;
      case MY_COMMENT_ML_STAR:
       if(buf[i]=='/')
        {
         memmove(buf+eat_start,buf+i+1,result-i-1);
         result-=(i-eat_start+1);
         i=eat_start-1;
         state.state=MY_START;
        }
       else if(buf[i]=='\n')
        {
         buf[eat_start++]='\n';
         state.state=MY_COMMENT_ML;
        }
       else if(buf[i]!='*')
        state.state=MY_COMMENT_ML;
       break;
      case MY_AT_NL:
       if((!MY_IDCHAR(buf[i]))&&(state.pos==end_id.length()))
        {
         state.pos=0;
         state.state=MY_START;
         --i;
        }
       else if(buf[i]=='\n')
        state.pos=0;
       else if((!MY_IDCHAR(buf[i]))||
               (state.pos>=end_id.length())||
               (end_id[state.pos]!=buf[i]))
        {
         state.pos=0;
         state.state=MY_AT_INSIDE;
        }
       else
        state.pos++;
       break;
      case MY_AT_INSIDE:
       if(buf[i]=='\n')
        state.state=MY_AT_NL;
       break;
     }

    ++i;
   }

  if(state.state==MY_EAT_LINE||state.state==MY_COMMENT_ML)
   i=result=eat_start;
 }

#include <map>

map<int,state_t> file_states;

void retrieve_state(state_t &state)
 {
  map<int,state_t>::const_iterator it=file_states.find(reinterpret_cast<int>(yyin));
  if(it!=file_states.end())
    state=it->second;
   else
    {
     state.state=MY_START;
     state.pos=0;
     state.end_id="";
    }
 }

void remove_state()
 {
  map<int,state_t>::iterator it=file_states.find(reinterpret_cast<int>(yyin));
  if(it!=file_states.end())
   file_states.erase(it);
 }

void store_state(const state_t &state)
 {
  if((state.state==MY_START)||(feof(yyin)))
   remove_state();
  else
   file_states[reinterpret_cast<int>(yyin)]=state;
 }

void my_yy_input(char * const buf,int &result,const size_t max_size)
 {
  //XXX fixme unsigned?
  int i=0,old_result;
  state_t state;
  result=0;

  retrieve_state(state);

  if((!real_file))
   real_file=0;

  do
   {
    if(state.state==MY_SLASH)
     {
      buf[0]='/';
      sub_my_yy_input(buf+1,result,max_size-1);
      ++result;
      state.state=MY_START;
     }
    else
     {
      sub_my_yy_input(buf+i,result,max_size-i);
      result+=i;
     }

    if(errno) return;

    old_result=result;

    stripper(buf,i,result,state);

   }
  while(result!=old_result);

  if(state.state==MY_SLASH) --result;

  store_state(state);
 }

/*void my_yy_input_init()
 {
 }*/

#endif
#endif
