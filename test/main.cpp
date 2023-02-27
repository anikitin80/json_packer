#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "TLVConverter.h"

TEST(jsonpackage, TLVConverter_WriteTag) 
{
    ofstream os("test.bin", ios::trunc|ios::binary);
    std::vector<int8_t> tagsList;

    EXPECT_FALSE(TLV::WriteTag(os, 0));
    for(uint8_t tag = TLV::FirstTagType; tag <= TLV::LastTagType; tag++) 
    {
        EXPECT_TRUE(TLV::WriteTag(os, tag));
    }
    os.close();

    ifstream is("test.bin", ios::binary);
    for(int8_t tag : tagsList)
    {
        int8_t tagRead = 0;
        EXPECT_TRUE(TLV::ReadTag(is, tagRead));
        EXPECT_EQ(tagRead, tag);
    }
    is.close();    
}

TEST(jsonpackage, TLVConverter_WriteString) 
{
    std::vector<std::string> stringsList = { "", "test", "test test test", "!@#$%^&*()_+=-",
        "very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string very long string"
        };

    ofstream os("test.bin", ios::trunc|ios::binary);
    for(string str: stringsList) 
    {
        EXPECT_TRUE(TLV::WriteString(os, str));
    }
    os.close();

    ifstream is("test.bin", ios::binary);
    for(string str: stringsList) 
    {
        string strRead;
        EXPECT_TRUE(TLV::ReadString(is, strRead));
        EXPECT_EQ(strRead, str);
    }
    is.close();
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}