#ifndef __H_GENESON__
#define __H_GENESON__

enum GeneSONType {
    GeneSONNull,
    GeneSONBoolean,
    GeneSONNumber,
    GeneSONString,
    GeneSONObject,
    GeneSONArray
};

union GeneSONValue {
    int i;
    float f;
    void* v;
};

typedef struct {
    enum GeneSONType type;
    union GeneSONValue value;
} GeneSON;

GeneSON* parse_json(char* json_string);
char* geneson_get_string(GeneSON* json);
float geneson_get_number(GeneSON* json);
bool geneson_get_bool(GeneSON* json);
void* geneson_get_null(GeneSON* json);

#endif
