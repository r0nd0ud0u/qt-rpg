#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include <vector>

enum class characType {
    Hero,
    Boss
};

struct AttaqueType {
    std::string name;
    int damage = 0;
    int aggroCum = 0;
};

class Character
{
public:
    Character(QString name, characType type, int hp, int mana, int vigueur, int rage, int armP, int armMag, int powPhy, int powMag, int aggro, int speed, int criticalStrike, int dodge, int regenHp, int regenMana, int regenVigeur);

    void Attaque(int degat, Character* target);
    void AddAtq (const AttaqueType& atq);

    QString m_Name = "default";
    characType m_type = characType::Hero;
    int m_HP = 0;
    int m_Mana = 0;
    int m_Vigueur = 0;
    int m_Rage = 0;
    int m_ArmPhy = 0;
    int m_ArmMag = 0;
    int m_powPhy = 0;
    int m_powMag = 0;
    int m_aggro = 0;
    int m_speed = 0;
    int m_CriticalStrike  = 0;
    int m_dogde = 0;
    int m_regenHP = 0;
    int m_regenMana = 0;
    int m_regenVigueur = 0;

    std::vector<AttaqueType> attakList;

};



#endif // CHARACTER_H
