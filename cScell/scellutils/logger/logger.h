#ifndef LOG_LOGGER_H
#define LOB_LOGGER_H

/* IMPORTS */

#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>

#include "../colour/colour.h"

/* FUNCTIONS */

/**
 * Logs an info message
 * @param message The message to be logged
*/
void log_info(char *message);
/**
 * Logs a success message
 * @param message The message to be logged
*/
void log_success(char *message);
/**
 * Logs a debug message
 * @param message The message to be logged
*/
void log_debug(char *message);
/**
 * Logs an error message
 * @param message The message to be logged
*/
void log_error(char *message);
/**
 * Logs a warning message
 * @param message The message to be logged
*/
void log_warning(char *message);

/**
 * Logs a info message. The passed string will be deallocated.
 * @param message The message to be logged. It will be deallocated.
*/
void log_info_del(char *message);
/**
 * Logs a success message. The passed string will be deallocated.
 * @param message The message to be logged. It will be deallocated.
*/
void log_success_del(char *message);
/**
 * Logs a debug message. The passed string will be deallocated.
 * @param message The message to be logged. It will be deallocated.
*/
void log_debug_del(char *message);
/**
 * Logs a error message. The passed string will be deallocated.
 * @param message The message to be logged. It will be deallocated.
*/
void log_error_del(char *message);
/**
 * Logs a warning message. The passed string will be deallocated.
 * @param message The message to be logged. It will be deallocated.
*/
void log_warning_del(char *message);

/**
 * Logs an info message, logging also a context between brackets
 * @param message The message to be logged
 * @param context The context to be logged between brackets
*/
void log_info_ctx(char *message, char *context);
/**
 * Logs a success message, logging also a context between brackets
 * @param message The message to be logged
 * @param context The context to be logged between brackets
*/
void log_success_ctx(char *message, char *context);
/**
 * Logs a debug message, logging also a context between brackets
 * @param message The message to be logged
 * @param context The context to be logged between brackets
*/
void log_debug_ctx(char *message, char *context);
/**
 * Logs an error message, logging also a context between brackets
 * @param message The message to be logged
 * @param context The context to be logged between brackets
*/
void log_error_ctx(char *message, char *context);
/**
 * Logs a warnig message, logging also a context between brackets
 * @param message The message to be logged
 * @param context The context to be logged between brackets
*/
void log_warning_ctx(char *message, char *context);

/**
 * Logs an info message, logging also a context between brackets. The parameters will be deallocated
 * @param message The message to be logged. It will be also deallocated
 * @param context The context to be logged between brackets It will be also deallocated
*/
void log_info_del_ctx(char *message, char *context);
/**
 * Logs a success message, logging also a context between brackets. The parameters will be deallocated
 * @param message The message to be logged. It will be also deallocated
 * @param context The context to be logged between brackets It will be also deallocated
*/
void log_success_del_ctx(char *message, char *context);
/**
 * Logs a debug message, logging also a context between brackets. The parameters will be deallocated
 * @param message The message to be logged. It will be also deallocated
 * @param context The context to be logged between brackets It will be also deallocated
*/
void log_debug_del_ctx(char *message, char *context);
/**
 * Logs an error message, logging also a context between brackets. The parameters will be deallocated
 * @param message The message to be logged. It will be also deallocated
 * @param context The context to be logged between brackets It will be also deallocated
*/
void log_error_del_ctx(char *message, char *context);
/**
 * Logs a warnig message, logging also a context between brackets. The parameters will be deallocated
 * @param message The message to be logged. It will be also deallocated
 * @param context The context to be logged between brackets It will be also deallocated
*/
void log_warning_del_ctx(char *message, char *context);

#endif