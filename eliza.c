// Found on hackchina.com
// Tidied up a little by David Bolton, http://cplus.about.com
// Tidied up even more by Timm Knape, http://www.knp.de
// This is in the public domain

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* CONFIGURATION
 ****************/

#define MAX_INPUT_BUFFER_SIZE 80
#define MAX_REMEMBERED_LINES_COUNT 100
#define MAX_TOKEN_SIZE 80

const int SHORT_ANSWER_LENGTH = 10;
const int VERY_SHORT_ANSWER_LENGTH = 2;


/* GENERIC RESPONSES
 ********************/

void print_generic_response() {

    static char *generic_responses[] = {
        "How are you this beautiful day?",
        "Did you have a happy childhood?",
        "Did you hate your father?",
        "Did you have a good friend?",
        "Did you like your friend?",
        "What do you think about your friendship?",
        "I'm not sure I understand.",
        NULL
    };

    static int next_generic_response = 0;

    if (!generic_responses[next_generic_response]) next_generic_response = 0;
    
    puts(generic_responses[next_generic_response++]);
}


/* FABRICATED ANSWERS
 *********************/

char *lookup_fabricated_answer(char *token) {

    static char *fabricated_answers[] = {
        "no", "Tell me more about the detail.",
        "yes", "Tell me more about the detail.",
        "fuck", "Don't be so rude again!",
        "shet", "Don't be so rude again!",
        "you", "Let't not talk about me.",
        "think", "Why do you think that?",
        "hate", "So you hate something -- tell me more.",
        "what", "Why do you ask?",
        "want", "Why do you want that?",
        "need", "We all need many things -- is this special?",
        "why", "Remember, therapy is good for you.",
        "know", "How do you know that?",
        "bye", "Your bill will be mailed to you.",
        "murder", "I don't like killing.",
        "kill", "It is wrong to kill.",
        "jerk", "Don't ever call me a jerk!",
        "can't", "Don't be negative -- be positive.",
        "failure", "Strive for success .",
        "never", "Don't be negative -- be positive.",
        "unhappy", "Why are you unhappy?",
        "beautiful", "I'm sure you like her, don't you?",
        "like", "Why do you like that?",
        "love", "Remember, love everthing what you love.",
        NULL
    };

    int index = 0;
    while (fabricated_answers[index]) {
        if (!strcmp(fabricated_answers[index], token)) return fabricated_answers[index + 1];
        index += 2;
    }
    return NULL;
}


/* REMEMBERED LINES
 *******************/

char remembered_lines[MAX_REMEMBERED_LINES_COUNT][MAX_INPUT_BUFFER_SIZE];
int next_remembered_line_enter_index = 0;
int next_remembered_line_return_index = 0;

void remember_line(char *line) {
    if (next_remembered_line_enter_index == MAX_REMEMBERED_LINES_COUNT) {
        next_remembered_line_enter_index = 0;
    }
    strncpy(remembered_lines[next_remembered_line_enter_index], line, MAX_INPUT_BUFFER_SIZE);
    ++next_remembered_line_enter_index;
}

char *get_old_remembered_line() {
    if (next_remembered_line_enter_index != next_remembered_line_return_index)  {
        int result = next_remembered_line_return_index;
        ++next_remembered_line_return_index;
        if (next_remembered_line_return_index == MAX_REMEMBERED_LINES_COUNT) {
            next_remembered_line_return_index = 0;
        }
        return remembered_lines[result];
    }
    return NULL;
}

bool is_line_remebered(char *line) {
    for (int index = 0; index < MAX_REMEMBERED_LINES_COUNT; ++index) {
        if (!strcmp(line, remembered_lines[index]))  return true;
    }
    return false;
}


/*
 * TOKENIZER
 ************/

    char *position_in_input;

char *next_token() {

    static char token[MAX_TOKEN_SIZE];
    
    char *position_in_token = token;
    
    /* skip spaces */
    while (*position_in_input && *position_in_input <= ' ') ++position_in_input;
    
    if (!*position_in_input) { // end of input
        return NULL;
    }
    
    if (strchr(".,;!?", *position_in_input)) { // return punctuation
        *position_in_token++ = *position_in_input++;
        *position_in_token = 0;
        return token;
    }
    
    // read word
    while (*position_in_input && !strchr(" \n\r.,;?!", *position_in_input))  {
        if (position_in_token != token + MAX_TOKEN_SIZE - 2) {
            *position_in_token++ = tolower(*position_in_input++);
        } else {
            ++position_in_input;
        }
    }
    *position_in_token = 0;
    return token;
}


/*
 * CREATE THE DOCTOR'S RESPONE
 ******************************/

void respond(char *input) {
    if (strlen(input) < VERY_SHORT_ANSWER_LENGTH && strcmp(input,"bye"))  { // short answer
        char *remebered = get_old_remembered_line();
        if (remebered) {
            printf("You just said: %s\n", remebered);
            puts("Tell me more.");
        }
        else {
            print_generic_response();
        }
        return;
    }
    
    if (is_line_remebered(input))  {
        puts("Stop repeating yourself!");
        return ;
    }
    
    if (strlen(input) > SHORT_ANSWER_LENGTH) { // remeber line
        remember_line(input);
    }

    position_in_input = input;
    
    for (;;) {
        char *token = next_token();
        if (!token) break;
        char *fabricated_answer = lookup_fabricated_answer(token);
        if (fabricated_answer) {
            puts(fabricated_answer);
            return;
        }
    }
    
    // comment of last resort
    if (strlen(input) > SHORT_ANSWER_LENGTH) {
        puts("It's seem intersting, tell me more ...");
    } else {
        puts("Tell me more ...");
    }
}

int main(int argc, char **argv) {
    char input_buffer[MAX_INPUT_BUFFER_SIZE];
    print_generic_response();
    do {
        printf(": ");
        fgets(input_buffer, sizeof(input_buffer), stdin);
        respond(input_buffer);
    } while (strnstr(input_buffer,"bye", MAX_INPUT_BUFFER_SIZE) != input_buffer);
}
