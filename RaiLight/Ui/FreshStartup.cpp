#include "Ui\FreshStartup.h"

#include "Controllers\ICore.h"

using namespace rail::control;

FreshStartup::FreshStartup(ICore* _coreController) :
    coreController(_coreController)
{
    ui.setupUi(this);

    connect(ui.generateButton, SIGNAL(clicked()), this, SIGNAL(createNewSeedClicked()));
    connect(ui.restoreButton, &QPushButton::clicked, [this]() 
    {
        emit restoreSeedClicked(ui.seedInputBox->text());
    });
}