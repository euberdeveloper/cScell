#include "colour.h"

// Without free

char* col_blue(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_BLUE, str, ANSI_COLOR_RESET);
    return result;
}

char* col_green(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_GREEN, str, ANSI_COLOR_RESET);
    return result;
}

char* col_gray(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_DARK_GRAY, str, ANSI_COLOR_RESET);
    return result;
}

char* col_red(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_RED, str, ANSI_COLOR_RESET);
    return result;
}

char* col_yellow(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_YELLOW, str, ANSI_COLOR_RESET);
    return result;
}

char* col_magenta(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_MAGENTA, str, ANSI_COLOR_RESET);
    return result;
}

char* col_cyan(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_CYAN, str, ANSI_COLOR_RESET);
    return result;
}

// With free

char* col_blue_del(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_BLUE, str, ANSI_COLOR_RESET);
    free(str);
    return result;
}

char* col_green_del(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_GREEN, str, ANSI_COLOR_RESET);
    free(str);
    return result;
}

char* col_gray_del(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_DARK_GRAY, str, ANSI_COLOR_RESET);
    free(str);
    return result;
}

char* col_red_del(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_RED, str, ANSI_COLOR_RESET);
   free(str);
    return result;
}

char* col_yellow_del(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_YELLOW, str, ANSI_COLOR_RESET);
    free(str);
    return result;
}

char* col_magenta_del(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_MAGENTA, str, ANSI_COLOR_RESET);
   free(str);
    return result;
}

char* col_cyan_del(char *str) {
    char *result;
    asprintf(&result, "%s%s%s", ANSI_COLOR_CYAN, str, ANSI_COLOR_RESET);
    free(str);
    return result;
}