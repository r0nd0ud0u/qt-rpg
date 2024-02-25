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

  SetPixmap(name, 300);

}

void ImgCharacterView::SetPixmap(const QString& name, const int scalingHeight){
    // Update image character
    auto qp = QPixmap(OFFLINE_IMG + name + ".png");
    QPixmap scaledPixmap = qp.scaledToHeight(scalingHeight); // Set the desired width and height
    ui->img_label->setPixmap(scaledPixmap);
    ui->img_label->setScaledContents(true);
}
