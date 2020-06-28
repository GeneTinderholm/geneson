#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "geneson.h"

void assert(bool test, char* message) {
    if(!test) {
        fprintf(stderr, "%s\n", message);
    }
}

void test_strings() {
    GeneSON* test_string = parse_json("\"test string\"");
    char* real_string = geneson_get_string(test_string);
    assert(strcmp("test string", real_string) == 0, "Should be able to parse a string");
}

void test_numbers() {
    GeneSON* test_string = parse_json("4.8213");
    float real_number = geneson_get_number(test_string);
    assert(real_number > 4.8212 && real_number < 4.8214, "Should be able to parse a number");
}

void test_bools() {
    GeneSON* test_string = parse_json("true");
    bool real_bool = geneson_get_bool(test_string);
    assert(real_bool == true, "Should be able to parse a bool");

    GeneSON* test_string2 = parse_json("false");
    bool real_bool2 = geneson_get_bool(test_string2);
    assert(real_bool2 == false, "Should be able to parse a bool");
}

void test_nulls() {
    GeneSON* test_string = parse_json("null");
    void* real_null = geneson_get_null(test_string);
    assert(real_null == NULL, "Should be able to parse a null");
}

int main (void) {
    test_strings();
    test_numbers();
    test_bools();
    test_nulls();
}
