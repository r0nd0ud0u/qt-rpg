#ifndef CHARACTERWINDOW_H
#define CHARACTERWINDOW_H

#include <QMainWindow>

namespace Ui {
class CharacterWindow;
}

class CharacterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CharacterWindow(QWidget *parent = nullptr);
    ~CharacterWindow();
    void InitWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CharacterWindow *ui;
};

#endif // CHARACTERWINDOW_H
