#ifndef COL_COLOUR_H
#define COL_COLOUR_H

/* IMPORTS */

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>

/* CONSTANTS */

#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_GREEN        "\x1b[32m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_BLUE         "\x1b[34m"
#define ANSI_COLOR_MAGENTA      "\x1b[35m"
#define ANSI_COLOR_CYAN         "\x1b[36m"
#define ANSI_COLOR_DARK_GRAY    "\e[1;30m"
#define ANSI_COLOR_RESET        "\x1b[0m"

/* FUNCTIONS */

/**
 * Returns a blue-coloured string
 * @param str  The string to be coloured
 * @return The blue-coloured string
*/
char* col_blue(char *str);
/**
 * Returns a green-coloured string
 * @param str  The string to be coloured
 * @return The green-coloured string
*/
char* col_green(char *str);
/**
 * Returns a gray-coloured string
 * @param str  The string to be coloured
 * @return The gray-coloured string
*/
char* col_gray(char *str);
/**
 * Returns a red-coloured string
 * @param str  The string to be coloured
 * @return The red-coloured string
*/
char* col_red(char *str);
/**
 * Returns a yellow-coloured string
 * @param str  The string to be coloured
 * @return The yellow-coloured string
*/
char* col_yellow(char *str);
/**
 * Returns a magenta-coloured string
 * @param str  The string to be coloured
 * @return The magenta-coloured string
*/
char* col_magenta(char *str);
/**
 * Returns a cyan-coloured string
 * @param str  The string to be coloured
 * @return The cyan-coloured string
*/
char* col_cyan(char *str);

/**
 * Returns a blue-coloured string, deallocating the one passed
 * @param str  The string to be coloured. It will be deallocated
 * @return The blue-coloured string
*/
char* col_blue_del(char *str);
/**
 * Returns a green-coloured string, deallocating the one passed
 * @param str  The string to be coloured. It will be deallocated
 * @return The green-coloured string
*/
char* col_green_del(char *str);
/**
 * Returns a gray-coloured string, deallocating the one passed
 * @param str  The string to be coloured. It will be deallocated
 * @return The gray-coloured string
*/
char* col_gray_del(char *str);
/**
 * Returns a red-coloured string, deallocating the one passed
 * @param str  The string to be coloured. It will be deallocated
 * @return The red-coloured string
*/
char* col_red_del(char *str);
/**
 * Returns a yellow-coloured string, deallocating the one passed
 * @param str  The string to be coloured. It will be deallocated
 * @return The yellow-coloured string
*/
char* col_yellow_del(char *str);
/**
 * Returns a magenta-coloured string, deallocating the one passed
 * @param str  The string to be coloured. It will be deallocated
 * @return The magenta-coloured string
*/
char* col_magenta_del(char *str);
/**
 * Returns a cyan-coloured string, deallocating the one passed
 * @param str  The string to be coloured. It will be deallocated
 * @return The cyan-coloured string
*/
char* col_cyan_del(char *str);

#endif