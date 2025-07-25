#include "DeathCounter.h"

#if defined(TWN) || defined(KOR)
#define REGIONOFF 0x10
#else
#define REGIONOFF 0
#endif

s32 getMissCount() {
    UserFile* file = GameDataFunction::getSaveDataHandleSequence()->getCurrentUserFile();

    if (file != NULL) {
        return file->getPlayerMissNum();
    }
    
    return 0;
}

bool isDeadAndNotInFaceShipOrWorldMap() {
    return MR::isPlayerDead() && !MR::isStageMarioFaceShipOrWorldMap();
}

void showDeathCounterIfDead(PlayerLeft* pPlayerLeft) {
    pPlayerLeft->mPaneRumbler->update();

    Nerve* pNerveCountUp = &NrvPlayerLeftExt::NrvDeathCounterCountUp::sInstance;

    if (!pPlayerLeft->isNerve(pNerveCountUp) && isDeadAndNotInFaceShipOrWorldMap())
        pPlayerLeft->setNerve(pNerveCountUp);
}
kmWrite32(0x80488E9C+REGIONOFF, 0x7FE3FB78); // mr r3, r31
kmCall(0x80488EA0+REGIONOFF, showDeathCounterIfDead); // bl showDeathCounterIfDead

void skipDisappearingPlayerLeft(PlayerLeft* pPlayerLeft) {
    if (isDeadAndNotInFaceShipOrWorldMap())
        pPlayerLeft->disappear();
}

kmCall(0x80465BAC+REGIONOFF, skipDisappearingPlayerLeft); // bl skipDisappearingPlayerLeft

void skipAppearingPlayerLeft(PlayerLeft* pPlayerLeft) {
    if (isDeadAndNotInFaceShipOrWorldMap())
        pPlayerLeft->forceAppear();
}

kmCall(0x80465B2C+REGIONOFF, skipAppearingPlayerLeft); // bl skipDisappearingPlayerLeft

// PauseMenu
kmCall(0x80487140+REGIONOFF, getMissCount); // ::appear

// PlayerLeft
kmCall(0x80488D2C+REGIONOFF, getMissCount); // ::init
kmCall(0x80488EE8+REGIONOFF, getMissCount); // ::exeHide
kmCall(0x80488FE0+REGIONOFF, getMissCount); // ::exeWait
kmCall(0x80489038+REGIONOFF, getMissCount); // ::exeWait

kmWrite32(0x804B43A4+REGIONOFF, 0x48000058); // b 0x58

namespace NrvPlayerLeftExt {
    void NrvDeathCounterCountUp::execute(Spine* pSpine) const {
        PlayerLeft* pPlayerLeft = (PlayerLeft*)pSpine->mExecutor;

        if (MR::isFirstStep(pPlayerLeft)) {
            if (pPlayerLeft->mAppearer->isDisappeared()) {
                pPlayerLeft->LayoutActor::appear();
                pPlayerLeft->mAppearer->reset();
                pPlayerLeft->mPaneRumbler->reset();
                pPlayerLeft->mAppearer->appear(TVec2f(0.0f, 0.0f));
                MR::showLayout(pPlayerLeft);
                MR::startAnim(pPlayerLeft, "Wait", 1);
            }
        }

        if (MR::isStep(pPlayerLeft, 30)) {
            pPlayerLeft->mNumPlayersLeft += 1;
            MR::startAnim(pPlayerLeft, "Flash", 0);
            pPlayerLeft->mPaneRumbler->start();
            MR::startSystemSE("SE_SY_BEAT_ENEMY_COUNT_UP", -1, -1);
        }
    }

    NrvDeathCounterCountUp(NrvDeathCounterCountUp::sInstance);
}