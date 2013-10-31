#include "preproc_tables.h"

using namespace std;

t_op2op::t_op2op():map<string,opers>()
 {
  insert(make_pair("#" ,op_eol ));
  insert(make_pair("(" ,op_par6));
  insert(make_pair("?" ,op_qry ));
  insert(make_pair(":" ,op_col ));
  insert(make_pair("||",op_lor ));
  insert(make_pair("&&",op_land));
  insert(make_pair("==",op_eq  ));
  insert(make_pair("!=",op_neq ));
  insert(make_pair("<" ,op_lt  ));
  insert(make_pair("<=",op_le  ));
  insert(make_pair(">" ,op_gt  ));
  insert(make_pair(">=",op_ge  ));
  insert(make_pair("|" ,op_bor ));
  insert(make_pair("^" ,op_bxor));
  insert(make_pair("&" ,op_band));
  insert(make_pair(">>",op_shr ));
  insert(make_pair("<<",op_shl ));
  insert(make_pair("+" ,op_add ));
  insert(make_pair("-" ,op_sub ));
  insert(make_pair("*" ,op_mul ));
  insert(make_pair("/" ,op_div ));
  insert(make_pair("%" ,op_mod ));
  insert(make_pair("!" ,op_lneg));
  insert(make_pair("~" ,op_bneg));
  //insert(make_pair(... ,op_ineg));
  insert(make_pair(")" ,op_par9));
 }


t_prec_map::t_prec_map():map<opers,prec>()
 {
  insert(make_pair(op_eol ,prec( 0, 0))); // #
  insert(make_pair(op_par6,prec( 0,20))); // (
  insert(make_pair(op_qry ,prec( 0, 2))); // ?
  insert(make_pair(op_col ,prec( 1, 0))); // :
  insert(make_pair(op_lor ,prec( 3, 2))); // ||
  insert(make_pair(op_land,prec( 5, 4))); // &&
  insert(make_pair(op_eq  ,prec( 7, 6))); // ==
  insert(make_pair(op_neq ,prec( 7, 6))); // !=
  insert(make_pair(op_lt  ,prec( 7, 8))); // <
  insert(make_pair(op_le  ,prec( 7, 8))); // <=
  insert(make_pair(op_gt  ,prec( 7, 8))); // >
  insert(make_pair(op_ge  ,prec( 7, 8))); // >=
  insert(make_pair(op_bor ,prec( 9, 8))); // |
  insert(make_pair(op_bxor,prec(11,10))); // ^
  insert(make_pair(op_band,prec(13,12))); // &
  insert(make_pair(op_shr ,prec(15,14))); // >>
  insert(make_pair(op_shl ,prec(15,14))); // <<
  insert(make_pair(op_add ,prec(17,16))); // +
  insert(make_pair(op_sub ,prec(17,16))); // -
  insert(make_pair(op_mul ,prec(19,18))); // *
  insert(make_pair(op_div ,prec(19,18))); // /
  insert(make_pair(op_mod ,prec(19,18))); // %
  insert(make_pair(op_lneg,prec(19,20))); // !
  insert(make_pair(op_bneg,prec(19,20))); // ~
  insert(make_pair(op_ineg,prec(19,20))); // N
  insert(make_pair(op_par9,prec(21, 0))); // )
 }
