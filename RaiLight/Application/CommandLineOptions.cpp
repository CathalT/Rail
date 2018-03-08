#include "CommandLineOptions.h"

#include <QtCore\QCoreApplication>
#include <QtCore\QCommandLineParser>

namespace rail
{
    CommandLineOptions::CommandLineOptions(const QCoreApplication & app)
    {
        QCommandLineParser parser;

        parser.setApplicationDescription("Light wallet for Raiblocks");
        parser.addHelpOption();
        parser.addVersionOption();

        parser.addOptions({
            { "seed", QCoreApplication::translate("main", "Hex encoded seed used to restore your Raiblocks account"), "seed", "" },
            { "node-address", QCoreApplication::translate("main", "Remote node address and port e.g. http://localhost:7076"), "node-address", "" }
        });

        parser.process(app);

        seed = parser.value("seed");
        nodeAddress = parser.value("node-address");
    }

    QString CommandLineOptions::getSeed()
    {
        return seed;
    }

    QString CommandLineOptions::getNodeAddress()
    {
        return nodeAddress;
    }
}
