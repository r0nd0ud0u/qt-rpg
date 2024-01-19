#include "stuff.h"

QString Stuff::GetStringBody(const Body &body) {
  switch (body) {
  case Body::head:
    return "Tête";
  case Body::necklace:
    return "Collier";
  case Body::chest:
    return "Torse";
  case Body::pants:
    return "Pantalon";
  case Body::shoes:
    return "Chaussures";
  case Body::arm_left:
    return "Bras gauche";
  case Body::arm_right:
    return "Bras droit";
  case Body::weapon_left:
    return "Arme gauche";
  case Body::weapon_right:
    return "Arme droite";
  case Body::ring:
    return "Bâgue";
  default:
      break;
  }

    return "test";
}
