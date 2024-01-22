#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include <vector>
#include <unordered_map>

#include "stuff.h"
#include "common.h"


enum class characType {
    Hero,
    Boss
};

enum class TargetType{
    ennemy,
    ally,
    enumSize
};

enum class ReachType{
    zone,
    individual,
    enumSize
};

struct AttaqueType {
    QString name = "Atq";
    uint32_t damage = 0;
    uint32_t aggroCum = 0;
    QString effect;
    uint32_t manaCost = 0;
    TargetType target = TargetType::ennemy;
    ReachType reach = ReachType::individual;
    uint16_t turnsDuration = 1;
    QString namePhoto;
};

enum class InventoryType{
    healthPotion,
    manaPotion,
    enumSize

};



class Character
{
public:
    Character(const QString name, const characType type, const Stats& stats);

    void Attaque(int degat, Character* target) const;
    void AddAtq (const AttaqueType& atq);
    void AddStuff(const Stuff& stuff);

    static QString GetInventoryString(const InventoryType& type);
    static QString GetTargetString(const TargetType& type);
    static QString GetReachString(const ReachType& type);

    QString m_Name = "default";
    characType m_type = characType::Hero;
    Stats m_Stats;
    std::unordered_map<Body, Stuff> m_Stuffs;
    std::vector<AttaqueType> attakList;
    std::vector<uint8_t> m_Inventory;
    int m_Level = 1;
    int m_Exp = 0;

};



#endif // CHARACTER_H
