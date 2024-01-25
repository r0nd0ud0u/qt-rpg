#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include <vector>
#include <unordered_map>

#include "stuff.h"
#include "common.h"
#include "effect.h"


enum class characType {
    Hero,
    Boss
};

class AttaqueType {
public:
    QString name = "Atq";
    uint32_t damage = 0;
    uint32_t heal = 0;
    uint32_t regenMana = 0;
    uint32_t aggroCum = 0;
    QString effect;
    uint32_t manaCost = 0;
    uint32_t vigorCost = 0;
    QString target = TARGET_ENNEMY;
    QString reach = REACH_INDIVIDUAL;
    uint16_t turnsDuration = 1;
    QString namePhoto = "default.png";
    static std::vector<QString> TARGET_TYPES;
    static std::vector<QString> REACH_TYPES;
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
    void LoadAtkJson();
    void LoadStuffJson();
    void ApplyAllEquipment(const std::unordered_map<QString, Stuff>& allEquipMap);

    static QString GetInventoryString(const InventoryType& type);

    QString m_Name = "default";
    characType m_type = characType::Hero;
    Stats m_Stats;
    std::unordered_map<QString, QString> m_WearingEquipment; // key: body, value: equipmentName
    std::vector<AttaqueType> attakList;
    std::vector<uint8_t> m_Inventory;
    std::vector<Effect*> m_EffectsList;
    int m_Level = 1;
    int m_Exp = 0;

};



#endif // CHARACTER_H
