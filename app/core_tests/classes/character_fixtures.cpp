#include "Application.h"
#include "character.h"
#include "playersmanager.h"

Character GetTestCharacter() {
  const QString testName = "Test";
  const std::set<QString> activePlayers{testName};
  auto *pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  // add heroes
  pm->ClearHeroBossList();
  pm->LoadAllCharactersJson(false, "");
  auto testCh = pm->GetCharacterByName(testName);

  return *testCh;
}
