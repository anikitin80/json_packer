#include "jsonpacker.h"
#include "TLVConverter.h"

using namespace std;

CJsonPacker::CJsonPacker()
{

}

CJsonPacker::~CJsonPacker()
{
    Clear();
}

void CJsonPacker::Clear()
{
    if (InStream)
        InStream.close();
    if (OutStream)
        OutStream.close();
    Dictionary.clear();
    InFilePath.clear();
    OutFilePath.clear();
    CurrentDictId = 0;    
}

bool CJsonPacker::EncodeFile(std::string filePath, std::string outFilePath /*= ""*/)
{
    Clear();

    // try open input file
    InStream.open(filePath.c_str());
    if (!InStream)
    {
        cout << "Failed to open input file " << filePath << ". Error code: " << errno << endl;
        return false;
    }

    InFilePath = filePath;
    OutFilePath = outFilePath;

    // if output file name is empty - use input file name but replace extension with ".jpk"
    if (OutFilePath.empty())
    {
        size_t pos = filePath.rfind('.');
        if (pos != string::npos)
            OutFilePath = filePath.substr(0, pos);
        else
            OutFilePath = filePath;
        OutFilePath += ".jpk";
    }

    // try to open output file
    OutStream.open(OutFilePath.c_str(), fstream::out | fstream::trunc | fstream::binary);
    if (!OutStream)
    {
        cout << "Failed to open output file " << OutFilePath << ". Error code: " << errno << endl;
        InStream.close();
        return false;
    }

    // first encode records
    int nCount = DoProcessRecords();
    cout << "Records processed: " << nCount << endl;
    InStream.close();

    // last step - write dictionary
    if (nCount)
    {
        SerializeDictionary();
        cout << "Dictionary saved: " << Dictionary.size() << " items" << endl;
    }
    else
    {
        cout << "No records to encode" << endl;
    }
        
    OutStream.close();

    if(nCount)
        cout << "Encoding completed successfully. Ouput file: " << OutFilePath << endl;

    return nCount > 0;    
}

int CJsonPacker::DoProcessRecords()
{
    int nLine = 0, nProcessed = 0;

    // read json data
    std::string strJson;
    Json::Reader reader;

    while (getline(InStream, strJson))
    {
        nLine++;

        Json::Value record;
        reader.parse(strJson, record, false);
        if (record.type() != Json::objectValue)
        {
            cout << "Incorrect data format in line " << nLine << " (oject expected)" << endl;
            continue;
        }

        if (SerializeRecord(record))
            nProcessed++;
    }

    return nProcessed;
}

__int16 CJsonPacker::GetDictionaryId(const char* key)
{
    auto iterDict = Dictionary.find(key);
    if (iterDict != Dictionary.end())
        return iterDict->second;

    Dictionary[key] = ++CurrentDictId;
    return CurrentDictId;
}

bool CJsonPacker::SerializeRecord(const Json::Value& record)
{
    // write all fileds of the record as pairs DictId:Value
    // where DictId - is id of the attribute from the dictionary
    for (auto iter = record.begin(); iter != record.end(); iter++)
    {
        TLV::Write(OutStream, TLV::Identity, GetDictionaryId(iter.memberName()));
        TLV::WriteJson(OutStream, record.get(iter.memberName(), ""));
    }

    TLV::WriteTag(OutStream, TLV::RecordEnd);

    return true;
}

void CJsonPacker::SerializeDictionary()
{
    if (Dictionary.empty())
        return;
    
    // write dictionsary at the end of file in the same format of pairs DictId:Value
    TLV::WriteTag(OutStream, TLV::DictionaryStart);
    for (auto iter = Dictionary.begin(); iter != Dictionary.end(); iter++)
    {
        TLV::Write(OutStream, TLV::Identity, iter->second);
        TLV::WriteString(OutStream, iter->first);
    }
}