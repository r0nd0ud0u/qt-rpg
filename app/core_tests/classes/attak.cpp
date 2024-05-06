#include "character.h"

AttaqueType SimpleAtkMana(){
    AttaqueType atk;
    atk.manaCost = 10;
    return atk;
}

AttaqueType SimpleAtkVigor(){
    AttaqueType atk;
    atk.vigorCost = 10;
    return atk;
}

AttaqueType SimpleAtkBerseck(){
    AttaqueType atk;
    atk.berseckCost = 10;
    return atk;
}

AttaqueType SimpleHot(){
    AttaqueType atk;
    atk.manaCost = 10;
    effectParam ep;
    ep.effect = EFFECT_VALUE_CHANGE;
    ep.value = 10;
    ep.nbTurns = 10;
    ep.statsName = STATS_HP;
    atk.m_AllEffects.push_back(ep);
    return atk;
}
