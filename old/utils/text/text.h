#ifndef TXT_TEXT_H
#define TXT_TEXT_H

/* IMPORTS */

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "../boolean/boolean.h"
#include "../logger/logger.h"

/* FUNCTIONS */

/**
 * Reads from stdin a line of text
 * @return The read line as a string
*/
char *txt_readline();
/**
 * Reads from stdin a line of text, providing also backspace, left 
 * and right arrow usage and a history of the old commands, accessible
 * with the up and down arrows.
 * @param history The strings array containing the old commands
 * @param history_size The number of old commands
 * @return The read line as a string
*/
char *txt_readline_special(char** history, int history_size);
/**
 * Splits a line into words
 * @param line The line to be split into words
 * @param size A pointer to an integer that will contain the number of the returned words
 * @return An array of strings containing the obtained words
*/
char **txt_splitline(char *line, int *size);
/**
 * Deallocates a given array of strings
 * @param array The strings array to be deallocated
 * @param size The number of elements of the given strings array 
*/
void txt_free_string_array(char **array, int size);

#endif