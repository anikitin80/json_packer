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

        FirstTagType = 1,
        LastTagType = DictionaryStart
    };

    // tag only - zero length data
    bool WriteTag(ostream& os, int8_t tag);
    bool ReadTag(istream& os, int8_t& tag);
    
    // string serialization
    bool WriteString(ostream& os, string str);
    bool ReadString(istream& os, string& str, bool bReadTagPart = true);
    
    // Json::Value serialization
    bool WriteJson(ostream& os, Json::Value value);
    bool ReadJson(istream& os, Json::Value& value);

    // serialization of common types: int, double, bool
    template<typename ValType>
    bool Write(ostream& os, int8_t tag, ValType value)
    {
        // write tag part
        os.write(reinterpret_cast<char*>(&tag), 1);

        // length part
        int8_t len = sizeof(ValType);
        os.write(reinterpret_cast<char*>(&len), sizeof(int8_t));

        //value part
        os.write(reinterpret_cast<char*>(&value), len);
        return true;
    }

    template<typename ValType>
    bool ReadValue(istream& os, ValType& value)
    {
        // length part
        int8_t len = 0;
        os.read(reinterpret_cast<char*>(&len), sizeof(int8_t));

        if(sizeof(ValType) != len)
        {
            std::cout << "ReadValue: incorrect value type";
            return false;
        }

        //value part
        os.read(reinterpret_cast<char*>(&value), len);
        return true;
    }
};

