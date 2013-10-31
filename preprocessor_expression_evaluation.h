#ifndef __PREPROCESSOR_EXPRESSION_EVALUATION_H
#define __PREPROCESSOR_EXPRESSION_EVALUATION_H

#include <string>

namespace prep_eval
 {

  bool is_valid();

  int result();

  void receive_int(const int);
  void receive_op(const std::string &);
  void reset();
 }

#endif
