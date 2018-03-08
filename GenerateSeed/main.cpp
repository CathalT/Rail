#include <cryptopp\osrng.h>
#include <cryptopp\hex.h>
#include <iostream>
#include <fstream>

#define SIZE 32

int main(int argc, char *argv[])
{
    byte data[SIZE];
    CryptoPP::AutoSeededRandomPool randPool;

    randPool.GenerateBlock(reinterpret_cast<byte*>(&data), SIZE);

    CryptoPP::HexEncoder encoder;
    encoder.Put(data, sizeof(data));
    encoder.MessageEnd();

    std::string encoded;

    CryptoPP::StringSource ss(data, sizeof(data), true,
        new CryptoPP::HexEncoder(new CryptoPP::StringSink(encoded))
    ); 

    std::ofstream myfile;
    myfile.open("x213.cpp");
    myfile << "#include <string>" << std::endl;
    myfile << "extern std::string secret_key = \"" << encoded << "\";";
    myfile.close();

    return 0;
}
