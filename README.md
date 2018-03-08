# Rail wallet

A hybrid wallet implementation for  Nano https://nano.org/en
The wallet wraps the Nano node and interacts with it over RCP. 
This means you can run a node locally or point it at a remote endpoint (e.g. https://www.nanode.co/ ).
For now only a local node is supported.

## NB
This is still very much a personal project, it has a grand total of 0 tests.
Do not run it against a nano node on the live network. Test network only.
Once I've the client sufficiently tested it should be easier to contribute. (Couple of things on my to do list first.)

## Getting Started

Currently only building on Windows, have not built or tested Mac OSX or Linux.
All the of the components are cross platform so the code itself should not need much modification.
Setting up a cross platform build system like CMake would be ideal, but I don't have the experience to do so in any short amount of time.

### Pre-requisites

- Visual studio 2017 (C++ 17)
- Qt 5.10
- Qt Visual Studio Plugin
- Boost headers - (hoping to remove in the future)
- CryptoPP
- openMP (possibly will remove, was just experimenting)
- cpprest

### Installing

```
Download Visual Studio 2017
Download Qt 5.10 and install to your directory of preference.
Install Qt Visual Studio Plugin
Set Qt Version to Qt Install directory.
Enable OpenMP in Properties -> C++ -> Language

For the next steps I used vcpkg (https://github.com/Microsoft/vcpkg) to install:
boost
cryptoPP
cpprest

Finally download or build the nano node from source and run it in RPC mode locally.
More details can be found at https://github.com/nanocurrency/raiblocks/wiki
```

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
