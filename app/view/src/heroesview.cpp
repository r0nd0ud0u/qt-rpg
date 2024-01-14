#include "heroesview.h"
#include "heropanel.h"
#include "ui_heroesview.h"

#include "character.h"
#include "heropanel.h"

HeroesView::HeroesView(QWidget *parent)
    : QWidget(parent), ui(new Ui::HeroesView) {
  ui->setupUi(this);

    for (HeroPanel *panel : m_HeroesList){
        connect(panel, &HeroPanel::addStuff, this, &HeroesView::Dosomething);
    }
}

HeroesView::~HeroesView() { delete ui; }

void HeroesView::AddHeroPanel() {

  HeroPanel *hero1 = new HeroPanel();

  Stats stats;
  stats.m_HP = 605;
  stats.m_Mana = 369;
  stats.m_Vigueur = 126;
  stats.m_Rage = 0;
  stats.m_ArmPhy = 80;
  stats.m_ArmMag = 35;
  stats.m_powPhy = 10;
  stats.m_powMag = 138;
  stats.m_aggro = 0;
  stats.m_speed = 25;
  stats.m_CriticalStrike = 20;
  stats.m_dogde = 10;
  stats.m_regenHP = 4;
  stats.m_regenMana = 25;
  stats.m_regenVigueur = 5;

  hero1->m_Heroe = new Character("Thalia", characType::Hero, stats);

  ui->left_heroes_lay->addWidget(hero1);

  m_HeroesList.push_back(hero1);

  connect(hero1, &HeroPanel::addStuff, this, &HeroesView::Dosomething);
}

void HeroesView::on_pushButton_clicked() { AddHeroPanel(); }

void HeroesView::Dosomething(){
    emit SigAddStuff();
}
