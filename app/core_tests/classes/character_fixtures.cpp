#include "character.h"
#include "Application.h"
#include "playersmanager.h"

Character GetTestCharacter(){
    const QString testName = "Test";
    const std::set<QString> activePlayers{testName};
    auto *pm = Application::GetInstance().m_GameManager->m_PlayersManager;
    // add heroes
    pm->ClearHeroBossList();
    pm->LoadAllCharactersJson();
    pm->UpdateActivePlayers();
    auto testCh = pm->GetCharacterByName(testName);

    return *testCh;
}
