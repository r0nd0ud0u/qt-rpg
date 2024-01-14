#ifndef CHANNEL_H
#define CHANNEL_H

#include <QWidget>

namespace Ui {
class Channel;
}

class Channel : public QWidget
{
    Q_OBJECT

public:
    explicit Channel(QWidget *parent = nullptr);
    ~Channel();

    void ShowPageStuffs();
    void AddStuff();

private:
    Ui::Channel *ui;


};

#endif // CHANNEL_H
