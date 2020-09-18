#include "text.h"

/* HELPER VARIABLES AND CONSTANTS */

static const char* _TXT_SPLIT_DELIMITERS = " \t\r\n\a";
static const int _TXT_SPLITLINE_INITIAL_BUFFER_SIZE = 8;

static const char* _TXT_MOVE_RIGHT = "\033[1C";
static const char* _TXT_MOVE_LEFT = "\033[1D";

static const int _TXT_UP[3] = { 27, 91, 65 };
static const int _TXT_DOWN[3] = { 27, 91, 66 };
static const int _TXT_RIGHT[3] = { 27, 91, 67 };
static const int _TXT_LEFT[3] = { 27, 91, 68 };
static const int _TXT_BACKSPACE = 127;

static struct termios _txt_default_terminal, _txt_advanced_terminal;

/* HELPER FUNCTIONS DECLARATIONS */

static void _txt_change_terminal();
static void _txt_reset_terminal();
static bool _txt_char_is_among(char ch, const char* chars);

/* EXPORTED FUNCTIONS */

char *txt_readline() {
    char* line = NULL;
    size_t size;

    if (getline(&line, &size, stdin) == -1) {
        log_error("Error in getting line");
    }

    return line;
}

char *txt_readline_special(char** history, int history_size) {
    char* line = strdup("");
    char typed_char[3];
    int size = 0, length = 0, curr = -1;

    int history_current = history_size;
    char* history_temp = NULL;

    int i;

    _txt_change_terminal();

    do {
        read(STDIN_FILENO, &typed_char, 3);
        if (typed_char[0] == '\n') {
            for (i = curr; i < length; i++) {
                printf("%s", _TXT_MOVE_RIGHT);
            }

            length += 2;

            if (length > size) {
                size = length;
                line = realloc(line, size);
            }

            line[length - 2] = '\n';
            line[length - 1] = '\0';
        }
        else if (typed_char[0] == _TXT_UP[0] && typed_char[1] == _TXT_UP[1] && typed_char[2] == _TXT_UP[2]) {
            // UP
            if (history_current > 0) {
                if (history_current == history_size) {
                    if (history_temp != NULL) {
                        free(history_temp);
                    }
                    asprintf(&history_temp, "%s", line);
                }
                free(line);

                for(i = 0; i <= curr; i++) {
                    printf("%s", _TXT_MOVE_LEFT);
                }
                for(i = 0; i < length; i++) {
                    printf(" ");
                }
                for(i = 0; i < length; i++) {
                    printf("%s", _TXT_MOVE_LEFT);
                }

                history_current--;
                line = strdup(history[history_current]);
                length = strlen(line);
                size = strlen(line);
                curr = length - 1;
                printf("%s", line);
            }
        }
        else if (typed_char[0] == _TXT_DOWN[0] && typed_char[1] == _TXT_DOWN[1] && typed_char[2] == _TXT_DOWN[2]) {
            // DOWN
            if (history_current < history_size) {
                for(i = 0; i <= curr; i++) {
                    printf("%s", _TXT_MOVE_LEFT);
                }
                for(i = 0; i < length; i++) {
                    printf(" ");
                }
                for(i = 0; i < length; i++) {
                    printf("%s", _TXT_MOVE_LEFT);
                }

                free(line);
                history_current++;
                if (history_current == history_size) {
                    line = strdup(history_temp);
                }
                else {
                    line = strdup(history[history_current]);
                }

                length = strlen(line);
                size = strlen(line);
                curr = length - 1;
                printf("%s", line);
            }
        }
        else if (typed_char[0] == _TXT_RIGHT[0] && typed_char[1] == _TXT_RIGHT[1] && typed_char[2] == _TXT_RIGHT[2]) {
            // RIGHT
            if (curr < length - 1) {
                printf("%s", _TXT_MOVE_RIGHT);
                curr++;
            }
        }
        else if (typed_char[0] == _TXT_LEFT[0] && typed_char[1] == _TXT_LEFT[1] && typed_char[2] == _TXT_LEFT[2]) {
            // LEFT
            if (curr >= 0) {
                printf("%s", _TXT_MOVE_LEFT);
                curr--;
            }
        }
        else if (typed_char[0] == _TXT_BACKSPACE) {
            // BACKSPACE
            if (curr >= 0) {
                printf("%s", _TXT_MOVE_LEFT);
                for (i = curr; i < length - 1; i++) {
                    printf("%c", line[i + 1]);
                    line[i] = line[i + 1];
                }
                printf(" ");
                for (i = curr; i < length - 1; i++) {
                    printf("%s", _TXT_MOVE_LEFT);
                }
                printf("%s", _TXT_MOVE_LEFT);

                curr--;
                length--;
            }
        }
        else {            
            length++;
            curr++;

            if (length > size) {
                char* reallocated = realloc(line, ++size);
                if (reallocated == NULL) {
                    printf("Error in reallocating resources\n");
                }
                line = reallocated;
            }

            for (i = length - 1; i > curr; i--) {
                line[i] = line[i - 1];
            }
            line[curr] = typed_char[0];

            for (i = curr; i < length; i++) {
                printf("%c", line[i]);
            }
            for (i = 0; i < length - curr - 1; i++) {
                printf("%s", _TXT_MOVE_LEFT);
            }
        }
        fflush(stdout);
    }
    while(typed_char[0] != '\n');

    _txt_reset_terminal();
    puts(" ");

    return line;
}

char **txt_splitline(char *line, int *size) {
    int index = -1;
    char **words;

    char* word;
    int word_size, word_index;

    bool double_apix = false;

    *size = _TXT_SPLITLINE_INITIAL_BUFFER_SIZE;
    words = (char**) malloc(*size * sizeof(char*));

    word_size = _TXT_SPLITLINE_INITIAL_BUFFER_SIZE;
    word = (char*) malloc(word_size * sizeof(char));
    word_index = -1;

    if (words == NULL) {
        log_error("Error in allocating words");
    }

    int i;
    for (i = 0; line[i] != '\0'; i++) {
        if (!double_apix && (_txt_char_is_among(line[i], _TXT_SPLIT_DELIMITERS) || line[i] == '"')) {
            if (word_index > -1) {
                if (++word_index >= word_size) {
                    word_size++;
                    word = realloc(word, word_size * sizeof(char));
                }

                word[word_index] = '\0';

                if (++index >= *size) {
                    *size *= 2;
                    words = realloc(words, *size * sizeof(char*));
                }

                words[index] = word;

                word_size = _TXT_SPLITLINE_INITIAL_BUFFER_SIZE;
                word = (char*) malloc(word_size * sizeof(char));
                word_index = -1;
            }

            if (line[i] == '"') {
                double_apix = true;
            }
        }
        else if (double_apix && line[i] == '"') {
            if (word_index > -1) {
                if (++word_index >= word_size) {
                    word_size++;
                    word = realloc(word, word_size * sizeof(char));
                }

                word[word_index] = '\0';

                if (++index >= *size) {
                    *size *= 2;
                    words = realloc(words, *size * sizeof(char*));
                }

                words[index] = word;

                word_size = _TXT_SPLITLINE_INITIAL_BUFFER_SIZE;
                word = (char*) malloc(word_size * sizeof(char));
                word_index = -1;
            }

            double_apix = false;
        }
        else if (double_apix && line[i] == '\\' && (line[i + 1] == '"' || line[i + 1] == '\\')) {
            i++;

            if (++word_index >= word_size) {
                word_size *= 2;
                word = realloc(word, word_size * sizeof(char));
            }

            word[word_index] = line[i];
        }
        else {
            if (++word_index >= word_size) {
                word_size *= 2;
                word = realloc(word, word_size * sizeof(char));
            }

            word[word_index] = line[i];
        }
    }

    if (word_index > -1) {
        if (++word_index >= word_size) {
            word_size++;
            word = realloc(word, word_size * sizeof(char));
        }

        word[word_index] = '\0';

        if (++index >= *size) {
            *size *= 2;
            words = realloc(words, *size * sizeof(char*));
        }

        words[index] = word;
    }

    *size = index + 1;

    return words;
}

void txt_free_string_array(char **array, int size) {
    int i;
    
    for (i = 0; i < size; i++) {
        free(array[i]);
    }
    free(array);
}

/* HELPER FUNCTIONS DEFINITIONS */

static void _txt_change_terminal() {
    tcgetattr(0, &_txt_default_terminal);
    _txt_advanced_terminal = _txt_default_terminal;
    _txt_advanced_terminal.c_lflag &= ~ICANON;
    _txt_advanced_terminal.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &_txt_advanced_terminal);
}

static void _txt_reset_terminal() {
    tcsetattr(0, TCSANOW, &_txt_default_terminal);
}

static bool _txt_char_is_among(char ch, const char* chars) {
    bool result = false;
    
    int i;
    for (i = 0; chars[i] != '\0' && !result; i++) {
        if (ch == chars[i]) {
            result = true;
        }
    }

    return result;
}