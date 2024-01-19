#include "imgcharacterview.h"
#include "ui_imgcharacterview.h"

#include "Application.h"
#include "gamedisplay.h"

ImgCharacterView::ImgCharacterView(QWidget *parent)
    : QWidget(parent), ui(new Ui::ImgCharacterView) {
  ui->setupUi(this);
  connect((GameDisplay *)parentWidget(), &GameDisplay::selectCharacter, this,
          &ImgCharacterView::UpdateView);
}

ImgCharacterView::~ImgCharacterView() { delete ui; }

void ImgCharacterView::UpdateView(QString name) {

  const auto &app = Application::GetInstance();
  if (name.isEmpty() || app.m_GameManager == nullptr ||
      app.m_GameManager->m_PlayersManager == nullptr) {
    return;
  }

  const auto &heroList = app.m_GameManager->m_PlayersManager->m_HeroesList;
  // Case hero list empty
  if (heroList.empty()) {
    return;
  }

  // Update image character
  auto qp = QPixmap(":/" + name + ".png");
  ui->img_label->setPixmap(qp);
  ui->img_label->setScaledContents(true);
}
