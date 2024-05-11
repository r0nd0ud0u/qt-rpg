#ifndef IMGCHARACTERVIEW_H
#define IMGCHARACTERVIEW_H

#include <QWidget>

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
    void UpdateView(QString name, const QString& photo);


};

#endif // IMGCHARACTERVIEW_H
