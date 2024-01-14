#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include <vector>
#include <unordered_map>

#include "Stuff.h"
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
    Character(QString name, characType type, Stats stats);

    void Attaque(int degat, Character* target);
    void AddAtq (const AttaqueType& atq);

    void AddStuff(Stuff stuff);

    Stats m_Stats;
    QString m_Name = "default";
    characType m_type = characType::Hero;
    std::unordered_map<Body, Stuff> m_Stuffs;

    std::vector<AttaqueType> attakList;

    int m_Level = 1;
    int m_Exp = 0;

};



#endif // CHARACTER_H
