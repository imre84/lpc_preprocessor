#ifndef __COMMENT_STRIPPER_H
#define __COMMENT_STRIPPER_H

void my_yy_input(char * const buf,int &result,const size_t max_size);
void my_yy_input_init();
#define YY_INPUT(buf,result,max_size) my_yy_input(buf,result,max_size)
#define MY_YY_INPUT_INIT my_yy_input_init()

#endif
