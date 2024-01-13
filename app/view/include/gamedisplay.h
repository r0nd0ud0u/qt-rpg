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

private:
    Ui::GameDisplay *ui;
};

#endif // GAMEDISPLAY_H
