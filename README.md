# UselessJSON
A simple JSON extension for GameMaker 8.2.

## Features
- Fast JSON parsing and decoding.
- dot-path navigation for nested values.

## Quick Start
```gml
// Parse a JSON string into memory
var json_str = '{"name": "Hero", "hp": 100}';
var handle = json_parse(json_str);

// Access values directly using the handle
var _name = json_get_string(handle, "name");
var _hp = json_get_number(handle, "hp");

// Clean up the parsed JSON handle from memory
json_delete(handle);
```