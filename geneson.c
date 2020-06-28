#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "geneson.h"

struct StackNode {
    char delimiter;
    struct StackNode* prev;
};

int find_first_char(char* string);
int find_next_quote_position(char* string);
int validate_json(char* string);
int find_next_delimiter(char* string);
char get_complementary_delimiter(char c);
bool is_open_delimiter(char c);
bool is_close_delimiter(char c);
bool is_bool(char* string);
bool is_number(char* string);
bool is_null(char* string);
bool is_string(char* string);
bool is_valid_number_char(char c);
bool str_cmp_no_null(char* string, char* sub_string);
struct StackNode* stack_push(struct StackNode* stack, struct StackNode* node);
struct StackNode* stack_pop(struct StackNode* stack);
GeneSON* handle_primative(char* string);
GeneSON* handle_bool(char* string);
GeneSON* handle_number(char* string);
GeneSON* handle_string(char* string);
GeneSON* handle_null(char* string);
GeneSON* handle_object(char* string, int ending_index);
GeneSON* handle_array(char* string, int ending_index);

GeneSON* parse_json(char* json_string) {
    int i = find_first_char(json_string);

    if (!is_open_delimiter(json_string[i])) {
        return handle_primative(&json_string[i]);
    }

    int ending_index = validate_json(&json_string[i]);

    if (ending_index == -1)
        return NULL;

    if (json_string[i] == '{') {
        return handle_object(&json_string[i], ending_index);
    }

    /* while (json_string[i] != '\0') { */
    /*     if (isspace(json_string[i])) { */
    /*         continue; */
    /*     } */
    /*     if (is_open_delimiter(json_string[i])) { */
    /*     } */
    /* } */

    return NULL;
}

char* geneson_get_string(GeneSON* json) {
    return (char *)json->value.v;
}

float geneson_get_number(GeneSON* json) {
    return json->value.f;
}

bool geneson_get_bool(GeneSON* json) {
    return (bool) json->value.i;
}
void* geneson_get_null(GeneSON* json) {
    return json->value.v;
}

GeneSON* handle_primative(char* string) {
    if (is_bool(string)) {
        return handle_bool(string);
    }
    if (is_number(string)) {
        return handle_number(string);
    }
    if (is_string(string)) {
        return handle_string(string);
    }
    if (is_null(string)) {
        return handle_null(string);
    }
    return NULL;
}

GeneSON* handle_bool(char* string) {
    GeneSON* result = malloc(sizeof(GeneSON));
    union GeneSONValue value;
    result->value = value;
    result->type = GeneSONBoolean;

    if(*string == 't') {
        value.i = 1;
        return result;
    }

    result->value.i = 0;
    return result;
}

GeneSON* handle_number(char* string) {
    float f = atof(string);

    GeneSON* result = malloc(sizeof(GeneSON));
    union GeneSONValue value;
    result->value = value;
    result->type = GeneSONNumber;
    result->value.f = f;

    return result;
}

GeneSON* handle_string(char* string) {
    ++string;
    int str_len = find_next_quote_position(string);
    char* internal_string = malloc(sizeof(char) * (str_len + 1));
    for (int i = 0; i < str_len; ++i)
        internal_string[i] = string[i];
    internal_string[str_len] = '\0';

    GeneSON* result = malloc(sizeof(GeneSON));
    union GeneSONValue value;
    result->value = value;
    result->type = GeneSONString;
    result->value.v = internal_string;

    return result;
}

GeneSON* handle_null(char* string) {
    GeneSON* result = malloc(sizeof(GeneSON));
    union GeneSONValue value;
    result->value = value;
    result->type = GeneSONNull;
    result->value.v = NULL;

    return result;
}

bool is_bool(char* string) {
    return str_cmp_no_null(string, "true") || str_cmp_no_null(string, "false");
}

bool is_null(char* string) {
    return str_cmp_no_null(string, "null");
}

bool is_number(char* string) {
    return is_valid_number_char(*string);
}

bool is_valid_number_char(char c) {
    return (c >= '0' && c <= '9') || c == '.';
}

bool is_string(char* string) {
    return *string == '"';
}

bool str_cmp_no_null(char* string, char* sub_string) {
    int i = 0;
    while (sub_string[i] != '\0') {
        if (sub_string[i] != string[i])
            return false;
        ++i;
    }
    return true;
}

int find_next_quote_position(char* string) {
    int i = 0;
    while (string[i] != '"') {
        if (string[i] == '\0') {
            return -1;
        }
        ++i;
    }
    return i;
}

int find_first_char(char* string) {
    int i = 0;
    while (string[i] != '\0') {
        if (isspace(string[i]))
            continue;
        else
            return i;
        ++i;
    }
    return -1;
}

bool is_open_delimiter(char c) {
    return c == '{' || c == '[';
}

bool is_close_delimiter(char c) {
    return c == '}' || c == ']';
}

int find_next_delimiter(char* string) {
    int i = 0;
    while (string[i] != '\0') {
        char c = string[i];
        if (c == '{' || c == '}' || c == '[' || c == ']' || c == '"')
            return i;
        ++i;
    }
    return -1;
}

char get_complementary_delimiter(char c) {
    switch (c) {
        case '}':
            return '{';
        case ']':
            return '[';
        case '{':
            return '}';
        case '[':
            return ']';
        case '"':
            return '"';
        default:
            return -1;
    }
}

struct StackNode* stack_push(struct StackNode* stack, struct StackNode* node) {
    node->prev = stack;
    return node;
}

struct StackNode* stack_pop(struct StackNode* stack) {
    struct StackNode* new_stack = stack->prev;
    free(stack);

    return new_stack;
}


int validate_json(char* string) {
    struct StackNode* stack = malloc(sizeof(struct StackNode));
    stack->prev = NULL;
    stack->delimiter = *string;
    int i = 0;

    while (stack != NULL) {
        ++i;
        int offset = find_next_delimiter(&string[i]);

        if (offset == -1) {
            return -1;
        }

        i += offset;

        if (is_open_delimiter(string[i])) {
            struct StackNode* node = malloc(sizeof(struct StackNode));
            node->delimiter = string[i];
            stack = stack_push(stack, node);
        } else if (is_close_delimiter(string[i])) {
            if (stack->delimiter != get_complementary_delimiter(string[i])) {
                return -1;
            }
            stack = stack_pop(stack);
        } else {
            // string[i] is "
            ++i;
            // TODO support escaped quotes
            int next_quote = find_next_quote_position(&string[i]);
            if (next_quote == -1) {
                return -1;
            }
            i+= next_quote;
        }
    }

    return i;
}

GeneSON* handle_object(char* string, int ending_index) {
    int number_of_kv_pairs = 0,
        i = 0;

    while (i < ending_index) {
        ++i;
        int offset = find_first_char(&string[i]);
        i += offset;

        if (string[i] == '}') {
            break;
        }

        // start of key
        if (string[i] != '"') {
            return NULL;
        }
        // don't pass in a quote to find_next_quote_position, it will always be 0
        ++i;
        offset = find_next_quote_position(&string[i]);

        if (offset == -1) {
            return NULL;
        }

        // end of key
        i += offset;

        // find colon
        // find first char
        // handle each possible case
        // find comma if exists
        // increment i and number_of_kv_pairs
    }

    return NULL; // TODO change
}
