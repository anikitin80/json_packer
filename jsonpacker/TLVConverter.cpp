#include <iostream>
#include "TLVConverter.h"

// tag only - zero length data
bool TLV::WriteTag(ostream& os, int8_t tag)
{
    if(0 == tag)
    {
        cout << "0 is incorrect tag value" << endl;
        return false;
    }

    // write tag part
    os.write(reinterpret_cast<char*>(&tag), 1);

    // length part
    int8_t len = 0;
    os.write(reinterpret_cast<char*>(&len), sizeof(int8_t));

    return true;
}

bool TLV::ReadTag(istream& os, int8_t& tag)
{
    // read tag part
    os.read(reinterpret_cast<char*>(&tag), 1);
    if(0 == tag)
    {
        cout << "0 is incorrect tag value" << endl;
        return false;
    }

    // length part
    int8_t len = 0;
    os.read(reinterpret_cast<char*>(&len), sizeof(int8_t));
    if(0 != len)
    {
        cout << "length should be 0, otherwise another method (not ReadTag) should be used" << endl;
        return false;
    }

    return true;
}

// specific case - string serialization
bool TLV::WriteString(ostream& os, string str)
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
    return true;
}

bool TLV::ReadString(istream& os, string& str, bool bReadTagPart /*= true*/)
{
    // read tag part
    if(bReadTagPart)
    {
        int8_t type = 0;
        os.read(reinterpret_cast<char*>(&type), 1);
        if(type != Json::stringValue)
        {
            cout << "wrong tag value: Json::stringValue expected" << endl;
            return false;
        }
    }

    uint8_t len = 0;
    uint16_t len16 = 0;
    // first byte of length
    os.read(reinterpret_cast<char*>(&len), sizeof(int8_t));
    if(len == 0xFF) // if first byte contains 0xFF - read next two bytes to get length
        os.read(reinterpret_cast<char*>(&len16), sizeof(int16_t));

    // allocate memory and read
    str.resize(len == 0xFF? len16 : len);
    os.read(&str[0], len == 0xFF? len16 : len);

    return true;
}

bool TLV::WriteJson(ostream& str, Json::Value value)
{
    switch (value.type())
    {
    case Json::intValue:
        return Write(str, value.type(), value.asInt());
    case Json::uintValue:
        return Write(str, value.type(), value.asUInt());
    case Json::booleanValue:
        return Write(str, value.type(), value.asBool());
    case Json::realValue:
        return Write(str, value.type(), value.asDouble());
    case Json::stringValue:
        return WriteString(str, value.asString());
    }

    return false;
};

bool TLV::ReadJson(istream& os, Json::Value& value)
{
    // read tag part
    int8_t tag = Json::nullValue;
    os.read(reinterpret_cast<char*>(&tag), 1);

    if(tag == Json::nullValue)
        return false;

    //  read value part
    bool bRes = false;
    switch (tag)
    {
    case Json::intValue:
    {
        Json::Value::Int intVal = 0;
        bRes = ReadValue(os, intVal);
        value = Json::Value(intVal);
        break;
    }
    case Json::uintValue:
    {
        Json::Value::UInt uintVal = 0;
        bRes = ReadValue(os, uintVal);
        value = Json::Value(uintVal);
        break;
    }
    case Json::booleanValue:
    {
        bool bVal = false;
        bRes = ReadValue(os, bVal);
        value = Json::Value(bVal);
        break;
    }
    case Json::realValue:
    {
        double dVal = false;
        bRes = ReadValue(os, dVal);
        value = Json::Value(dVal);
        break;
    }
    case Json::stringValue:
    {
        string strVal;
        bRes = ReadString(os, strVal, false);
        value = Json::Value(strVal);
        break;
    }
    }

    return bRes;
};