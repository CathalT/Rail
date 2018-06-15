#include "Ui\RaiLight.h"

#include "Ui\MainWindow.h"
#include "Ui\LockScreen.h"
#include "Ui\NewPasswordScreen.h"
#include "Ui\FreshStartup.h"


#include "Controllers\ICore.h"
#include "Controllers\IBank.h"

#include "Crypto\SecretsStore.h"

#include "Database\RailDb.h"
#include "Database\DatabaseKeys.h"

#include <QMessageLogger>
#include <QDebug>

using namespace rail;
using namespace rail::control;

RaiLight::~RaiLight() = default;
RaiLight::RaiLight(ICore* _coreController) :
    coreController(_coreController)
{
    ui.setupUi(this);

    auto isSeedSet = _coreController->getSecretsStore()->isSeedSet();
    if (!isSeedSet)
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
        goToMainWindow();
    }
}

void RaiLight::goToMainWindow()
{
    deleteChildren();

    const auto mainWindow = new MainWindow(coreController);

    ui.centralWidget->layout()->addWidget(mainWindow);

    coreController->getBank()->init();
}

void RaiLight::goToFreshStartup()
{
    deleteChildren();

    const auto freshStartup = new FreshStartup(coreController);

    connect(freshStartup, SIGNAL(createNewSeedClicked()), this, SLOT (goToNewPasswordScreen()));
    connect(freshStartup, SIGNAL(restoreSeedClicked()), this, SLOT(goToNewPasswordScreen()));

    ui.centralWidget->layout()->addWidget(freshStartup);
}

void RaiLight::goToLockScreen()
{
    deleteChildren();

    const auto lockScreen = new LockScreen(coreController);

    connect(lockScreen, SIGNAL(backButtonClicked()), this, SLOT(goToFreshStartup()));
    connect(lockScreen, &LockScreen::passwordCorrect, [this]()
    {
        goToMainWindow();
    });

    ui.centralWidget->layout()->addWidget(lockScreen);
}

void RaiLight::goToNewPasswordScreen()
{
    deleteChildren();

    const auto newPasswordScreen = new NewPasswordScreen(coreController);

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