#ifndef COMMON_H
#define COMMON_H

#include <qobject.h>

struct Stats{
    int m_HP = 0;
    int m_Mana = 0;
    int m_Vigor = 0;
    int m_Bersecker = 0;
    int m_ArmPhy = 0;
    int m_ArmMag = 0;
    int m_powPhy = 0;
    double m_powMag = 0;
    int m_aggro = 0;
    int m_speed = 0;
    // critical strike in %
    int m_CriticalStrike  = 0;
    // dodge in %
    double m_dogde = 0;
    int m_regenHP = 0;
    double m_regenMana = 0;
    int m_regenVigueur = 0;
};

const QString OFFLINE_IMG = "./offlines/attak/img/";
const QString OFFLINE_ATK = "./offlines/attak/";

// json keys
const QString ATK_NAME = "Nom";
const QString ATK_TARGET = "Cible";
const QString ATK_REACH = "Portée";
const QString ATK_DURATION = "Durée";
const QString ATK_MANA_COST = "Coût de mana";
const QString ATK_VIGOR_COST = "Coût de vigueur";
const QString ATK_BERSECK_AGGRO = "Rage aggro";
const QString ATK_PHOTO = "Photo";
const QString ATK_DAMAGE = "Dégâts";
const QString ATK_HEAL = "Soin";
const QString ATK_REGEN_MANA = "Regen mana";
const QString ATK_EFFECT = "Effet";
const QString REACH_ZONE = "Zone";
const QString REACH_INDIVIDUAL = "Individuel";
const QString TARGET_ENNEMY = "Ennemie";
const QString TARGET_ALLY = "Allié";


#endif // COMMON_H
