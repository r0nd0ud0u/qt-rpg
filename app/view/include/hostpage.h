#ifndef HOSTPAGE_H
#define HOSTPAGE_H

#include <QWidget>

namespace Ui {
class HostPage;
}

class HostPage : public QWidget
{
    Q_OBJECT

public:
    explicit HostPage(QWidget *parent = nullptr);
    ~HostPage();

signals:
    void showGameDisplay();
    void SigShowGameCharacters();

private slots:
    void on_pushButton_clicked();

    void on_charactersPushButton_clicked();

private:
    Ui::HostPage *ui;
};

#endif // HOSTPAGE_H
