#include "imgcharacterview.h"
#include "ui_imgcharacterview.h"


ImgCharacterView::ImgCharacterView(QWidget *parent)
    : QWidget(parent), ui(new Ui::ImgCharacterView) {
  ui->setupUi(this);
}

ImgCharacterView::~ImgCharacterView() { delete ui; }

void ImgCharacterView::UpdateView(const Character *c) {
  if (c == nullptr || c->m_Name.isEmpty()) {
    return;
  }
  const auto localPhoto =
      (c->m_PhotoName.isEmpty()) ? c->m_Name : c->m_PhotoName;
  SetPixmap(localPhoto, 300);
}

void ImgCharacterView::SetPixmap(const QString &name, const int scalingHeight) {
  QString photoPath = OFFLINE_IMG + name + ".png";
  if (!Utils::FileExists(photoPath)) {
    photoPath = OFFLINE_IMG + "default.png";
    if (!Utils::FileExists(photoPath)) {
      return;
    }
  }
  // Update image character
  auto qp = QPixmap(photoPath);
  QPixmap scaledPixmap =
      qp.scaledToHeight(scalingHeight); // Set the desired width and height
  ui->img_label->setPixmap(scaledPixmap);
  ui->img_label->setScaledContents(true);
}
