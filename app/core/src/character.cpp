#include "character.h"

#include <QtDebug>

using namespace std;

Character::Character(const QString name, const characType type,  const Stats& stats):
    m_Name(name),
    m_type(type),
    m_Stats(stats)
{
    m_Inventory.resize(static_cast<int>(InventoryType::enumSize));
}

void Character::Attaque(int degat, Character* target) const {
    if (target == nullptr) {
        return;
    }

    target->m_Stats.m_HP -= degat;

}

void Character::AddAtq(const AttaqueType& atq){
    attakList.push_back(atq);
}

void Character::AddStuff(const Stuff& stuff){

    m_Stuffs[stuff.m_type] = stuff;

    m_Stats.m_HP = stuff.m_Stats.m_HP;
    m_Stats.m_Mana = stuff.m_Stats.m_Mana;
    m_Stats.m_Vigor = stuff.m_Stats.m_Vigor;
    m_Stats.m_Bersecker = stuff.m_Stats.m_Bersecker;
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

QString Character::GetInventoryString(const InventoryType& type){
    switch (type) {
    case InventoryType::healthPotion:
        return "Potion de vie";
    case InventoryType::manaPotion:
        return "Potion de mana";
    default:
        break;
    }
    return "";
}

QString Character::GetTargetString(const TargetType& type){
    switch (type) {
    case TargetType::ennemy:
        return "Ennemie";
    case TargetType::ally:
        return "Allié";
    default:
        break;
    }
    return "";
}

QString Character::GetReachString(const ReachType& type){
    switch (type) {
    case ReachType::zone:
        return "Zone";
    case ReachType::individual:
        return "Individuel";
    default:
        break;
    }
    return "";
}
