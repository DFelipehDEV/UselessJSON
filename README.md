# UselessJSON
A simple JSON extension for GameMaker 8.2.

## Features
- Fast JSON parsing and decoding.
- dot-path navigation for nested values.

## Quick Start
```gml
var _json_str, _handle;
_json_str = '{"player": {"name": "Carlos", "hp": 100}}';
_handle = json_parse(_json_str);

// Access nested values directly using dot-paths
var _name, _hp;
_name = json_dotget_string(_handle, "player.name");
_hp = json_dotget_number(_handle, "player.hp");

show_message("name: " + _name);
show_message("hp: " + string(_hp));

json_delete(_handle);
```