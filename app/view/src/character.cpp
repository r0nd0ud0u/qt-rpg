#include "character.h"

#include <QtDebug>

using namespace std;

Character::Character(QString name, characType type, int hp, int mana, int vigueur, int rage, int armP, int armMag, int powPhy, int powMag, int aggro, int speed, int criticalStrike, int dodge, int regenHp, int regenMana, int regenVigeur)
{
    m_Name = name;
    m_type = type;
    m_HP = hp;
    m_Mana = mana;
    m_Vigueur = vigueur;
    m_Rage = rage;
    m_ArmPhy = armP;
    m_ArmMag = armMag;
    m_powPhy = powPhy;
    m_powMag = powMag;
    m_aggro = aggro;
    m_speed = speed;
    m_CriticalStrike = criticalStrike;
    m_dogde = dodge;
    m_regenHP = regenHp;
    m_regenMana = regenMana;
    m_regenVigueur = regenVigeur;
}

void Character::Attaque(int degat, Character* target) {
    if (target == nullptr) {
        qDebug("problem");
        return;
    }

    target->m_HP -= degat;

}

void Character::AddAtq(const AttaqueType& atq){
    attakList.push_back(atq);
}
