#include "Ui\RaiLight.h"

#include "Application\CommandLineOptions.h"
#include "Controllers\ICore.h"
#include "Controllers\Core.h"

#include <QtWidgets\QApplication>

#include <argon2.h>

#include <memory>

using namespace rail;
using namespace rail::control;

int main(int argc, char *argv[])
{
    uint8_t hash1[32];

    uint8_t salt[32];
    memset(salt, 0x00, 32);

    uint8_t *pwd = (uint8_t *)_strdup("test");
    uint32_t pwdlen = strlen((char *)pwd);

    uint32_t t_cost = 2;            // 1-pass computation
    uint32_t m_cost = (1 << 16);      // 64 mebibytes memory usage
    uint32_t parallelism = 1;       // number of threads and lanes

                                    // high-level API
    argon2i_hash_raw(t_cost, m_cost, parallelism, pwd, pwdlen, salt, 32, hash1, 32);

    QApplication a(argc, argv);
    CommandLineOptions options(a);
    const auto coreController = std::make_unique<Core>(options.getSeed().toStdString(), options.getNodeAddress().toStdString());

    QCoreApplication::setApplicationName("Rai Light");
    QCoreApplication::setApplicationVersion("0.0.0.0");

    RaiLight w(coreController.get());
    w.show();
    return a.exec();
}
