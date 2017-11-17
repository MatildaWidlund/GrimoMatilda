#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

bool is_char(char* c) {
  return (strlen(c) == 1 && isalpha(*c));
}

bool is_shelf(char* str) {
  if (str == NULL || *str == '\0') return false;
  char c = *str;

  if (!isalpha(c)) return false;

  c = *(++str);

  if (!isdigit(c)) return false;
  
  c = *(++str);

  while (c != '\0') {
    if (!isdigit(c)) return false;
    c = *(++str);
  }

  return true;
}

bool not_empty(char* str) {
  return (strlen(str) > 0);
}

bool is_number(char* str) {
  int len = strlen(str);
  
  if (len <= 0) return false;

  for (int i = 0; i < len - 1; ++i) {
    if (!isdigit(str[i]))
      return false;
  }

  return true;
}

char ask_question_char(char* question) {
  answer_t a = ask_question(question, is_char, (convert_func) strdup);
  char c = *(a.s);

  return c;
}

char* ask_question_string(char* question) {
  return ask_question(question, not_empty, (convert_func) strdup).s;
}

char* ask_question_shelf(char* question) {
  return ask_question(question, is_shelf, (convert_func) strdup).s;
}

int ask_question_int(char* question) {
  return ask_question(question, is_number, (convert_func) atoi).i;
}

void clear_input_buffer() {
  char c;

  do {
    c = getchar();
  } while (c != '\n');

  return;
}

int read_string(char* buf, int buf_siz) {
  int limit = buf_siz - 1;
  int counter = 0;

  while (counter < limit) {
    int c = getchar();

    if (c == '\n' || c == EOF) {
      buf[counter] = '\0';
      return counter;
    } else {
      buf[counter++] = c;
    }
  }

  clear_input_buffer();

  buf[limit] = '\0';
  return limit;
}

answer_t ask_question(char* question, check_func check, convert_func convert) {
  int buf_siz = 256;
  char buf[buf_siz];
  bool correct = false;

  do {
    printf("%s", question);
    read_string(buf, buf_siz);
    correct = check(buf);
  } while (!correct);

  return convert(buf);
}
