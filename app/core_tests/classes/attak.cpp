#include "character.h"

AttaqueType SimpleAtkMana(){
    AttaqueType atk;
    atk.manaCost = 10;
    return atk;
}

AttaqueType SimpleAtkVigor(){
    AttaqueType atk;
    atk.vigorCost = 10;
    return atk;
}

AttaqueType SimpleAtkBerseck(){
    AttaqueType atk;
    atk.berseckCost = 10;
    return atk;
}