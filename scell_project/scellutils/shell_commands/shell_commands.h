#ifndef SH_SHELL_COMMANDS
#define SH_SHELL_COMMANDS

/* IMPORTS */

#include <string.h>
#include "../constructs/boolean.h"
#include "../constructs/sh_state.h"
#include "../text/text.h"

/* EXTERNS */

extern void shu_unknown_command(char *command);
extern void shu_value_without_argument(char* value, bool write);
extern void shu_unknown_argument(char *command, char* argument, bool write);
extern void shu_help_misplaced_argument(char* value);
extern void shu_help_unknown_command(char* command);
extern bool shu_check_noval(const char *command, const char *argument, int size, int *i, bool write);
extern bool shu_check_noval_array(const char *command, const char *argument, bool is_assigned, bool write);
extern bool shu_check_required(const char* command, const char* argument, bool is_assigned, bool write);
extern bool shu_check_min_int(const char *command, const char *argument, int value, int min, bool write);
extern bool shu_check_max_int(const char *command, const char *argument, int value, int max, bool write);
extern bool shu_check_min_long(const char *command, const char *argument, long value, long min, bool write);
extern bool shu_check_max_long(const char *command, const char *argument, long value, long max, bool write);
extern bool shu_check_min_float(const char *command, const char *argument, float value, float min, bool write);
extern bool shu_check_max_float(const char *command, const char *argument, float value, float max, bool write);
extern bool shu_check_min_double(const char *command, const char *argument, double value, double min, bool write);
extern bool shu_check_max_double(const char *command, const char *argument, double value, double max, bool write);
extern bool shu_get_int_value(const char* command, const char* argument, char* str_value, int* value, bool write);
extern bool shu_get_long_value(const char* command, const char* argument, char* str_value, long* value, bool write);
extern bool shu_get_float_value(const char* command, const char* argument, char* str_value, float* value, bool write);
extern bool shu_get_double_value(const char* command, const char* argument, char* str_value, double* value, bool write);
extern bool shu_get_char_value(const char* command, const char* argument, char* str_value, char* value, bool write);
extern char* shu_extract_argument(char *word, bool* is_alias);

extern SH_STATE arguments(int a);
extern SH_STATE init(int first, int second);
extern SH_STATE sub(int first, int second);
extern SH_STATE quit();

/* STRUCTS */

typedef struct {
    char* name;
    char* alias;
    char* description;
    char* type;
    char* default_value;
} sh_param_details;

/* FUNCTIONS */

void sh_help(char **words, int size);
void sh_help__arguments();
void sh_help_add();
void sh_help_sub();
void sh_help_quit();
SH_STATE sh_handle__arguments(char** words, int n_words);
SH_STATE sh_handle_add(char** words, int n_words);
SH_STATE sh_handle_sub(char** words, int n_words);
SH_STATE sh_handle_quit(char** words, int n_words);
SH_STATE sh_parse_command(char** words, int n_words);
void sh_loop();

/* VARIABLES */

extern char *sh_last_command;
extern char *sh_prompt_symbol;
extern bool sh_use_advanced_terminal;

#endif