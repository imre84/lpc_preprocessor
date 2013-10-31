#ifndef __PREPROC_TABLES
#define __PREPROC_TABLES

#include <string>
#include <map>

enum opers
 {
  op_eol,     //end-of-line, begin of line     #
  op_par6,    //open parentheses               (
  op_qry,     //query simbol                   ?
  op_col,     //colon simbol                   :
  op_lor,     //logical or                     ||
  op_land,    //logical and                    &&
  op_eq,      //equivalent simbol              ==
  op_neq,     //not equivalent simbol          !=
  op_lt,      //less than                      <
  op_le,      //less than or equal             <=
  op_gt,      //greater than                   >
  op_ge,      //greather than or equal         >=
  op_bor,     //bitwise or                     |
  op_bxor,    //bitwise xor                    ^
  op_band,    //bitwise and                    &
  op_shr,     //shift right                    >>
  op_shl,     //shift left                     <<
  op_add,     //addition                       +
  op_sub,     //subtract                       -
  op_mul,     //multiply                       *
  op_div,     //divide                         /
  op_mod,     //modulus                        %
  op_lneg,    //logical negation               !
  op_bneg,    //bitwise negation               ~
  op_ineg,    //integer negation               -
  op_par9,    //close parentheses              )
  num_ops     //number of operators
 };

//A synonym for confort:
const opers op_invalid=num_ops;

struct prec
 {
  const int f,g;
  prec(int f,int g):f(f),g(g){}
 };

class t_op2op:public std::map<std::string,opers>
 {
  public:
   t_op2op();
 };

class t_prec_map:public std::map<opers,prec>
 {
  public:
   t_prec_map();
 };

#endif
