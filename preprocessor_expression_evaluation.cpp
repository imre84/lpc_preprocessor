#include "preprocessor_expression_evaluation.h"
#include <map>
#include <stack>

#warning debug:
#include <iostream>

using namespace std;
using namespace prep_eval;
bool failed;

#include "preproc_tables.h"

const t_prec_map prec_map;
const t_op2op op2op;

class ptree;

class stack_element
 {
   union
    {
     ptree *op;//operator
     int x;//operand
    };
   bool is_operator;
   stack_element(const stack_element &x){assign(x);}

  public:
   stack_element(const opers op);
   stack_element(const int x):x(x),is_operator(false){}
   //stack_element(const stack_element &x){assign(x);}
   stack_element():op(0),is_operator(true){}
   void cleanup();
   ~stack_element() {cleanup();}
   void assign(const stack_element &);
   stack_element &operator=(const stack_element &);
   bool ok_as_operand() const;
   bool ok_as_operator() const;
   int value() const;
   opers op_symbol() const;
   stack_element *add_subtrees(stack_element *a,stack_element *b=0,stack_element *c=0);
 };

class ptree
 {
  stack_element *a, *b, *c;
  ptree(const ptree &x):op_symbol(x.op_symbol){};//XXX fixme implementme (?)
  public:
   const opers op_symbol;
   ptree(opers op):a(0),b(0),c(0),op_symbol(op){}
   ~ptree(){if(a) delete a;if(b) delete b;if(c) delete c;}
   void add_subtrees(stack_element *a,stack_element *b,stack_element *c) {this->a=a;this->b=b;this->c=c;}
   friend bool stack_element::ok_as_operand() const;
   int value() const;
 };

int to_bool(const int x)
 {
  return x?1:0;
 }

int to_bool(const stack_element &x)
 {
  return to_bool(x.value());
 }

int to_int(const stack_element &x)
 {
  return x.value();
 }

namespace prep_eval
 {

void error(const void *)
 {
  #warning connect to preprocessor.lpp::yyerror
  failed=true;
 }

 }

int ptree::value() const
 {
  switch(op_symbol)
   {
    case op_qry:
     return to_bool(*a)?to_int(*b):to_int(*c);
    case op_lor:
     {
      int ba=to_bool(*a);
      if(ba) return ba;
      return to_bool(*b);
     }
    case op_land:
     {
      int ba=to_bool(*a);
      if(!ba) return ba;
      return to_bool(*b);
     }
    case op_eq:
     return to_int(*a)==to_int(*b);
    case op_neq:
     return to_int(*a)!=to_int(*b);
    case op_lt:
     return to_int(*a)<to_int(*b);
    case op_le:
     return to_int(*a)<=to_int(*b);
    case op_gt:
     return to_int(*a)>to_int(*b);
    case op_ge:
     return to_int(*a)>=to_int(*b);
    case op_bor:
     return to_int(*a)|to_int(*b);
    case op_bxor:
     return to_int(*a)^to_int(*b);
    case op_band:
     return to_int(*a)&to_int(*b);
    case op_shr:
     return to_int(*a)>>to_int(*b);
    case op_shl:
     return to_int(*a)<<to_int(*b);
    case op_add:
     return to_int(*a)+to_int(*b);
    case op_sub:
     return to_int(*a)-to_int(*b);
    case op_mul:
     return to_int(*a)*to_int(*b);
    case op_div:
     return to_int(*a)/to_int(*b);
    case op_mod:
     return to_int(*a)%to_int(*b);
    case op_lneg:
     return 1-to_bool(*a);
    case op_bneg:
     return ~to_bool(*a);
    case op_ineg:
     return -to_bool(*a);
    default:
     prep_eval::error("you seem to have some error in your expression, also, internal error.");
   };

  return 0;
 }

stack_element::stack_element(const opers op):op(new ptree(op)),is_operator(true)
 {
 }

void stack_element::cleanup()
 {
  if(is_operator&&op)
   delete op;
 }

/*void stack_element::assign(const stack_element &w)
 {
  is_operator=w.is_operator;
  if(is_operator&&w.op)
    {
     op=new ptree(*(w.op));
    }
   else if(is_operator)
    op=0;
   else
    x=w.x;
 }*/

/*stack_element &stack_element::operator=(const stack_element &w)
 {
  cleanup();
  assign(w);
  return *this;
 }*/

bool stack_element::ok_as_operand() const
 {
  return (!is_operator)||(op->a);
 }

bool stack_element::ok_as_operator() const
 {
  return !ok_as_operand();
 }

int stack_element::value() const
 {
  if(!ok_as_operand()) return 0;

  if(!is_operator) return x;

  return op->value();
 }

opers stack_element::op_symbol() const
 {
  if(!is_operator) return op_invalid;
  return op->op_symbol;
 }

stack_element *stack_element::add_subtrees(stack_element *a,stack_element *b,stack_element *c)
 {
  op->add_subtrees(a,b,c);
  return this;
 }

stack<stack_element *> st;
bool was_op;//Az előző lexikális szimbólum operátor volt.
opers last_op;//A verem tetejéhez legközelebbi operátorszimbólum

void prep_eval::reset()
 {
  while(!st.empty())
   {
    delete(st.top());
    st.pop();
   }

  last_op=op_eol;
  was_op=true;
  failed=false;
  st.push(new stack_element(op_eol));
 }

void prep_eval::receive_int(const int x)
 {
  was_op=false;
  st.push(new stack_element(x));
 }

const prec &find_entry(const opers op)
 {
  return prec_map.find(op)->second;
 }

int f(const opers op)
 {
  return find_entry(op).f;
 }

int g(const opers op)
 {
  return find_entry(op).g;
 }

void delete_vec(stack_element **x,int xlen)
 {
  do
   {
    delete(*x);
    x++;
   }
  while(--xlen);
 }

void adjust_stack()
 {
  //Egy prím részmondat jobb szélén vagyunk.
  stack_element **x, **y;
  y=new stack_element *[5];
  opers my_op=op_invalid;
  x=y+5;

  while((x!=y)&&
        (!st.empty())&&
        ((st.top()->ok_as_operand())||
         (my_op==op_invalid)||
         (f(st.top()->op_symbol())==g(my_op))))
   {
    if(st.top()->ok_as_operator())
     my_op=st.top()->op_symbol();

    --x;
    *x=st.top();
    st.pop();
   }

  if(st.empty())
   st.push(new stack_element(op_eol));

  last_op=st.top()->op_symbol();

  const unsigned int xlen=5-(x-y);

  if(x[0]->ok_as_operator())
   if((x[0]->op_symbol()==op_par6)&&(xlen==3)&&(x[2]->op_symbol()==op_par9)&&(!x[1]->ok_as_operator()))
    {
     st.push(x[1]);
     delete(x[0]);
     delete(x[2]);
    }
   else if(((x[0]->op_symbol()==op_lneg)||(x[0]->op_symbol()==op_bneg)||(x[0]->op_symbol()==op_ineg))&&(xlen==2)&&(x[1]->ok_as_operand()))
    st.push(x[0]->add_subtrees(x[1]));
   else
    {
     delete_vec(x,xlen);
     error("you seem to take some mistakes in the expression.");
    }
  else
   {
    if((xlen==5)&&(x[1]->ok_as_operator())&&(x[1]->op_symbol()==op_qry)&&(x[3]->ok_as_operator())&&(x[3]->op_symbol()==op_col)&&(x[0]->ok_as_operand())&&(x[2]->ok_as_operand())&&(x[4]->ok_as_operand()))
     {
      st.push(x[1]->add_subtrees(x[0],x[2],x[4]));
      delete x[3];
     }
    else if(xlen==3&&(x[0]->ok_as_operand())&&(x[1]->ok_as_operator())&&(x[2]->ok_as_operand()))
     {
      const opers this_op=x[1]->op_symbol();
      if((this_op==op_lor)||(this_op==op_land)||(this_op==op_eq)||
         (this_op==op_neq)||(this_op==op_lt)||(this_op==op_gt)||
         (this_op==op_ge)||(this_op==op_le)||(this_op==op_bor)||
         (this_op==op_bxor)||(this_op==op_band)||(this_op==op_shr)||
         (this_op==op_shl)||(this_op==op_add)||(this_op==op_sub)||
         (this_op==op_mul)||(this_op==op_div)||(this_op==op_mod))
        st.push(x[1]->add_subtrees(x[0],x[2]));
       else
        {
         delete_vec(x,xlen);
         error("you seem to take some mistakes in the expression.");
        }
     }
    else
     {
      delete_vec(x,xlen);
      error("you seem to take some mistakes in the expression.");
     }
   }

  delete[] y;
 }

bool is_operator(const string &op)
 {
  return (op2op.find(op)!=op2op.end())&&(op!="#");
 }

void sub_recv_op(const opers op)
 {
  was_op=true;

  while(f(last_op)>g(op))
   adjust_stack();

  last_op=op;
  st.push(new stack_element(op));
 }

opers op_str_to_op_symbol(const string &op)
 {
  const t_op2op::const_iterator x=op2op.find(op);
  if(x==op2op.end())
    return op_invalid;
   else
    return x->second;
 }

void prep_eval::receive_op(const string &op)
 {
  if(!is_operator(op))
   {
    error(("internal error: no such operator: "+op).c_str());
    sub_recv_op(op_add);
   }
  else if(was_op&&(op=="-"))
   sub_recv_op(op_ineg);
  else
   sub_recv_op(op_str_to_op_symbol(op));
 }

int prep_eval::result()
 {
  if(!is_valid()) return 0;
  return to_int(*st.top());
 }

bool prep_eval::is_valid()
 {
  if(failed) return false;
  sub_recv_op(op_eol);
  st.pop();
  return
   (!failed)&&
   (st.size()==2)&&
   (st.top()->ok_as_operand());
 }
