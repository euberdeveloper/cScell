#include "shell_commands.h"

/* HELPER CONSTANTS */

static const int _SH_INIT_BUFFER = 23;

/* HELPER FUNCTIONS SIGNATURES */

void shu_print_command_help(char *command_name, char *command_description, sh_param_details *details, int details_size);

/* EXPORTED VARIABLES */

char *sh_last_command;
char *sh_prompt_symbol = ">> ";
bool sh_use_advanced_terminal = true;

/* EXPORTED FUNCTIONS */

SH_STATE sh_handle__arguments(char **words, int n_words) {
    SH_STATE state = SH_CONTINUE;

    bool is_assigned_a = false;

    int a;

    bool is_alias, finish = false;
    char *argument = NULL;
    int i;

    for (i = 1; i < n_words && !finish; i++) {
        argument = shu_extract_argument(words[i], &is_alias);

        if (argument == NULL) {
            shu_value_without_argument(words[i], false);
        }
        else {
            if (strcmp(argument, "a") == 0) {
			    finish = !shu_check_noval("_arguments", "a", n_words, &i, false);
			    if (!finish) {
			        is_assigned_a = true;
			        finish = !shu_get_int_value("_arguments", "a", words[i], &a, false);         
			    }
			}
			else {
				shu_unknown_argument("_arguments", words[i], false);
			}
        }
    }

    if (!finish) {
		finish = !shu_check_required("_arguments", "a", is_assigned_a, false);
	}

    if (!finish) {  
        state = arguments(a);
    }

    return state;
}

SH_STATE sh_handle_add(char **words, int n_words) {
    SH_STATE state = SH_CONTINUE;

    bool is_assigned_first = false;
	bool is_assigned_second = false;

    int first;
	int second;

    bool is_alias, finish = false;
    char *argument = NULL;
    int i;

    for (i = 1; i < n_words && !finish; i++) {
        argument = shu_extract_argument(words[i], &is_alias);

        if (argument == NULL) {
            shu_value_without_argument(words[i], true);
        }
        else {
            if (strcmp(argument, "first") == 0 || (is_alias && strcmp(argument, "x") == 0)) {
			    finish = !shu_check_noval("add", "first", n_words, &i, true);
			    if (!finish) {
			        is_assigned_first = true;
			        finish = !shu_get_int_value("add", "first", words[i], &first, true);         
			    }
			}
			else if (strcmp(argument, "second") == 0 || (is_alias && strcmp(argument, "y") == 0)) {
			    finish = !shu_check_noval("add", "second", n_words, &i, true);
			    if (!finish) {
			        is_assigned_second = true;
			        finish = !shu_get_int_value("add", "second", words[i], &second, true);         
			    }
			}
			else {
				shu_unknown_argument("add", words[i], true);
			}
        }
    }

    if (!finish) {
		finish = !shu_check_required("add", "first", is_assigned_first, true);
	}
	if (!finish) {
		finish = !shu_check_required("add", "second", is_assigned_second, true);
	}

    if (!finish) {  
        state = init(first, second);
    }

    return state;
}

SH_STATE sh_handle_sub(char **words, int n_words) {
    SH_STATE state = SH_CONTINUE;

    bool is_assigned_first = false;
	bool is_assigned_second = false;

    int first;
	int second;

    bool is_alias, finish = false;
    char *argument = NULL;
    int i;

    for (i = 1; i < n_words && !finish; i++) {
        argument = shu_extract_argument(words[i], &is_alias);

        if (argument == NULL) {
            shu_value_without_argument(words[i], true);
        }
        else {
            if (strcmp(argument, "first") == 0 || (is_alias && strcmp(argument, "x") == 0)) {
			    finish = !shu_check_noval("sub", "first", n_words, &i, true);
			    if (!finish) {
			        is_assigned_first = true;
			        finish = !shu_get_int_value("sub", "first", words[i], &first, true);         
			    }
			}
			else if (strcmp(argument, "second") == 0 || (is_alias && strcmp(argument, "y") == 0)) {
			    finish = !shu_check_noval("sub", "second", n_words, &i, true);
			    if (!finish) {
			        is_assigned_second = true;
			        finish = !shu_get_int_value("sub", "second", words[i], &second, true);         
			    }
			}
			else {
				shu_unknown_argument("sub", words[i], true);
			}
        }
    }

    if (!finish) {
		finish = !shu_check_required("sub", "first", is_assigned_first, true);
	}
	if (!finish) {
		finish = !shu_check_required("sub", "second", is_assigned_second, true);
	}

    if (!finish) {  
        state = sub(first, second);
    }

    return state;
}

SH_STATE sh_handle_quit(char **words, int n_words) {
    SH_STATE state = SH_CONTINUE;

    

    

    bool is_alias, finish = false;
    char *argument = NULL;
    int i;

    for (i = 1; i < n_words && !finish; i++) {
        argument = shu_extract_argument(words[i], &is_alias);

        if (argument == NULL) {
            shu_value_without_argument(words[i], true);
        }
        else {
            
			shu_unknown_argument("quit", words[i], true);
        }
    }

    

    if (!finish) {  
        state = quit();
    }

    return state;
}


void sh_help__arguments() {
	sh_param_details details[1];

	details[0].name = "a";
	details[0].alias = NULL;
	details[0].type = "int";
	details[0].default_value = NULL;
	details[0].description = NULL;

	shu_print_command_help("_arguments", NULL, details, 1);
}
void sh_help_add() {
	sh_param_details details[2];

	details[0].name = "first";
	details[0].alias = "x";
	details[0].type = "int";
	details[0].default_value = NULL;
	details[0].description = "The number of P processes. It must be at least 1.";
	
	details[1].name = "second";
	details[1].alias = "y";
	details[1].type = "int";
	details[1].default_value = NULL;
	details[1].description = "The number of Q processes. It must be at least 1.";

	shu_print_command_help("add", "Initializes the shit", details, 2);
}
void sh_help_sub() {
	sh_param_details details[2];

	details[0].name = "first";
	details[0].alias = "x";
	details[0].type = "int";
	details[0].default_value = NULL;
	details[0].description = "The number of P processes. It must be at least 1.";
	
	details[1].name = "second";
	details[1].alias = "y";
	details[1].type = "int";
	details[1].default_value = NULL;
	details[1].description = "The number of Q processes. It must be at least 1.";

	shu_print_command_help("sub", "Initializes the shit", details, 2);
}
void sh_help_quit() {
	

	

	shu_print_command_help("quit", NULL, NULL, 0);
}

void sh_help(char **words, int size) {
	if (size == 1) {
		sh_help__arguments();
		sh_help_add();
		sh_help_sub();
		sh_help_quit();
	}
	else {
        char* command = words[1];

		int i;
		for (i = 2; i < size; i++) {
			shu_help_misplaced_argument(words[i]);
		}

		if (strcmp(command, "_arguments") == 0) {
			sh_help__arguments();
		}
		else if (strcmp(command, "add") == 0) {
			sh_help_add();
		}
		else if (strcmp(command, "sub") == 0) {
			sh_help_sub();
		}
		else if (strcmp(command, "quit") == 0) {
			sh_help_quit();
		}
		else {
			shu_help_unknown_command(words[1]);
		}
	}
}

SH_STATE sh_parse_command(char **words, int size) {
    SH_STATE state = SH_CONTINUE;

    if (size > 0) {
        char *command = words[0];

        if (strcmp(command, "help") == 0)  {
			sh_help(words, size);
		}
        else if (strcmp(command, "add") == 0) {
			state = sh_handle_add(words, size);
		}
		else if (strcmp(command, "sub") == 0) {
			state = sh_handle_sub(words, size);
		}
		else if (strcmp(command, "quit") == 0) {
			state = sh_handle_quit(words, size);
		}
        else {
            shu_unknown_command(command);
            state = SH_CONTINUE;
        }
    }

    return state;
}

void sh_loop() {
    SH_STATE state = SH_CONTINUE;

    int n_words;
    char *command, **words;
	
	int history_size = 8, history_index = 0, temp;
	char** history = malloc(history_size * sizeof(char*));

    sh_last_command = strdup("");

    while (state != SH_EXIT) {
        printf("%s", sh_prompt_symbol);
        fflush(stdout);

        command = sh_use_advanced_terminal 
            ? txt_readline_special(history, history_index)
            : txt_readline();

        free(sh_last_command);
        sh_last_command = strdup(command);

		if (history_index == history_size) {
			history_size *= 2;
			history = realloc(history, history_size * sizeof(char*));
		}

		history[history_index] = strdup(command);
		history[history_index][strlen(command) - 1] = '\0';
		history_index++;

        words = txt_splitline(command, &n_words);
        state = sh_parse_command(words, n_words);

        free(command);
        txt_free_string_array(words, n_words);
    }
}

/* HELPER FUNCTIONS DEFINITIONS */

void shu_print_command_help(char *command_name, char *command_description, sh_param_details *details, int details_size) {
  printf("%-15s", command_name);
  if (command_description != NULL) {
    printf("%s\n", command_description);
  } 
  else {
    printf("\n");
  }
  int i;
  for (i = 0; i < details_size; i++) {
    printf("--%-13s", details[i].name);
    
    printf("-%-9s", details[i].alias);
    printf("%-15s", details[i].type);
    if(details[i].default_value != NULL) {
      printf("default: %-11s", details[i].default_value);
    } 
    else {
      printf("%-20s", "required");
    }
    if(details[i].description != NULL) {
      printf("%-80s\n", details[i].description);
    } 
    else {
      printf("%-80s\n", "no description");
    }
  }
  printf("\n");
}