# GeneSON

Small json parsing library written in C.

# Rationale

Why do we need another json parsing library?

We probably don't, but I'm learning C, and it seemed like a reasonable project to take up a couple of weekends.

# Planned Interface

- parse_json(char * json_string) - In progress
- pretty_print(GeneSON * obj) - TODO
- serialize(GeneSON * obj) - TODO
- get/set object key (get complete)
- push/pop shift/unshift array

# Status

- [x] primitives handled
- [x] parse number object key/value pairs
- [x] parse object key and value
- [x] parse number of array elements
- [ ] parse array element

# Next steps

Use linked list for object/arrays, it's going to be a lot easier once we start mutating things, also I believe it will let me parse this in one pass rather than the two I'm currently doing.
