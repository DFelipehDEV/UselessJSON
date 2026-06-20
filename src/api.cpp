#define _CRT_SECURE_NO_WARNINGS
#include "../useless.hpp"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/pointer.h"
#include <unordered_map>
#include <cstdio>
#include <string>

using namespace rapidjson;

static std::unordered_map<int, Document*> json_objects;
static int json_handle_counter = 0;
static std::string result_buffer;

inline Document* getJsonObject(double handle) {
    auto it = json_objects.find(static_cast<int>(handle));
    return (it != json_objects.end()) ? it->second : nullptr;
}

inline Value* getDotPath(Value* val, const char* path) {
    if (!val || !path) return nullptr;
    
    std::string p = "/";
    for (const char* c = path; *c; ++c) p += (*c == '.') ? '/' : *c;
    return Pointer(p.c_str()).Get(*val);
}

GMREAL json_load_file(const char* filepath) {
    FILE* fp = std::fopen(filepath, "rb");
    if (!fp) return -1.0;
    
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    
    Document* doc = new Document();
    doc->ParseStream(is);
    std::fclose(fp);
    
    if (doc->HasParseError()) {
        delete doc;
        return -1.0;
    }
    
    json_objects[json_handle_counter] = doc;
    return static_cast<double>(json_handle_counter++);
}

GMREAL json_parse(const char* json_string) {
    Document* doc = new Document();
    doc->Parse(json_string);
    
    if (doc->HasParseError()) {
        delete doc;
        return -1.0;
    }
    
    json_objects[json_handle_counter] = doc;
    return static_cast<double>(json_handle_counter++);
}

GMSTRING json_stringify(double handle) {
    Document* doc = getJsonObject(handle);
    if (!doc) return const_cast<char*>("");
    
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc->Accept(writer);
    
    result_buffer = buffer.GetString();
    return const_cast<char*>(result_buffer.c_str());
}

GMREAL json_get_number(double handle, const char* key) {
    Document* doc = getJsonObject(handle);
    if (doc && doc->IsObject() && doc->HasMember(key)) {
        auto& val = (*doc)[key];
        if (val.IsNumber()) return val.GetDouble();
    }
    return 0.0;
}

GMSTRING json_get_string(double handle, const char* key) {
    Document* doc = getJsonObject(handle);
    if (doc && doc->IsObject() && doc->HasMember(key)) {
        auto& val = (*doc)[key];
        if (val.IsString()) {
            result_buffer = val.GetString();
            return const_cast<char*>(result_buffer.c_str());
        }
    }
    return const_cast<char*>("");
}

GMREAL json_set_number(double handle, const char* key, double value) {
    Document* doc = getJsonObject(handle);
    if (!doc || !doc->IsObject()) return 0.0;
    
    if (!doc->HasMember(key)) {
        doc->AddMember(Value(key, doc->GetAllocator()).Move(), Value(value).Move(), doc->GetAllocator());
    } else {
        (*doc)[key] = value;
    }
    return 1.0;
}

GMREAL json_set_string(double handle, const char* key, const char* value) {
    Document* doc = getJsonObject(handle);
    if (!doc || !doc->IsObject()) return 0.0;
    
    if (!doc->HasMember(key)) {
        doc->AddMember(Value(key, doc->GetAllocator()).Move(), Value(value, doc->GetAllocator()).Move(), doc->GetAllocator());
    } else {
        (*doc)[key] = Value(value, doc->GetAllocator());
    }
    return 1.0;
}

GMREAL json_has_key(double handle, const char* key) {
    Document* doc = getJsonObject(handle);
    return (doc && doc->IsObject() && doc->HasMember(key)) ? 1.0 : 0.0;
}

GMREAL json_delete(double handle) {
    int h = static_cast<int>(handle);
    auto it = json_objects.find(h);
    if (it != json_objects.end()) {
        delete it->second;
        json_objects.erase(it);
        return 1.0;
    }
    return 0.0;
}

GMSTRING json_dotget_string(double handle, const char* path) {
    Document* doc = getJsonObject(handle);
    if (!doc) return const_cast<char*>("");
    
    Value* val = getDotPath(doc, path);
    if (val && val->IsString()) {
        result_buffer = val->GetString();
        return const_cast<char*>(result_buffer.c_str());
    }
    return const_cast<char*>("");
}

GMREAL json_dotget_number(double handle, const char* path) {
    Document* doc = getJsonObject(handle);
    if (!doc) return 0.0;
    
    Value* val = getDotPath(doc, path);
    if (val && val->IsNumber()) {
        return val->GetDouble();
    }
    return 0.0;
}

GMSTRING json_get_keys(double handle) {
    Document* doc = getJsonObject(handle);
    if (!doc || !doc->IsObject()) return const_cast<char*>("");
    
    result_buffer = "";
    for (auto& member : doc->GetObject()) {
        if (!result_buffer.empty()) result_buffer += ",";
        result_buffer += member.name.GetString();
    }
    return const_cast<char*>(result_buffer.c_str());
}

GMREAL json_is_array(double handle, const char* key) {
    Document* doc = getJsonObject(handle);
    if (doc && doc->IsObject() && doc->HasMember(key)) {
        return (*doc)[key].IsArray() ? 1.0 : 0.0;
    }
    return 0.0;
}

GMREAL json_array_length(double handle, const char* key) {
    Document* doc = getJsonObject(handle);
    if (doc && doc->HasMember(key)) {
        auto& val = (*doc)[key];
        if (val.IsArray()) return static_cast<double>(val.Size());
    }
    return 0.0;
}

GMSTRING json_array_get_string(double handle, const char* key, double index) {
    Document* doc = getJsonObject(handle);
    if (doc && doc->HasMember(key)) {
        auto& arr = (*doc)[key];
        if (arr.IsArray()) {
            int idx = static_cast<int>(index);
            if (idx >= 0 && idx < static_cast<int>(arr.Size()) && arr[idx].IsString()) {
                result_buffer = arr[idx].GetString();
                return const_cast<char*>(result_buffer.c_str());
            }
        }
    }
    return const_cast<char*>("");
}

GMREAL json_array_get_number(double handle, const char* key, double index) {
    Document* doc = getJsonObject(handle);
    if (doc && doc->HasMember(key)) {
        auto& arr = (*doc)[key];
        if (arr.IsArray()) {
            int idx = static_cast<int>(index);
            if (idx >= 0 && idx < static_cast<int>(arr.Size()) && arr[idx].IsNumber()) {
                return arr[idx].GetDouble();
            }
        }
    }
    return 0.0;
}