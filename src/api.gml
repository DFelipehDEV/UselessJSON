#define json_decode
/// json_decode(string)
var _json_str, _handle, _root;
_json_str = argument0;
_handle = json_parse(_json_str);

if (_handle == -1) {
    return -1;
}

_root = json_decode_value(_handle);
json_delete(_handle);

return _root;


#define json_decode_value
/// json_decode_value(handle)
var _handle, _result, _keys, _comma_pos, _key, _is_array;
var _i, _array_len, _values;

_handle = argument0;

_is_array = json_is_array(_handle, "");
if (_is_array) {
    _result = ds_list_create();
    _array_len = json_array_length(_handle, "");
    
    for (_i = 0; _i < _array_len; _i = _i + 1) {
        ds_list_add(_result, json_array_get_string(_handle, "", _i));
    }
    
    return _result;
}

_result = ds_map_create();
_keys = json_get_keys(_handle);
while (_keys != "") {
    _comma_pos = string_pos(",", _keys);
    
    if (_comma_pos == 0) {
        _key = _keys;
        _keys = "";
    } else {
        _key = string_copy(_keys, 1, _comma_pos - 1);
        _keys = string_delete(_keys, 1, _comma_pos);
    }
    
    if (_key == "") continue;
    
    var _val_str, _val_num;
    _val_str  = json_get_string(_handle, _key);
    _val_num = json_get_number(_handle, _key);
    
    if (_val_str != "") {
        ds_map_add(_result, _key, _val_str);
    } else {
        ds_map_add(_result, _key, _val_num);
    }
}

return _result;

