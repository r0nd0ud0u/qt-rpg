#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QWidget>
#include <QColor>

// for TargetInfo class
#include "actionsview.h"

#include "playersmanager.h"

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
    void SigUpdatePlayerPanel();
    void SigBossDead(QString);
    void SigEndOfGame();
    void SigUpdateChannelView(const QString&, const QString&, const QColor = QColor("dark"));
    void SigUpdateAllEffectPanel(const std::unordered_map<QString, std::vector<GameAtkEffects>>& );
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
    void EndOfTurn() ;
    void EndOfGame();
    void NewRound();
    void LaunchAttak(const QString& atkName, const std::vector<TargetInfo>&);
};


#endif // GAMEDISPLAY_H
