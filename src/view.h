#ifndef VIEW_H
#define VIEW_H

#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "picture.h"

class View : public QWidget {
 public:
  explicit View(QWidget *parent = nullptr);

 private:
  void onLoadImage();
  void onSaveImage();
  void updateImageLabel();
  void updateImageFromPicture();
  void applyThreePassFilter();

  QPushButton *btnLoad;
  QPushButton *btnSave;
  QPushButton *btnCreateTest;
  QPushButton *btnAddNoise;
  QPushButton *btnFilter;
  QPushButton *btnThreePassFilter;
  QLabel *imageLabel;

  Picture picture;
  QImage currentImage;
};

#endif