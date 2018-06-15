#include <cryptopp\osrng.h>
#include <cryptopp\hex.h>

#include <iostream>
#include <iomanip>
#include <fstream>

#define SIZE 32

int main(int argc, char *argv[])
{
    CryptoPP::byte data[SIZE];
    CryptoPP::AutoSeededRandomPool randPool;

    randPool.GenerateBlock(reinterpret_cast<CryptoPP::byte*>(&data), SIZE);

    bool shouldAppendComma = true;
    std::stringstream stream;
    stream << "{";
    for (int i = 0; i < SIZE; ++i)
    {
        shouldAppendComma = (i + 1) != SIZE;
        stream << "static_cast<std::byte>(0x" << std::hex << std::noshowbase << std::setfill('0') << std::setw(2) << static_cast<int>(data[i]) << ")" << (shouldAppendComma ? "," : "");
    }
    stream << " }";

    std::ofstream myfile;
    myfile.open("x213.cpp"); 
    myfile << "#include \"Model\\BasicTypes.h\"\n";
    myfile << "#include <cstddef>\n";
    myfile << "extern rail::ByteArray32 secret_key = " << stream.str() << ";";
    myfile.close();

    return 0;
}
