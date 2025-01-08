#include "DeathCounter.h"
#include "syati.h"

#if defined(TWN) || defined(KOR)
#define REGIONOFF 0x10
#else
#define REGIONOFF 0
#endif

s32 DeathCounterSystem::getMissCount() {
    UserFile* file = GameDataFunction::getSaveDataHandleSequence()->getCurrentUserFile();

    if (file != NULL) {
        return file->getPlayerMissNum();
    }
    
    return 0;
}

kmCall(0x80487140+REGIONOFF, DeathCounterSystem::getMissCount); // PauseMenu::appear
kmCall(0x80488D2C+REGIONOFF, DeathCounterSystem::getMissCount); // PlayerLeft::init
kmCall(0x80488EE8+REGIONOFF, DeathCounterSystem::getMissCount); // PlayerLeft::exeHide
kmCall(0x80488FE0+REGIONOFF, DeathCounterSystem::getMissCount); // PlayerLeft::exeWait
kmCall(0x80489038+REGIONOFF, DeathCounterSystem::getMissCount); // PlayerLeft::exeWait
kmCall(0x8048EB1C+REGIONOFF, DeathCounterSystem::getMissCount); // ScenarioSelectLayout::setPlayerLeft
kmCall(0x8048EB30+REGIONOFF, DeathCounterSystem::getMissCount); // ScenarioSelectLayout::setPlayerLeft