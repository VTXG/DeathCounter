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

// PauseMenu
kmCall(0x80487140+REGIONOFF, DeathCounterSystem::getMissCount); // ::appear

// PlayerLeft
kmCall(0x80488D2C+REGIONOFF, DeathCounterSystem::getMissCount); // ::init
kmCall(0x80488EE8+REGIONOFF, DeathCounterSystem::getMissCount); // ::exeHide
kmCall(0x80488FE0+REGIONOFF, DeathCounterSystem::getMissCount); // ::exeWait
kmCall(0x80489038+REGIONOFF, DeathCounterSystem::getMissCount); // ::exeWait

// PlayerMissLeft
kmCall(0x80489424+REGIONOFF, DeathCounterSystem::getMissCount); // ::exeAppear
kmWrite32(0x80489434+REGIONOFF, 0x38A5FFFF); // ::exeAppear (addi r5, r5, -1)
kmCall(0x80489514+REGIONOFF, DeathCounterSystem::getMissCount); // ::exeWait

// ScenarioSelectLayout
// kmCall(0x8048EB1C+REGIONOFF, DeathCounterSystem::getMissCount); // ::setPlayerLeft
// kmCall(0x8048EB30+REGIONOFF, DeathCounterSystem::getMissCount); // ::setPlayerLeft