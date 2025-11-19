#include "view.h"

#include <QColor>
#include <QDebug>
#include <QFileDialog>
#include <QPixmap>

View::View(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Медианная фильтрация - Вариант 12");
  resize(600, 400);

  btnLoad = new QPushButton("Загрузить изображение");
  btnSave = new QPushButton("Сохранить изображение");
  btnCreateTest = new QPushButton("Создать тест");
  btnAddNoise = new QPushButton("Добавить шум 50%");
  btnFilter = new QPushButton("Применить медианный фильтр");

  imageLabel = new QLabel();
  imageLabel->setAlignment(Qt::AlignCenter);
  imageLabel->setMinimumSize(256, 256);
  imageLabel->setText("Загрузите изображение или создайте тестовое");

  auto *buttonLayout = new QVBoxLayout();
  buttonLayout->addWidget(btnLoad);
  buttonLayout->addWidget(btnSave);
  buttonLayout->addWidget(btnCreateTest);
  buttonLayout->addWidget(btnAddNoise);
  buttonLayout->addWidget(btnFilter);
  buttonLayout->addStretch();

  auto *mainLayout = new QHBoxLayout(this);
  mainLayout->addWidget(imageLabel, 1);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);

  connect(btnLoad, &QPushButton::clicked, [this]() { this->onLoadImage(); });

  connect(btnSave, &QPushButton::clicked, [this]() { this->onSaveImage(); });

  connect(btnCreateTest, &QPushButton::clicked, [this]() {
    picture.createTestImage();
    updateImageFromPicture();
    btnSave->setEnabled(true);
    btnAddNoise->setEnabled(true);
    btnFilter->setEnabled(true);
  });

  connect(btnAddNoise, &QPushButton::clicked, [this]() {
    if (picture.isValid()) {
      picture.applyImpulseNoise(0.50);
      updateImageFromPicture();
    }
  });

  connect(btnFilter, &QPushButton::clicked, [this]() {
    if (picture.isValid()) {
      picture.applyVerticalMedian5x1();
      updateImageFromPicture();
    }
  });

  btnSave->setEnabled(false);
  btnAddNoise->setEnabled(false);
  btnFilter->setEnabled(false);
}

void View::onLoadImage() {
  QString fileName = QFileDialog::getOpenFileName(
      this, "Выберите изображение", "",
      "Images (*.png *.jpg *.jpeg *.bmp *.raw);;RAW images (*.raw);;All files "
      "(*.*)");

  if (fileName.isEmpty()) return;

  bool success = false;

  if (fileName.endsWith(".raw", Qt::CaseInsensitive)) {
    success = picture.loadRaw256(fileName.toStdString());
  } else {
    success = picture.loadFromFile(fileName.toStdString());
  }

  if (!success) {
    qDebug() << "Ошибка загрузки изображения!";
    return;
  }

  updateImageFromPicture();
  btnSave->setEnabled(true);
  btnAddNoise->setEnabled(true);
  btnFilter->setEnabled(true);
}

void View::onSaveImage() {
  if (!picture.isValid()) return;

  QString fileName = QFileDialog::getSaveFileName(
      this, "Сохранить изображение", "processed_image.png",
      "PNG images (*.png);;JPEG images (*.jpg *.jpeg);;BMP images (*.bmp);;All "
      "files (*.*)");

  if (fileName.isEmpty()) return;

  if (!fileName.contains('.')) {
    fileName += ".png";
  }

  if (!picture.saveToFile(fileName.toStdString())) {
    qDebug() << "Ошибка сохранения изображения!";
  } else {
    qDebug() << "Изображение успешно сохранено:" << fileName;
  }
}

void View::updateImageFromPicture() {
  if (!picture.isValid()) return;

  currentImage = QImage(picture.getWidth(), picture.getHeight(),
                        QImage::Format_Grayscale8);
  for (int y = 0; y < picture.getHeight(); ++y) {
    for (int x = 0; x < picture.getWidth(); ++x) {
      unsigned char val = picture.at(x, y);
      currentImage.setPixelColor(x, y, QColor(val, val, val));
    }
  }
  updateImageLabel();
}

void View::updateImageLabel() {
  if (currentImage.isNull()) return;

  imageLabel->setPixmap(QPixmap::fromImage(currentImage)
                            .scaled(imageLabel->width(), imageLabel->height(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
}