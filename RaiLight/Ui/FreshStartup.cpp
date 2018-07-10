#include "RaiLight\Ui\FreshStartup.h"

#include "RaiLight\Controllers\ICore.h"
#include "RaiLight\Crypto\SecretsStore.h"

using namespace rail::control;

FreshStartup::FreshStartup(ICore* _coreController) :
    coreController(_coreController)
{
    ui.setupUi(this);

    connect(ui.generateButton, SIGNAL(clicked()), this, SIGNAL(createNewSeedClicked()));
    connect(ui.restoreButton, &QPushButton::clicked, [this]() 
    {
        auto qSeed = ui.seedInputBox->text();
        if (!qSeed.isEmpty())
        {
            coreController->getSecretsStore()->setSeed(qSeed.toStdString());
        }
        emit restoreSeedClicked();
    });
}