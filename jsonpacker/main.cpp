// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "jsonpacker.h"

int main(int argc, char *argv[])
{
    // open input file
    if (argc < 2)
    {
        std::cout << "No file name specified. Usage: JsonPacker.exe [FILE_PATH]" << std::endl;
        return -1;
    }

    CJsonPacker packer;
    if (!packer.EncodeFile(argv[1]))
        return -1;

    return 0;
}
