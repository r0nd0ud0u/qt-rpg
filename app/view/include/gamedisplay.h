#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QWidget>

// for TargetInfo class
#include "actionsview.h"

namespace Ui {
class GameDisplay;
}

class GameDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit GameDisplay(QWidget *parent = nullptr);
    ~GameDisplay();

signals:
    void selectCharacter(QString);
    void SigUpdateHeroPanel();
    void SigBossDead(QString);
    void SigEndOfGame();
private:
    Ui::GameDisplay *ui;
    void UpdateGameStatus();

private slots:
    void UpdateChannel();
    void UpdateViews(const QString& name);
    void on_attaque_button_clicked();
    void on_bag_button_clicked();
    void on_stackedWidget_currentChanged(const int arg1);
    void StartNewTurn();
    void EndOfGame();
    void NewRound();
    void LaunchAttak(const QString& atkName, const std::vector<TargetInfo>&);
};


#endif // GAMEDISPLAY_H
