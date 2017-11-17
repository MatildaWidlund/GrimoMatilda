#ifndef __utils_h__
#define __utils_h__

#include <stdbool.h>

extern char* strdup(const char*);

typedef union {
  char* s;
  int i;
} answer_t;

typedef answer_t(*convert_func)(char*);
typedef bool(*check_func)(char*);

bool is_char(char*);
bool is_shelf(char*);
bool not_empty(char*);
char ask_question_char(char*);
char* ask_question_string(char*);
char* ask_question_shelf(char*);
int ask_question_int(char*);
void clear_input_buffer();
int read_string(char*, int);
answer_t ask_question(char*, check_func, convert_func);

#endif
