#include "utils.h"

typedef union 
{
    int   int_value;
    float float_value;
    char* string_value;
    bool bool_value;
} answer_t;

typedef bool(*check_func)(char*);

typedef answer_t(*convert_func)(char*);

void clear_input_buffer()
{
    int c;
    do
    {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

int read_input_string(char* buf, int buf_siz)
{
    int result = -1;
    int c;

    while(true)
    {
        result++;
        c = getchar();

        if (c != '\n' && c != EOF)
            buf[result] = c;
        else
            break;

        if (result >= buf_siz - 1)
        {
            buf[buf_siz - 1] = '\0';
            clear_input_buffer();
            return result;
        }
    }

    buf[result + 1] = '\0';

    return result;
}

bool not_empty(char* str)
{
    return strlen(str) > 0;
}

bool is_number(char* str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
        {
            if (!(i == 0 && str[i] == '-'))
                return false;
        }
    }
    return true;
}

answer_t make_float(char* str)
{
    return (answer_t) { .float_value = atof(str) };
}

extern char* strdup(const char*);

bool is_float(char* str)
{
    if (strlen(str) < 1)
        return false;
    bool hasDecimal = false;
    for (int i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
        {
            if (i == 0 && str[i] == '-')
                continue;
            if (str[i] == '.' && !hasDecimal)
            {
                hasDecimal = true;
                continue;
            }

            return false;
        }
    }
    return true;
}

answer_t ask_question(char* question, check_func check, convert_func convert)
{
    char buf[255];

    do
    {
        memset(buf, 0, 255);
        //print(question);
        read_input_string(buf, 255);
    } while (!check(buf));

    return convert(buf);
}

bool valid_shelf(char* string)
{
    size_t size = strlen(string);
 
    if(size != 3)
        return false;
    
    if(!isalpha(string[0]))
        return false;
        
    for (int i = 1; i < size; i++) {
        if(!isdigit(string[i]))
            return false;
    }
    
    return true;
}

bool is_bool(char* string)
{
    return string[0] == 'y' || string[0] == 'n';
}

answer_t make_bool(char* string)
{
    return (answer_t) {.bool_value = string[0] == 'y'};
}

double ask_question_float(char* question)
{
    return ask_question(question, is_float, make_float).float_value;
}

int ask_question_int(char* question)
{
    return ask_question(question, is_number, (convert_func)atoi).int_value; 
}

char* ask_question_string(char* question)
{
    return ask_question(question, not_empty, (convert_func)strdup).string_value; // Remember to free that shit
}

char* ask_question_shelf(char* question)
{
    return ask_question(question, valid_shelf, (convert_func)strdup).string_value; // Remember to also free that shit
}

bool ask_question_bool(char* question)
{
    return ask_question(question, is_bool, make_bool).bool_value;
}