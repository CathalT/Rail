#include "Ui\RaiLight.h"

#include "Application\CommandLineOptions.h"
#include "Controllers\ICore.h"
#include "Controllers\Core.h"

#include <QtWidgets\QApplication>

#include "Crypto/SecureString.h"
#include <array>

#include <memory>

using namespace rail;
using namespace rail::control;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CommandLineOptions options(a);
    const auto coreController = std::make_unique<Core>(options.getNodeAddress().toStdString());

    QCoreApplication::setApplicationName("Rail");
    QCoreApplication::setApplicationVersion("0.0.0.0");

    std::string lol("lol");
    SecureString test(lol);

    RaiLight w(coreController.get(), options.getSeed());
    w.show();
    return a.exec();
}
