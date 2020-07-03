#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "geneson.h"

struct StackNode {
    char delimiter;
    struct StackNode* next;
};

struct QueueNode {
    int position;
    struct QueueNode* next;
};

struct KeyValuePair {
    char* key;
    GeneSON* value;
};

struct GenericArray {
    int length;
    void* elements;
};

int find_first_char(char* string);
int find_next_quote_position(char* string);
int validate_json(char* string);
int find_next_delimiter(char* string);
int find_next_character(char* string, char c);
int find_one_of_next_two_characters(char* string, char a, char b);
int find_next_comma_or_ending_curly(char* string);
char get_complementary_delimiter(char c);
bool is_open_delimiter(char c);
bool is_close_delimiter(char c);
bool is_bool(char* string);
bool is_number(char* string);
bool is_null(char* string);
bool is_string(char* string);
bool is_valid_number_char(char c);
bool str_cmp_no_null(char* string, char* sub_string);
struct StackNode* stack_push(struct StackNode* head, struct StackNode* next);
struct StackNode* stack_pop(struct StackNode* head);
struct QueueNode* queue_push(struct QueueNode* tail, struct QueueNode* next);
struct QueueNode* queue_pop(struct QueueNode* head);
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

GeneSON* geneson_get_object_key(GeneSON* json, char* key) {
    struct GenericArray* kv_pair_holder = json->value.v;

    for (int i = 0; i < kv_pair_holder->length; ++i) {
        struct KeyValuePair* pairs = (struct KeyValuePair*) kv_pair_holder->elements;
        if (strcmp(key, pairs[i].key) == 0) {
            return pairs[i].value;
        }
    }

    return NULL;
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
    /* int i = 0; */
    /* while (string[i] != '"') { */
    /*     if (string[i] == '\0') { */
    /*         return -1; */
    /*     } */
    /*     ++i; */
    /* } */
    /* return i; */
    return find_next_character(string, '"');
}

int find_next_character(char* string, char c) {
    int i = 0;
    while (string[i] != c) {
        if (string[i] == '\0') {
            return -1;
        }
        ++i;
    }
    return i;
}

int find_next_comma_or_ending_curly(char* string) {
    return find_one_of_next_two_characters(string, ',', '}');
}

int find_one_of_next_two_characters(char* string, char a, char b) {
    int i = 0;
    while (string[i] != a && string[i] != b) {
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
        if (!isspace(string[i])) {
            return i;
        }
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

struct StackNode* stack_push(struct StackNode* head, struct StackNode* next) {
    next->next = head;
    return next;
}

struct StackNode* stack_pop(struct StackNode* head) {
    struct StackNode* next_head = head->next;
    free(head);
    return next_head;
}
struct QueueNode* queue_push(struct QueueNode* tail, struct QueueNode* next) {
    tail->next = next;
    return next;
}
struct QueueNode* queue_pop(struct QueueNode* head) {
    struct QueueNode* next_head = head->next;
    free(head);
    return next_head;
}

int validate_json(char* string) {
    struct StackNode* stack = malloc(sizeof(struct StackNode));
    stack->next = NULL;
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
        i = 0,
        offset = 0;

    struct QueueNode* head = malloc(sizeof(struct QueueNode));
    struct QueueNode* tail = head;

    while (i < ending_index) {
        ++i;
        offset = find_first_char(&string[i]);
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
        offset = find_next_character(&string[i], ':');
        if (offset == -1) {
            return NULL;
        }
        // we want the character after the colon
        i += offset + 1;
        // find first char
        offset = find_first_char(&string[i]);

        i += offset;
        // handle each possible case
        offset = 0;

        // object or array
        if (string[i] == '{' || string[i] == '[') {
            offset = validate_json(&string[i]) + 1; // need to start looking after the end of the nested object
        }
        offset += find_next_comma_or_ending_curly(&string[i + offset]);

        if (offset == -1) {
            return NULL;
        }
        i += offset;

        ++number_of_kv_pairs;

        tail->position = i;

        // we're done
        if (string[i] == '}') {
            break;
        }
        tail = queue_push(tail, malloc(sizeof(struct QueueNode)));
    }

    struct GenericArray* kv_pair_holder = malloc(sizeof(struct GenericArray));
    kv_pair_holder->length = number_of_kv_pairs;

    GeneSON* result = malloc(sizeof(GeneSON));
    union GeneSONValue value;
    result->value = value;
    result->type = GeneSONObject;
    result->value.v = kv_pair_holder;

    if (number_of_kv_pairs == 0) {
        // this is kind of wasteful would handling objects as a linked list of kv pair arrays be a better option? Bring in a map library?
        free(head);
        return result;
    }

    struct KeyValuePair* kv_pair_array = malloc(sizeof(struct KeyValuePair) * number_of_kv_pairs);
    kv_pair_holder->elements = kv_pair_array;

    offset = 0;
    for (int i = 0; i < number_of_kv_pairs; ++i) {
        offset += find_next_quote_position(&string[offset]);
        ++offset;

        // the +1s are to correct for the two off by one errors we get by keeping offset equal to the position of the first quote
        // that position is useful later
        int key_length = find_next_quote_position(&string[offset]);
        int end_of_key = key_length + offset;
        char* key = malloc(sizeof(char) * (key_length + 1));

        // parse out key
        for (int j = 0; j < (key_length); ++j) {
            key[j] = string[offset + j];
            key[key_length] = '\0';
        }
        kv_pair_array[i].key = key;

        offset = end_of_key + 1;
        offset += find_next_character(&string[offset], ':');
        // start after colon
        ++offset;

        GeneSON* value = parse_json(&string[offset]);
        kv_pair_array[i].value = value;

        offset = head->position + 1;
        head = queue_pop(head);
    }

    return result;
}
