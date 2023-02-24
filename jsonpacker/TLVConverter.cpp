#include "TLVConverter.h"
#include <iostream>

// tag only - zero length data
void TLV::WriteTag(ostream& os, int8_t tag)
{
    // write tag part
    os.write(reinterpret_cast<char*>(&tag), 1);

    // length part
    int8_t len = 0;
    os.write(reinterpret_cast<char*>(&len), sizeof(int8_t));
}

// specific case - string serialization
void TLV::WriteString(ostream& os, string str)
{
    // write tag part
    int8_t type = Json::stringValue;
    os.write(reinterpret_cast<char*>(&type), 1);

    // first byte of length
    int8_t len = str.size() >= 0xFF ? 0xFF : static_cast<int8_t>(str.size());
    os.write(reinterpret_cast<char*>(&len), sizeof(int8_t));
    if (str.size() >= 0xFF)
    {
        // if string length more than 255 chars - use 3 bytes for length part
        int16_t len16 = static_cast<int16_t>(str.size());
        os.write(reinterpret_cast<char*>(&len16), sizeof(int16_t));
    }

    // write data part
    os.write(str.c_str(), str.size());
}

void TLV::WriteJson(ostream& str, Json::Value value)
{
    switch (value.type())
    {
    case Json::intValue:
        Write(str, value.type(), value.asInt());
        break;
    case Json::uintValue:
        Write(str, value.type(), value.asUInt());
        break;
    case Json::booleanValue:
        Write(str, value.type(), value.asBool());
        break;
    case Json::realValue:
        Write(str, value.type(), value.asDouble());
        break;
    case Json::stringValue:
        WriteString(str, value.asString());
        break;
    }
};