#include "TLVConverter.h"
#include <iostream>

// tag only - zero length data
void TLV::WriteTag(ostream& os, __int8 tag)
{
    // write tag part
    os.write(reinterpret_cast<char*>(&tag), 1);

    // length part
    __int8 len = 0;
    os.write(reinterpret_cast<char*>(&len), sizeof(__int8));
}

// specific case - string serialization
void TLV::WriteString(ostream& os, string str)
{
    // write tag part
    __int8 type = Json::stringValue;
    os.write(reinterpret_cast<char*>(&type), 1);

    // first byte of length
    __int8 len = str.size() >= 0xFF ? 0xFF : static_cast<__int8>(str.size());
    os.write(reinterpret_cast<char*>(&len), sizeof(__int8));
    if (str.size() >= 0xFF)
    {
        // if string length more than 255 chars - use 3 bytes for length part
        __int16 len16 = static_cast<__int16>(str.size());
        os.write(reinterpret_cast<char*>(&len16), sizeof(__int16));
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