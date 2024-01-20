#ifndef GAMEDISPLAY_H
#define GAMEDISPLAY_H

#include <QWidget>

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
private:
    Ui::GameDisplay *ui;

private slots:
    void UpdateChannel();
    void UpdateViews(const QString& name);
    void on_attaque_button_clicked();
    void on_bag_button_clicked();
    void on_stackedWidget_currentChanged(int arg1);
};

#endif // GAMEDISPLAY_H
