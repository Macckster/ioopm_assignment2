#include "common.h"

int read_input_string(char* buf, int buf_siz);
bool is_number(char* str);

int ask_question_int(char* question);
double ask_question_float(char* question);
char* ask_question_string(char* question);
char* ask_question_shelf(char* question);
bool ask_question_bool(char* question);