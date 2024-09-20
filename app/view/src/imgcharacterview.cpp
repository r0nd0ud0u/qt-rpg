#include "imgcharacterview.h"
#include "ui_imgcharacterview.h"

#include "Application.h"
#include "gamedisplay.h"
#include "gamecharacters.h"

ImgCharacterView::ImgCharacterView(QWidget *parent)
    : QWidget(parent), ui(new Ui::ImgCharacterView) {
  ui->setupUi(this);
  connect((GameDisplay *)parentWidget(), &GameDisplay::selectCharacter, this,
          &ImgCharacterView::UpdateView);
  connect((GameCharacters *)parentWidget(), &GameCharacters::SigSelectGameCharacter, this,
          &ImgCharacterView::UpdateView);
}

ImgCharacterView::~ImgCharacterView() { delete ui; }

void ImgCharacterView::UpdateView(QString name, const QString &photo) {

  const auto &app = Application::GetInstance();
  if (name.isEmpty() || app.m_GameManager == nullptr ||
      app.m_GameManager->m_PlayersManager == nullptr) {
    return;
  }
  const auto localPhoto = (photo.isEmpty()) ? name : photo;
  SetPixmap(localPhoto, 300);
}

void ImgCharacterView::SetPixmap(const QString &name, const int scalingHeight) {
  // Update image character
    //return;
  auto qp = QPixmap(OFFLINE_IMG + name + ".png");
  QPixmap scaledPixmap =
      qp.scaledToHeight(scalingHeight); // Set the desired width and height
 // ui->img_label->setPixmap(qp.scaled(171, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->img_label->setPixmap(scaledPixmap);
  ui->img_label->setScaledContents(true);
}
