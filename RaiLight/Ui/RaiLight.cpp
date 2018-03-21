#include "Ui\RaiLight.h"

#include "Ui\MainWindow.h"
#include "Ui\LockScreen.h"
#include "Ui\NewPasswordScreen.h"
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
            goToLockScreen();
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

    connect(freshStartup, &FreshStartup::createNewSeedClicked, [this](){ goToNewPasswordScreen(""); });
    connect(freshStartup, SIGNAL(restoreSeedClicked(const QString&)), this, SLOT(goToNewPasswordScreen(const QString&)));

    ui.centralWidget->layout()->addWidget(freshStartup);
}

void RaiLight::goToLockScreen()
{
    deleteChildren();

    const auto passwordScreen = new LockScreen(coreController);

    connect(passwordScreen, SIGNAL(backButtonClicked()), this, SLOT(goToFreshStartup()));

    ui.centralWidget->layout()->addWidget(passwordScreen);
}

void RaiLight::goToNewPasswordScreen(const QString& seed)
{
    deleteChildren();

    const auto newPasswordScreen = new NewPasswordScreen(coreController, seed);

    //pass seed
    connect(newPasswordScreen, SIGNAL(onPasswordMatch()), this, SLOT(goToMainWindow()));

    ui.centralWidget->layout()->addWidget(newPasswordScreen);
}

void RaiLight::deleteChildren()
{
    for (auto child : ui.centralWidget->findChildren<QWidget *>(QString(), Qt::FindChildOption::FindDirectChildrenOnly))
    {
        delete child;
    }
}