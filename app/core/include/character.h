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

struct AttaqueType {
    QString name;
    int damage = 0;
    int aggroCum = 0;
    QString effect;
    int manaCost = 0;
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
