#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "geneson.h"

struct StackNode {
    char delimiter;
    struct StackNode* prev;
};

int find_first_char(char* string);
int find_next_quote_position(char* string);
bool is_open_delimiter(char c);
bool is_bool(char* string);
bool is_number(char* string);
bool is_null(char* string);
bool is_string(char* string);
bool is_valid_number_char(char c);
bool str_cmp_no_null(char* string, char* sub_string);
GeneSON* handle_primative(char* string);
GeneSON* handle_bool(char* string);
GeneSON* handle_number(char* string);
GeneSON* handle_string(char* string);
GeneSON* handle_null(char* string);

GeneSON* parse_json(char* json_string) {
    int i = find_first_char(json_string);
    struct StackNode stack;

    if (!is_open_delimiter(json_string[i])) {
        return handle_primative(&json_string[i]);
    }

    while (json_string[i] != '\0') {
        if (isspace(json_string[i])) {
            continue;
        }
        if (is_open_delimiter(json_string[i])) {
        }
    }

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
    while (string[i] != '"')
        ++i;
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
