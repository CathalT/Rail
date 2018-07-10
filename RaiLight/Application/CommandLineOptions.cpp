#include "RaiLight\Application\CommandLineOptions.h"

#include <QtCore\QCoreApplication>
#include <QtCore\QCommandLineParser>

namespace rail
{
    CommandLineOptions::CommandLineOptions(const QCoreApplication & app)
    {
        QCommandLineParser parser;

        parser.setApplicationDescription("A Light wallet for Nano currency");
        parser.addHelpOption();
        parser.addVersionOption();

        parser.addOptions({
            { "seed", QCoreApplication::translate("main", "Hex encoded seed used to restore your Nano account"), "seed", "" },
            { "node-address", QCoreApplication::translate("main", "Remote node address and port e.g. http://localhost:7076"), "node-address", "" },
            { "listen-port", QCoreApplication::translate("main", "Local port for rpc callbacks e.g. 8080"), "listen-port", "" }
        });

        parser.process(app);

        seed = parser.value("seed");
        nodeAddress = parser.value("node-address");
        listenPort = parser.value("listen-port");
    }

    QString CommandLineOptions::getSeed()
    {
        return std::move(seed);
    }

    QString CommandLineOptions::getNodeAddress()
    {
        return nodeAddress;
    }

    QString CommandLineOptions::getListenPort()
    {
        return listenPort;
    }
}
