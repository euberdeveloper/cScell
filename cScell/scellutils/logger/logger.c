#include "logger.h"

/* HELPER FUNCTIONS SIGNATURES */

static char* _log_bracket_text(char* text);

/* EXPORTED FUNCTIONS */

// Without context and deallocation

void log_info(char *message) {
    printf("%s\t%s\n", col_blue("[INFO]"), message);
}

void log_success(char *message) {
    printf("%s\t%s\n", col_green("[SUCC]"), message);
}

void log_debug(char *message) {
    printf("%s\t%s\n", col_gray("[DEBUG]"), message);
}

void log_error(char *message) {
    printf("%s\t%s\n", col_red("[ERROR]"), message);
}

void log_warning(char *message) {
    printf("%s\t%s\n", col_yellow("[WARN]"), message);
}

// Without context and with deallocation

void log_info_del(char *message) {
    printf("%s\t%s\n", col_blue("[INFO]"), message);
    free(message);
}

void log_success_del(char *message) {
    printf("%s\t%s\n", col_green("[SUCC]"), message);
    free(message);
}

void log_debug_del(char *message) {
    printf("%s\t%s\n", col_gray("[DEBUG]"), message);
    free(message);
}

void log_error_del(char *message) {
    printf("%s\t%s\n", col_red("[ERROR]"), message);
    free(message);
}

void log_warning_del(char *message) {
    printf("%s\t%s\n", col_yellow("[WARN]"), message);
    free(message);
}

// With context and without deallocation

void log_info_ctx(char *message, char *context) {
    printf("%s\t%s\t%s\n", col_blue("[INFO]"), col_blue_del(_log_bracket_text(context)), message);
}

void log_success_ctx(char *message, char *context) {
    printf("%s\t%s\t%s\n", col_green("[SUCC]"), col_green_del(_log_bracket_text(context)), message);
}

void log_debug_ctx(char *message, char *context) {
    printf("%s\t%s\t%s\n", col_gray("[DEBUG]"), col_gray_del(_log_bracket_text(context)), message);
}

void log_error_ctx(char *message, char *context) {
    printf("%s\t%s\t%s\n", col_red("[ERROR]"), col_red_del(_log_bracket_text(context)), message);
}

void log_warning_ctx(char *message, char *context) {
    printf("%s\t%s\t%s\n", col_yellow("[WARN]"), col_yellow_del(_log_bracket_text(context)), message);
}

// With context and with deallocation

void log_info_del_ctx(char *message, char *context) {
    printf("%s\t%s\t%s\n", col_blue("[INFO]"), col_blue_del(_log_bracket_text(context)), message);
    free(message);
    free(context);
}

void log_success_del_ctx(char *message, char *context) {
    printf("%s\t%s\t%s\n", col_green("[SUCC]"), col_green_del(_log_bracket_text(context)), message);
    free(message);
    free(context);
}

void log_debug_del_ctx(char *message, char *context) {
    printf("%s\t%s\t%s\n", col_gray("[DEBUG]"), col_gray_del(_log_bracket_text(context)), message);
    free(message);
    free(context);
}

void log_error_del_ctx(char *message, char *context) {
    printf("%s\t%s\t%s\n", col_red("[ERROR]"), col_red_del(_log_bracket_text(context)), message);
    free(message);
    free(context);
}

void log_warning_del_ctx(char *message, char *context) {
    printf("%s\t%s\t%s\n", col_yellow("[WARN]"), col_yellow_del(_log_bracket_text(context)), message);
    free(message);
    free(context);
}

/* HELPER FUNCTIONS DEFINITIONS */

static char* _log_bracket_text(char* text) {
    char* result;
    asprintf(&result, "{%s}", text);
    return result;
}