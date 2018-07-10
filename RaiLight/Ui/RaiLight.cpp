#include "RaiLight\Ui\RaiLight.h"

#include "RaiLight\Ui\MainWindow.h"
#include "RaiLight\Ui\LockScreen.h"
#include "RaiLight\Ui\NewPasswordScreen.h"
#include "RaiLight\Ui\FreshStartup.h"


#include "RaiLight\Controllers\ICore.h"
#include "RaiLight\Controllers\IBank.h"

#include "RaiLight\Crypto\SecretsStore.h"

#include "RaiLight\Database\IDbWrapper.h"
#include "RaiLight\Database\DatabaseKeys.h"

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
        const auto seedBytes = _coreController->getDatabase()->getByteArray32(key::bytes::SEED);

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