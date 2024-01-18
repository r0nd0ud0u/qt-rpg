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
};

#endif // GAMEDISPLAY_H
