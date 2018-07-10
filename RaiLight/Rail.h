#pragma once

#include "RaiLight\Ui\RaiLight.h"

#include "RaiLight\Application\CommandLineOptions.h"
#include "RaiLight\Controllers\ICore.h"
#include "RaiLight\Controllers\Core.h"

#include <QtWidgets\QApplication>
#include <cryptopp\misc.h>

#include <memory>

using namespace rail::control;
namespace rail
{
    int Start(int argc, char *argv[])
    {
        QApplication a(argc, argv);
        CommandLineOptions options(a);

        for (auto i = 1; i < argc; ++i)
        {
            CryptoPP::SecureWipeArray(argv[i], strlen(argv[i]));
        }

        const auto coreController = std::make_unique<Core>(options.getNodeAddress().toStdString(), options.getListenPort().toStdString(), options.getSeed().toStdString());

        QCoreApplication::setApplicationName("Rail");
        QCoreApplication::setApplicationVersion("0.0.0.0");

        RaiLight w(coreController.get());
        w.show();
        return a.exec();
    }
}
