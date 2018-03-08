# Rail wallet

A hybrid wallet implementation for  Nano https://nano.org/en
The wallet wraps the Nano node and interacts with it over RCP. 
This means you can run a node locally or point it at a remote endpoint.

## Getting Started

Currently only building on Windows, have not built or tested Mac OSX or Linux.
All the of the components are cross platform so it should not take to much work to get things building.

### Pre-requisites

Visual studio 2017 (C++ 17)
Qt 5.10
Qt Visual Studio Plugin
Boost headers - (hoping to remove in the future)
CryptoPP
openMP (possibly will remove, was just experimenting)
cpprest

### Installing

A step by step series of examples that tell you have to get a development env running

Say what the step will be

```
Download Visual Studio 2017
Download Qt 5.10 and install to your directory of preference.
Install Qt Visual Studio Plugin
Set Qt Version to Qt Install directory.
Enable OpenMP in Properties -> C++ -> Language

For the next steps I used [vcpkg](https://github.com/Microsoft/vcpkg) to install:
boost
cryptoPP
cpprest
```

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
