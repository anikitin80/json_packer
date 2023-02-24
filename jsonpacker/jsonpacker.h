#pragma once

#include <iostream>
#include <fstream>
#include "json/json.h"
#include <unordered_map>

// class CJsonPacker
// responsible for Json-TLV conversion
class CJsonPacker
{
public:
    CJsonPacker();
    ~CJsonPacker();

    bool EncodeFile(std::string filePath, std::string outFilePath="");
    void Clear();

protected:

    int DoProcessRecords();
    __int16 GetDictionaryId(const char *key);    // get Id or create new entry in the dictionary
    bool SerializeRecord(const Json::Value& record);
    void SerializeDictionary();

    // input and output files data
    std::string InFilePath;
    std::string OutFilePath;

    std::ifstream InStream;
    std::ofstream OutStream;

    // dictionary
    std::unordered_map<std::string, __int16> Dictionary;
    __int16 CurrentDictId = 0;
};

