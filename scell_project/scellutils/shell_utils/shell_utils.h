#ifndef SHU_SHELL_UTILS
#define SHU_SHELL_UTILS

/* IMPORTS */

#define _GNU_SOURCE 
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../constructs/boolean.h"
#include "../constructs/sh_state.h"
#include "../logger/logger.h"

/* FUNCTIONS */

void shu_unknown_command(char *command);
void shu_unknown_argument(char *command, char* argument, bool write);
void shu_value_without_argument(char* value, bool write);
void shu_help_misplaced_argument(char* value);
void shu_help_unknown_command(char* command);

bool shu_check_noval(const char *command, const char *argument, int size, int *i, bool write);
bool shu_check_noval_array(const char *command, const char *argument, bool is_assigned, bool write);
bool shu_check_required(const char* command, const char* argument, bool is_assigned, bool write);
bool shu_check_min_int(const char *command, const char *argument, int value, int min, bool write);
bool shu_check_max_int(const char *command, const char *argument, int value, int max, bool write);
bool shu_check_min_long(const char *command, const char *argument, long value, long min, bool write);
bool shu_check_max_long(const char *command, const char *argument, long value, long max, bool write);
bool shu_check_min_float(const char *command, const char *argument, float value, float min, bool write);
bool shu_check_max_float(const char *command, const char *argument, float value, float max, bool write);
bool shu_check_min_double(const char *command, const char *argument, double value, double min, bool write);
bool shu_check_max_double(const char *command, const char *argument, double value, double max, bool write);

bool shu_get_int_value(const char *command, const char *argument, char* str_value, int* value, bool write);
bool shu_get_long_value(const char *command, const char *argument, char* str_value, long* value, bool write);
bool shu_get_float_value(const char *command, const char *argument, char* str_value, float* value, bool write);
bool shu_get_double_value(const char *command, const char *argument, char* str_value, double* value, bool write);
bool shu_get_char_value(const char *command, const char *argument, char* str_value, char* value, bool write);

char* shu_extract_argument(char *word, bool* is_alias);

#endif