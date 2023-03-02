#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "TLVConverter.h"

const string fileName = "test.bin"; // file for serialization tests

TEST(TLVConverter, ReadWriteTag) 
{
    ofstream os(fileName, ios::trunc|ios::binary);
    std::vector<int8_t> tagsList;

    EXPECT_FALSE(TLV::WriteTag(os, 0));
    for(uint8_t tag = TLV::FirstTagType; tag <= TLV::LastTagType; tag++) 
    {
        EXPECT_TRUE(TLV::WriteTag(os, tag));
    }
    os.close();

    ifstream is(fileName, ios::binary);
    for(int8_t tag : tagsList)
    {
        int8_t tagRead = 0;
        EXPECT_TRUE(TLV::ReadTag(is, tagRead));
        EXPECT_EQ(tagRead, tag);
    }
    is.close();
    remove(fileName.c_str());
}

TEST(TLVConverter, ReadWriteString) 
{
    std::vector<std::string> stringsList = { "", "test", "test test test", "!@#$%^&*()_+=-",
        "very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string"
        };

    ofstream os(fileName, ios::trunc|ios::binary);
    for(string str: stringsList) 
    {
        EXPECT_TRUE(TLV::WriteString(os, str));
    }
    os.close();

    ifstream is(fileName, ios::binary);
    for(string str: stringsList) 
    {
        string strRead;
        EXPECT_TRUE(TLV::ReadString(is, strRead));
        EXPECT_EQ(strRead, str);
    }
    is.close();
    remove(fileName.c_str());
}

TEST(TLVConverter, ReadWriteJson) 
{
    std::vector<Json::Value> values = { Json::Value(0), Json::Value(1), Json::Value(-1), Json::Value(true), Json::Value(false),
        Json::Value(10.2), Json::Value(-15.3), Json::Value(3453453.0), Json::Value(0xAFAFAFAF)};

    ofstream os(fileName, ios::trunc|ios::binary);
    for(auto val : values) 
    {
        EXPECT_TRUE(TLV::WriteJson(os, val));
    }
    os.close();

    ifstream is(fileName, ios::binary);
    for(auto val : values) 
    {
        Json::Value valRead;
        EXPECT_TRUE(TLV::ReadJson(is, valRead));
        EXPECT_EQ(valRead, val);
    }
    is.close();
    remove(fileName.c_str());
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}