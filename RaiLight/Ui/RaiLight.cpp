#include "Ui\RaiLight.h"

#include "Ui\MainWindow.h"
#include "Ui\PasswordScreen.h"
#include "Ui\FreshStartup.h"


#include "Controllers\ICore.h"
#include "Controllers\IBank.h"
#include "Database\RailDb.h"
#include "Database\DatabaseKeys.h"

#include <QMessageLogger>
#include <QDebug>

using namespace rail;
using namespace rail::control;

RaiLight::~RaiLight() = default;
RaiLight::RaiLight(ICore* _coreController, const QString& _seed) :
    coreController(_coreController)
{
    ui.setupUi(this);

    if (_seed.isEmpty())
    {
        const auto seedBytes = _coreController->getDatabase()->getValue<ByteArray32>(key::bytes::SEED);
        if (seedBytes)
        {
            goToPasswordScreen();
        }
        else
        {
            goToFreshStartup();
        }
    }
    else
    {
        goToMainWindow(_seed);
    }
}

void RaiLight::goToMainWindow(const QString& seed)
{
    deleteChildren();

    const auto mainWindow = new MainWindow(coreController);

    ui.centralWidget->layout()->addWidget(mainWindow);

    coreController->getBank()->initWithSeed(seed.toStdString());
}

void RaiLight::goToFreshStartup()
{
    deleteChildren();

    const auto freshStartup = new FreshStartup(coreController);

    connect(freshStartup, &FreshStartup::createNewSeedClicked, [this](){ goToMainWindow(""); });
    connect(freshStartup, SIGNAL(restoreSeedClicked(const QString&)), this, SLOT(goToMainWindow(const QString&)));

    ui.centralWidget->layout()->addWidget(freshStartup);
}

void RaiLight::goToPasswordScreen()
{
    deleteChildren();

    const auto passwordScreen = new PasswordScreen(coreController);

    connect(passwordScreen, SIGNAL(backButtonClicked()), this, SLOT(goToFreshStartup()));

    ui.centralWidget->layout()->addWidget(passwordScreen);
}

void RaiLight::deleteChildren()
{
    if (auto freshStartupWidget = ui.centralWidget->findChild<QWidget*>("FreshStartup"))
    {
        delete freshStartupWidget;
    }
    if (auto passwordScreenWidget = ui.centralWidget->findChild<QWidget*>("PasswordScreen"))
    {
        delete passwordScreenWidget;
    }
}