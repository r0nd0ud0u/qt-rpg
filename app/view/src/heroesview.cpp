#include "heroesview.h"
#include "heropanel.h"
#include "ui_heroesview.h"

#include "character.h"

HeroesView::HeroesView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HeroesView)
{
    ui->setupUi(this);
}

HeroesView::~HeroesView()
{
    delete ui;
}

void HeroesView::AddHeroPanel(){

    HeroPanel* hero1 = new HeroPanel();

    hero1->m_Heroe = new Character("Thalia", characType::Hero, 605, 369, 126,0, 80, 35, 10, 138,0, 25, 20, 10, 4 ,25,5);

    ui->left_heroes_lay->addWidget(hero1);

    m_HeroesList.push_back(hero1);



}

void HeroesView::on_pushButton_clicked()
{
    AddHeroPanel();
}

