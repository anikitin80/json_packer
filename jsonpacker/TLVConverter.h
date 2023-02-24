#pragma once

#include <iosfwd>
#include "json/json.h"

using namespace std;

// This code implements serialization of Json::Value in TLV format: Tag-Length-Value
// Tag [1 byte]       - object type
// Length [1|3 bytes] - length of the value in bytes: 
//      first byte value between 0 and 0xFE means length of the Value part between 0 and 254
//      first byte value 0xFF means that next two bytes contain length of the value part between 255 and 65535
// Value [L bytes]    - contains main data (if L > 0)
namespace TLV
{
    enum TagType
    {
        // use values 1-9 as tags for Json types
        FirstJsonType   = 1,
        LastJsonType    = 9,
        Identity        = 10,  // 16-bit integer for dictionary id
        RecordEnd       = 11,  // specific tag for records separation
        DictionaryStart = 12,  // specific tag which marks start of dictionary section
    };

    void WriteTag(ostream& os, int8_t tag);           // tag only - zero length data
    void WriteString(ostream& os, string str);        // string serialization
    void WriteJson(ostream& os, Json::Value value);   // Json::Value serialization

    // serialization of common types: int, double, bool
    template<typename ValType>
    void Write(ostream& str, int8_t tag, ValType value)
    {
        // write tag part
        str.write(reinterpret_cast<char*>(&tag), 1);

        // length part
        int8_t len = sizeof(ValType);
        str.write(reinterpret_cast<char*>(&len), sizeof(int8_t));

        //value part
        str.write(reinterpret_cast<char*>(&value), len);
    }
};