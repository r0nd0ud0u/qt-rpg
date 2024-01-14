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
    std::string name;
    int damage = 0;
    int aggroCum = 0;
};



class Character
{
public:
    Character(const QString name, const characType type, const Stats stats);

    void Attaque(int degat, Character* target) const;
    void AddAtq (const AttaqueType& atq);
    void AddStuff(const Stuff& stuff);

    QString m_Name = "default";
    characType m_type = characType::Hero;
    Stats m_Stats;
    std::unordered_map<Body, Stuff> m_Stuffs;
    std::vector<AttaqueType> attakList;
    int m_Level = 1;
    int m_Exp = 0;

};



#endif // CHARACTER_H
