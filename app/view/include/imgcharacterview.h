#ifndef IMGCHARACTERVIEW_H
#define IMGCHARACTERVIEW_H

#include <QWidget>

#include "character.h"

namespace Ui {
class ImgCharacterView;
}

class ImgCharacterView : public QWidget
{
    Q_OBJECT

public:
    explicit ImgCharacterView(QWidget *parent = nullptr);
    ~ImgCharacterView();

    void SetPixmap(const QString& name, const int scalingHeight);


private:
    Ui::ImgCharacterView *ui;

private slots:
    void UpdateView(const Character* c);


};

#endif // IMGCHARACTERVIEW_H
