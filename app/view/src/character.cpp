#include "character.h"

#include <QtDebug>

using namespace std;

Character::Character(QString name, characType type,  Stats stats)
{
    m_Name = name;
    m_type = type;
    m_Stats.m_HP = stats.m_HP;
    m_Stats.m_Mana = stats.m_Mana;
    m_Stats.m_Vigueur = stats.m_Vigueur;
    m_Stats.m_Rage = stats.m_Rage;
    m_Stats.m_ArmPhy = stats.m_ArmPhy;
    m_Stats.m_ArmMag = stats.m_ArmMag;
    m_Stats.m_powPhy = stats.m_powPhy;
    m_Stats.m_powMag = stats.m_powMag;
    m_Stats.m_aggro = stats.m_aggro;
    m_Stats.m_speed = stats.m_speed;
    m_Stats.m_CriticalStrike = stats.m_CriticalStrike;
    m_Stats.m_dogde = stats.m_dogde;
    m_Stats.m_regenHP = stats.m_regenHP;
    m_Stats.m_regenMana = stats.m_regenMana;
    m_Stats.m_regenVigueur = stats.m_regenVigueur;
}

void Character::Attaque(int degat, Character* target) {
    if (target == nullptr) {
        qDebug("problem");
        return;
    }

    target->m_Stats.m_HP -= degat;

}

void Character::AddAtq(const AttaqueType& atq){
    attakList.push_back(atq);
}

void Character::AddStuff(Stuff stuff){

    m_Stuffs[stuff.m_type] = stuff;

    m_Stats.m_HP = stuff.m_Stats.m_HP;
    m_Stats.m_Mana = stuff.m_Stats.m_Mana;
    m_Stats.m_Vigueur = stuff.m_Stats.m_Vigueur;
    m_Stats.m_Rage = stuff.m_Stats.m_Rage;
    m_Stats.m_ArmPhy = stuff.m_Stats.m_ArmPhy;
    m_Stats.m_ArmMag = stuff.m_Stats.m_ArmMag;
    m_Stats.m_powPhy = stuff.m_Stats.m_powPhy;
    m_Stats.m_powMag = stuff.m_Stats.m_powMag;
    m_Stats.m_aggro = stuff.m_Stats.m_aggro;
    m_Stats.m_speed = stuff.m_Stats.m_speed;
    m_Stats.m_CriticalStrike = stuff.m_Stats.m_CriticalStrike;
    m_Stats.m_dogde = stuff.m_Stats.m_dogde;
    m_Stats.m_regenHP = stuff.m_Stats.m_regenHP;
    m_Stats.m_regenMana = stuff.m_Stats.m_regenMana;
    m_Stats.m_regenVigueur = stuff.m_Stats.m_regenVigueur;
}
