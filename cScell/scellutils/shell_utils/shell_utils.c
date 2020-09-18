#include "shell_utils.h"

/* EXPORTED FUNCTIONS */

void shu_unknown_command(char* command) {
    char *message;
    asprintf(&message, "unknown command %s", command);
    log_error_del_ctx(message, strdup("SHELL"));
}

void shu_unknown_argument(char *command, char* argument, bool write) {
    if (write) {
        char *message;
        asprintf(&message, "command %s: unknown argument %s", command, argument);
        log_warning_del_ctx(message, strdup("SHELL"));
    }
}

void shu_value_without_argument(char* value, bool write) {
    if (write) {
        char *message;
        asprintf(&message, "value without argument, %s ignored", value);
        log_warning_del_ctx(message, strdup("SHELL"));
    }
}

void shu_help_misplaced_argument(char* value) {
    char *message;
    asprintf(&message, "help: misplaced argument, %s ignored", value);
    log_warning_del_ctx(message, strdup("SHELL"));
}

void shu_help_unknown_command(char* command) {
    char *message;
    asprintf(&message, "help: unknown argument %s", command);
    log_error_del_ctx(message, strdup("SHELL"));
}

bool shu_check_noval(const char *command, const char *argument, int size, int *i, bool write) {
    bool result = true;
    
    if (++(*i) >= size) {
        if (write) {
            char* message;
            asprintf(&message, "%s: argument %s has no value", command, argument);
            log_error_del_ctx(message, strdup("SHELL"));
        }

        result = false;
    }

    return result;
}

bool shu_check_noval_array(const char *command, const char *argument, bool is_assigned, bool write) {    
    if (!is_assigned && write) {
        char* message;
        asprintf(&message, "%s: argument %s can not be an empty array", command, argument);
        log_error_del_ctx(message, strdup("SHELL"));
    }

    return is_assigned;
}

bool shu_check_required(const char* command, const char* argument, bool is_assigned, bool write) {
    if (!is_assigned && write) {
        char *message;
        asprintf(&message, "%s: argument %s is required", command, argument);
        log_error_del_ctx(message, strdup("SHELL"));
    }

    return is_assigned;
}

bool shu_check_min_int(const char* command, const char* argument, int value, int min, bool write) {
    bool result = true; 

    if (value < min) {
        if (write) {
            char *message;
            asprintf(&message, "%s: argument %s is %i but must be greater or equal than %i", command, argument, value, min);
            log_error_del_ctx(message, strdup("SHELL"));
        }
        result = false;
    }

    return result;
}
bool shu_check_max_int(const char* command, const char* argument, int value, int max, bool write) {
    bool result = true; 

    if (value > max) {
        if (write) {
            char *message;
            asprintf(&message, "%s: argument %s is %i but must be lower or equal than %i", command, argument, value, max);
            log_error_del_ctx(message, strdup("SHELL"));
        }
        result = false;
    }

    return result;
}
bool shu_check_min_long(const char* command, const char* argument, long value, long min, bool write) {
    bool result = true; 

    if (value < min) {
        if (write) {
            char *message;
            asprintf(&message, "%s: argument %s is %li but must be greater or equal than %li", command, argument, value, min);
            log_error_del_ctx(message, strdup("SHELL"));
        }
        result = false;
    }

    return result;
}
bool shu_check_max_long(const char* command, const char* argument, long value, long max, bool write) {
    bool result = true; 

    if (value > max) {
        if (write) {
            char *message;
            asprintf(&message, "%s: argument %s is %li but must be lower or equal than %li", command, argument, value, max);
            log_error_del_ctx(message, strdup("SHELL"));
        }
        result = false;
    }

    return result;
}
bool shu_check_min_float(const char* command, const char* argument, float value, float min, bool write) {
    bool result = true; 

    if (value < min) {
        if (write) {
            char *message;
            asprintf(&message, "%s: argument %s is %f but must be greater or equal than %f", command, argument, value, min);
            log_error_del_ctx(message, strdup("SHELL"));
        }
        result = false;
    }

    return result;
}
bool shu_check_max_float(const char* command, const char* argument, float value, float max, bool write) {
    bool result = true; 

    if (value > max) {
        if (write) {
            char *message;
            asprintf(&message, "%s: argument %s is %f but must be lower or equal than %f", command, argument, value, max);
            log_error_del_ctx(message, strdup("SHELL"));
        }
        result = false;
    }

    return result;
}
bool shu_check_min_double(const char* command, const char* argument, double value, double min, bool write) {
    bool result = true; 

    if (value < min) {
        if (write) {
            char *message;
            asprintf(&message, "%s: argument %s is %f but must be greater or equal than %f", command, argument, value, min);
            log_error_del_ctx(message, strdup("SHELL"));
        }
        result = false;
    }

    return result;
}
bool shu_check_max_double(const char* command, const char* argument, double value, double max, bool write) {
    bool result = true; 

    if (value > max) {
        if (write) {
            char *message;
            asprintf(&message, "%s: argument %s is %f but must be lower or equal than %f", command, argument, value, max);
            log_error_del_ctx(message, strdup("SHELL"));
        }
        result = false;
    }

    return result;
}

bool shu_get_int_value(const char *command, const char *argument, char* str_value, int* value, bool write) {
    bool result = true;
    int length = strlen(str_value);

    *value = 0;

    if (length == 0 || (length == 1 && !isdigit(str_value[0]))) {
        result = false;
    }
    else {
        bool minus = false;
        int start = 0, i;

        if (str_value[0] == '-') {
            minus = true;
            start++;
        }
        
        for (i = start; i < length && result; i++) {
            if (!isdigit(str_value[i])) {
                result = false;
            }
            else {
                *value *= 10;
                *value += (str_value[i] - '0');
            }
        }

        if (minus) {
            *value = -*value;
        }
    }

    if (!result && write) {
        char *message;
        asprintf(&message, "%s: argument %s is %s but must be a valid int value", command, argument, str_value);
        log_error_del_ctx(message, strdup("SHELL"));
    }

    return result;
}

bool shu_get_long_value(const char *command, const char *argument, char* str_value, long* value, bool write) {
    bool result = true;
    int length = strlen(str_value);

    *value = 0;

    if (length == 0 || (length == 1 && !isdigit(str_value[0]))) {
        result = false;
    }
    else {
        bool minus = false;
        int start = 0, i;

        if (str_value[0] == '-') {
            minus = true;
            start++;
        }
        
        for (i = start; i < length && result; i++) {
            if (!isdigit(str_value[i])) {
                result = false;
            }
            else {
                *value *= 10;
                *value += (str_value[i] - '0');
            }
        }

        if (minus) {
            *value = -*value;
        }
    }

    if (!result && write) {
        char *message;
        asprintf(&message, "%s: argument %s is %s but must be a valid long value", command, argument, str_value);
        log_error_del_ctx(message, strdup("SHELL"));
    }

    return result;
}

bool shu_get_float_value(const char *command, const char *argument, char* str_value, float* value, bool write) {
    bool result = true;
    int length = strlen(str_value);

    *value = 0;

    if (length == 0 || (length == 1 && !isdigit(str_value[0]))) {
        result = false;
    }
    else {
        bool minus = false, point = false;
        int start = 0, i;
        float decimal_part = 0;

        if (str_value[0] == '-') {
            minus = true;
            start++;
        }
        
        for (i = start; i < length && !point && result; i++) {
            if (str_value[i] == '.') {
                point = true;
            }
            else if (!isdigit(str_value[i])) {
                result = false;
            }
            else {
                *value *= 10;
                *value += (str_value[i] - '0');
            }
        }

        start = i;

        for (i = length - 1; i >= start && result; i--) {
            if (!isdigit(str_value[i])) {
                result = false;
            }
            else {
                decimal_part += (str_value[i] - '0');
                decimal_part /= 10;
            }
        }

        *value += decimal_part;

        if (minus) {
            *value = -*value;
        }
    }

    if (!result && write) {
        char *message;
        asprintf(&message, "%s: argument %s is %s but must be a valid float value", command, argument, str_value);
        log_error_del_ctx(message, strdup("SHELL"));
    }

    return result;
}

bool shu_get_double_value(const char *command, const char *argument, char* str_value, double* value, bool write) {
    bool result = true;
    int length = strlen(str_value);

    *value = 0;

    if (length == 0 || (length == 1 && !isdigit(str_value[0]))) {
        result = false;
    }
    else {
        bool minus = false, point = false;
        int start = 0, i;
        double decimal_part = 0;

        if (str_value[0] == '-') {
            minus = true;
            start++;
        }
        
        for (i = start; i < length && !point && result; i++) {
            if (str_value[i] == '.') {
                point = true;
            }
            else if (!isdigit(str_value[i])) {
                result = false;
            }
            else {
                *value *= 10;
                *value += (str_value[i] - '0');
            }
        }

        start = i;

        for (i = length - 1; i >= start && result; i--) {
            if (!isdigit(str_value[i])) {
                result = false;
            }
            else {
                decimal_part += (str_value[i] - '0');
                decimal_part /= 10;
            }
        }

        *value += decimal_part;

        if (minus) {
            *value = -*value;
        }
    }

    if (!result && write) {
        char *message;
        asprintf(&message, "%s: argument %s is %s but must be a valid double value", command, argument, str_value);
        log_error_del_ctx(message, strdup("SHELL"));
    }

    return result;
}

bool shu_get_char_value(const char *command, const char *argument, char* str_value, char* value, bool write) {
    bool result;

    if (strlen(str_value) == 1) {
        *value = str_value[0];
        result = true;
    }
    else {
        if (write) {
            char *message;
            asprintf(&message, "%s: argument %s, %c is not a valid char value\n", command, argument, *value);
            log_error_del_ctx(message, strdup("SHELL"));
        }
        result = false;
    }

    return result;
}

char* shu_extract_argument(char *word, bool* is_alias) {
    const int len = strlen(word);
    char* argument = NULL;

    if (len > 2) {
        if (word[0] == '-' && word[1] == '-') {
            *is_alias = false;
            asprintf(&argument, "%.*s", len - 2, word + 2);
        }
    } 
    else if (len == 2) {
        if (word[0] == '-') {
            *is_alias = true;
            asprintf(&argument, "%c", word[1]);
        }
    } 
    
    return argument;    
}
