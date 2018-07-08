#pragma once

#include <QString>

class QCoreApplication;
namespace rail
{
    class CommandLineOptions
    {
    public:
        CommandLineOptions(const QCoreApplication& app);

        QString getSeed();
        QString getNodeAddress();
        QString getListenPort();

    private:
        QString seed;
        QString nodeAddress;
        QString listenPort;
    };
}
