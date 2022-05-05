#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


/// size of each line
#define MAX_LINE_SIZE 256
/// max number of commands
#define MAX_COMMANDS 100
/// max number of chars in a command
#define MAX_COMMAND_LINE_SIZE 100


enum EditCommand {
    APPEND_END,    /// append to the end of the line
    APPEND_START, /// aka insert, append to the start of the line
    APPEND_TITLE, /// aka O option, insert a line before the actual line
    DELETE,     /// delete the line
    REPLACE, /// replace a pattern with another pattern in the line
};
enum LineRange {
    LINE_RANGE_CONTAINS, /// line containing a pattern are effected
    LINE_RANGE_START_END, /// line with specific start and end are effected
    LINE_RANGE_NONE  /// every line is effected
};
/// command structure to keep track of the command
typedef struct command_struct {
    /// the range of the line to be effected
    enum LineRange line_range;
    /// the edit command to be performed
    enum EditCommand command;
    /// arg is an arbitrary string that serves based on the edit type
    char *arg1;
    /// arg2 is an arbitrary string that serves based on the edit type
    char *arg2;
    /// arg3 is an arbitrary string that serves based on the edit type
    char *arg3;
    /// start line number to be effected
    int line_number1;
    /// end line number to be effected
    int line_number2;
} command_t;

/// command array to keep track of the command reading report
typedef struct command_report {
    /// read commands
    command_t *command;
    /// number of commands read
    size_t command_count;
} command_report_t;


/// function to replace a pattern with another pattern in a line
char* replace_word(const char *s, const char *oldW,
                  const char *newW) {
    /// allocate memory for the new string
    char *result;
    int i, cnt = 0;
    /// the size of the new string
    /// the size of the old string
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
    /// Counting the number of times old word
    /// occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
            /// Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
    /// Making new string of enough length
    result = (char *) malloc(i + cnt * (newWlen - oldWlen) + 1);
    i = 0;
    while (*s) {
        /// compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        } else
            result[i++] = *s++;
    }
    /// Adding null character
    result[i] = '\0';
    return result;
}

/// function to read the commands from the standard I/O
command_report_t read_commands(char **commands) {
    /// allocate memory for the commands
    command_t *commands_array = calloc(MAX_COMMANDS, sizeof(command_t));
    /// variable to keep track of the number of commands
    auto counter = 0;
    /// variable to keep fd of the file
    FILE *filePointer;
    /// variable to keep track MAX_COMMAND_LINE_SIZE
    int bufferLength = MAX_COMMAND_LINE_SIZE;
    /// allocate buffer to read the line
    char buffer[bufferLength];
    /// open the file with read permission
    filePointer = fopen(commands[1], "r");
    /// check if the file is opened and has any new line to be read
    while (fgets(buffer, bufferLength, filePointer)) {
        /// allocate memory for the command args
        commands_array[counter].arg1 = calloc(MAX_LINE_SIZE, sizeof(char));
        /// allocate memory for the command args
        commands_array[counter].arg2 = calloc(MAX_LINE_SIZE, sizeof(char));
        /// allocate memory for the command args
        commands_array[counter].arg3 = calloc(MAX_LINE_SIZE, sizeof(char));
        /// check if it starts with a digit, if so it is a line number
        if (isdigit(buffer[0])) {
            /// get the start line number
            commands_array[counter].line_number1 = atoi(&buffer[0]);
            /// get the end line number
            commands_array[counter].line_number2 = atoi(&buffer[2]);
            /// set command line ranger to be with start and end line number
            commands_array[counter].line_range = LINE_RANGE_START_END;
            /// read parameters of the command
            char *command = strtok(buffer, "/");
            command = strtok(NULL, "/");  /// read command
            /// check if the command is insert
            if (command[0] == 'I') {
                commands_array[counter].command = APPEND_START;
                /// skip the first five characters (line numbers and , ) and read the parameters
                strcpy(commands_array[counter].arg1, buffer + 5);
                // check if the command is to add title before the current line
            } else if (command[0] == 'O') {
                commands_array[counter].command = APPEND_TITLE;
                /// skip the first five characters (line numbers and , ) and read the parameters
                strcpy(commands_array[counter].arg1, buffer + 5);
                /// check if the command is to delete the current line
            } else if (command[0] == 'd') {
                commands_array[counter].command = DELETE;
                /// skip the first five characters (line numbers and , ) and read the parameters
                strcpy(commands_array[counter].arg1, buffer + 5);
                /// check if the command is to append to the current line
            } else if (command[0] == 'A') {
                commands_array[counter].command = APPEND_END;
                /// skip the first five characters (line numbers and , ) and read the parameters
                commands_array[counter].arg1 = buffer + 5;
                /// check if the command is to replace the current line
            } else if (command[0] == 's') {
                /// read two parameters for replace function
                char* tmp = strtok(buffer, "/");
                strcpy(commands_array[counter].arg2, tmp);
                tmp = strtok(buffer, "/");
                strcpy(commands_array[counter].arg3, tmp);
                commands_array[counter].command = REPLACE;
            }
            /// check if the command starts with / , if so it is a line containing filter
        } else if (buffer[0] == '/') {
            commands_array[counter].line_range = LINE_RANGE_CONTAINS;
            char *command = strtok(buffer, "/");  /// skip empty string
            strcpy(commands_array[counter].arg1, command);
            command = strtok(NULL, "/"); /// read command data
            /// check if the command is insert
            if (command[0] == 'I') {
                commands_array[counter].command = APPEND_START;
                /// skip the first  character  and read the parameters
                strcpy(commands_array[counter].arg2, command + 1);
                /// check if the command is to add title before the current line
            } else if (command[0] == 'O') {
                commands_array[counter].command = APPEND_TITLE;
                /// skip the first  character  and read the parameters
                strcpy(commands_array[counter].arg2, command + 1);
                /// check if the command is to delete the current line
            } else if (command[0] == 'd') {
                commands_array[counter].command = DELETE;
                /// skip the first  character  and read the parameters
                strcpy(commands_array[counter].arg2, command + 1);
                /// check if the command is to append to the current line
            } else if (command[0] == 'A') {
                commands_array[counter].command = APPEND_END;
                /// skip the first  character  and read the parameters
                strcpy(commands_array[counter].arg2, command + 1);
                /// check if the command is to replace the current line
            } else if (command[0] == 's') {
                /// read two parameters for replace function
                char* tmp = strtok(NULL, "/");
                strcpy(commands_array[counter].arg2, tmp);
                tmp = strtok(NULL, "/");
                strcpy(commands_array[counter].arg3, tmp);
                commands_array[counter].command = REPLACE;
            }
            /// check if the command starts with other things, so it is a wild card line filter
        } else {
            commands_array[counter].line_range = LINE_RANGE_NONE;
            /// read the command data
            strcpy(commands_array[counter].arg1, buffer + 1);
            /// check if the command is insert
            if (buffer[0] == 'I') {
                commands_array[counter].command = APPEND_START;
                /// check if the command is to add title before the current line
            } else if (buffer[0] == 'O') {
                commands_array[counter].command = APPEND_TITLE;
                /// check if the command is to delete the current line
            } else if (buffer[0] == 'd') {
                commands_array[counter].command = DELETE;
                /// check if the command is to append to the current line
            } else if (buffer[0] == 'A') {
                commands_array[counter].command = APPEND_END;
                /// check if the command is to replace the current line
            } else if (buffer[0] == 's') {
                /// read two parameters for replace function
                char *tmp = strtok(NULL, "/");
                strcpy(commands_array[counter].arg2, tmp);
                tmp = strtok(NULL, "/");
                strcpy(commands_array[counter].arg3, tmp);
                commands_array[counter].command = REPLACE;
            }
        }
        /// increment the counter
        counter++;
    }
    /// close the file
    fclose(filePointer);
    /// return the number of commands and comands as a report
    command_report_t result;
    result.command_count = counter ;
    result.command = commands_array;
    return result;
}

bool string_contains_pattern(char *string, char *pattern) {
    /// check if the string is containing a pattern
    return strstr(string, pattern);
}


int main(int argc, char **argv) {
    /// read the commands from the file
    command_report_t result = read_commands(argv);
    /// define stdin as default input
    FILE *input = stdin;
    /// buffer to store the input line
    char *buffer;
    /// size of the buffer when returned from getline
    size_t getline_size;
    /// counter for the lines
    int line_number = 1;
    /// allocate memory for the buffer
    buffer = calloc(MAX_LINE_SIZE, sizeof(char));
    /// read the first line
    size_t line_result = getline(&buffer, &getline_size, input);
    /// check if the line is empty
    if (line_result < 0) {
        printf("input file doesn't have any line\n");
        return -1;
    }
    /// keep reading the lines until EOF
    do {
        /// check the line against every filter
        for (int i = 0; i < result.command_count; i++) {
            /// check if a line is already marked to be deleted so we won't bother with keeping it updated
            if (strlen(buffer) == 0) {
                break;
            }
            /// check the type of the command
            switch (result.command[i].command) {

                /// it is a append command
                case APPEND_START:
                    /// check what line filter is applied for the filter
                    switch (result.command[i].line_range) {
                        /// word containing filter
                        case LINE_RANGE_CONTAINS:
                            /// check if the line contains the pattern
                            if (string_contains_pattern(buffer, result.command[i].arg1)) {
                                /// append the pattern to the line
                                char tmp [strlen(result.command[i].arg1)]  ;
                                strcpy(tmp, result.command[i].arg1);
                                strcat(tmp , buffer);
                                strcpy(buffer, tmp);
                            }
                            break;
                            /// line number filter
                        case LINE_RANGE_START_END:
                            /// check if the line number is in the range
                            if (line_number >= result.command[i].line_number1 &&
                                line_number <= result.command[i].line_number2) {
                                char tmp [strlen(result.command[i].arg1)]  ;
                                strcpy(tmp, result.command[i].arg1);
                                strcat(tmp , buffer);
                                strcpy(buffer, tmp);
                            }
                            break;
                            /// no filter
                        case LINE_RANGE_NONE:{
                            /// apply the filter without any condition
                            char tmp [strlen(result.command[i].arg1)]  ;
                            strcpy(tmp, result.command[i].arg1);
                            strcat(tmp , buffer);
                            strcpy(buffer, tmp);
                            break;
                        }
                    }
                    break;
                /// it is a append end command
                case APPEND_END:
                    /// check what line filter is applied for the filter
                    switch (result.command[i].line_range) {
                        /// word containing filter
                        case LINE_RANGE_CONTAINS:
                            /// check if the line contains the pattern
                            if (string_contains_pattern(buffer, result.command[i].arg1)) {
                                 strcat(buffer, result.command[i].arg1);
                            }
                            break;

                            /// line number filter
                        case LINE_RANGE_START_END:
                            /// check if the line number is in the range
                            if (line_number >= result.command[i].line_number1 &&
                                line_number <= result.command[i].line_number2) {
                                 strcat(buffer, result.command[i].arg1);
                            }
                            break;
                            /// no filter
                        case LINE_RANGE_NONE:
                            /// apply the filter without any condition
                            strcat(buffer, result.command[i].arg1);
                            break;
                    }
                    break;
                /// it is a append title command
                case APPEND_TITLE:
                    /// check what line filter is applied for the filter
                    switch (result.command[i].line_range) {
                        /// word containing filter
                        case LINE_RANGE_CONTAINS:
                            /// check if the line contains the pattern
                            if (string_contains_pattern(buffer, result.command[i].arg1)) {
                                printf("%s", result.command[i].arg1);
                            }
                            break;
                            /// line number filter
                        case LINE_RANGE_START_END:
                            /// check if the line number is in the range
                            if (line_number >= result.command[i].line_number1 &&
                                line_number <= result.command[i].line_number2) {
                                printf("%s", result.command[i].arg1);
                            }
                            break;
                            /// no filter
                        case LINE_RANGE_NONE:
                            /// apply the filter without any condition
                            printf("%s", result.command[i].arg1);
                            break;
                    }
                    break;
                /// it is a delete command
                case DELETE:
                    /// check what line filter is applied for the filter
                    switch (result.command[i].line_range) {
                        /// word containing filter
                        case LINE_RANGE_CONTAINS:
                            /// check if the line contains the pattern
                            if (string_contains_pattern(buffer, result.command[i].arg1)) {
                              buffer[0] = '\0' ;
                            }
                            break;
                            /// line number filter
                        case LINE_RANGE_START_END:
                            /// check if the line number is in the range
                            if (line_number >= result.command[i].line_number1 &&
                                line_number <= result.command[i].line_number2) {
                                buffer[0] = '\0' ;
                            }
                            break;
                            /// no filter
                        case LINE_RANGE_NONE:
                            /// apply the filter without any condition
                            buffer[0] = '\0' ;
                            break;
                    }
                    break;
                /// it is a replace command
                case REPLACE:
                    /// check what line filter is applied for the filter
                    switch (result.command[i].line_range) {
                        /// word containing filter
                        case LINE_RANGE_CONTAINS:
                            /// check if the line contains the pattern
                            if (string_contains_pattern(buffer, result.command[i].arg1)) {
                                strcpy(buffer, replace_word(buffer, result.command[i].arg2, result.command[i].arg3));
                            }
                            break;
                            /// line number filter
                        case LINE_RANGE_START_END:
                            /// check if the line number is in the range
                            if (line_number >= result.command[i].line_number1 &&
                                line_number <= result.command[i].line_number2) {
                                strcpy(buffer, replace_word(buffer, result.command[i].arg2, result.command[i].arg3));
                            }
                            break;
                            /// no filter
                        case LINE_RANGE_NONE:
                            /// apply the filter without any condition
                            strcpy(buffer, replace_word(buffer, result.command[i].arg2, result.command[i].arg3));
                            break;
                    }
                    break;
            }
        }
        /// print the line, if it is not empty
        if (strlen(buffer) > 0) {
            /// print each char one by one except for "\n" char
            for (int i = 0; i < strlen(buffer); ++i) {
                if (buffer[i] != '\n'){
                    printf("%c", buffer[i]);
                }
            }
            /// print the new line char
            printf("\n");
        }
        /// increment the line number
        line_number++;
        /// read the next line
    } while (getline(&buffer, &getline_size, input) > 0);

}
