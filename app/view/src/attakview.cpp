#include "attakview.h"
#include "ui_attakview.h"

#include "Application.h"
#include "gamedisplay.h"

#include <QPushButton>
#include <QDebug>

AttakView::AttakView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AttakView)
{
    ui->setupUi(this);
}

AttakView::~AttakView()
{
    delete ui;
}

void AttakView::UpdateAttak(QString name){
    RemoveAttak();

    for (int i = 0; i < ui->attak_lay->count(); i++){
        QWidget* widget = ui->attak_lay->itemAt(i)->widget();
        if (widget != nullptr)
        {
            widget->setParent(nullptr);
            ui->attak_lay->removeWidget(widget);
            i--;
        }

    }
    const auto &app = Application::GetInstance();
    if (app.m_GameManager == nullptr ||
        app.m_GameManager->m_PlayersManager == nullptr || app.m_GameManager->m_PlayersManager->m_SelectedHero == nullptr) {
        return;
    }

    for (const auto& atk : app.m_GameManager->m_PlayersManager->m_SelectedHero->attakList ) {
        auto* button = new QPushButton();
        button->setText(atk.name);
        ui->attak_lay->addWidget(button);
    }
}

void AttakView::RemoveAttak(){
    auto itt = ui->attak_lay->count();
    for (int i = 0; i < ui->attak_lay->count(); i++){
        QWidget* widget = ui->attak_lay->itemAt(i)->widget();
        if (widget != nullptr)
        {
            widget->setParent(nullptr);
            ui->attak_lay->removeWidget(widget);
            i--;
        }

    }}
