#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "geneson.h"

void assert(bool test, char* message) {
    char* prefix;
    if(!test) {
        prefix = "FAILED %s\n";
    } else {
        prefix = "PASSED %s\n";
    }
    fprintf(stderr, prefix, message);
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

void test_objects() {
    GeneSON* test_obj = parse_json("{\"a\":\"b\"}");
    GeneSON* value = geneson_get_object_key(test_obj, "a");
    char* string = geneson_get_string(value);
    assert(strcmp(string, "b") == 0, "Should be able to grab key from object 1");

    // TODO arrays
    GeneSON* test_obj2 = parse_json("{\"a\":[1, 2, 3]}");

    GeneSON* test_obj3 = parse_json("{\"a\":\"b\", \"c\": \"d\"}");
    GeneSON* value3 = geneson_get_object_key(test_obj, "a");
    char* string_a = geneson_get_string(value3);
    assert(strcmp(string_a, "b") == 0, "Should be able to grab key a from object 2");
    char* string_c = geneson_get_string(geneson_get_object_key(test_obj3, "c"));
    assert(strcmp(string_c, "d") == 0, "Should be able to grab key c from object 2");

    GeneSON* test_obj4 = parse_json("{\"a\":{\"b\":\"c\"}}");
    GeneSON* value4 = geneson_get_object_key(geneson_get_object_key(test_obj4, "a"), "b");
    char* string_4 = geneson_get_string(value4);
    assert(strcmp(string_4, "c") == 0, "Should be able to grab key from nested object");

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
    puts("Start Tests");
    test_strings();
    test_numbers();
    test_bools();
    test_nulls();
    test_objects();
    puts("End Tests");
}
