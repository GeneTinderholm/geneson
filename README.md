# GeneSON

Small json parsing library written in C.

# Rationale

Why do we need another json parsing library?

We probably don't, but I'm learning C, and it seemed like a reasonable project to take up a couple of weekends.

# Planned Interface

- parse_json(char * json_string) - In progress
- pretty_print(GeneSON * obj) - TODO
- serialize(GeneSON * obj) - TODO

# Status

- [x] primitives handled
- [x] parse number object key/value pairs
- [ ] parse object key and value
- [ ] parse number of array elements
- [ ] parse array element

