#include "picture.h"

#include <QColor>
#include <QImage>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

bool Picture::loadRaw256(const string& filename) {
  ifstream file(filename, ios::binary);
  if (!file.is_open()) return false;

  const size_t size = 256 * 256;
  data.resize(size);
  file.read(reinterpret_cast<char*>(data.data()), size);

  if (!file || file.gcount() != size) {
    data.clear();
    return false;
  }

  width = height = 256;
  return true;
}

bool Picture::loadFromFile(const string& filename) {
  QImage qimg;
  if (!qimg.load(QString::fromStdString(filename))) {
    return false;
  }

  // Конвертация в grayscale, если это необходимо
  if (qimg.format() != QImage::Format_Grayscale8) {
    qimg = qimg.convertToFormat(QImage::Format_Grayscale8);
  }

  // Масштабирование до 256x256, если это необходимо
  if (qimg.width() != 256 || qimg.height() != 256) {
    qimg =
        qimg.scaled(256, 256, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  }

  width = qimg.width();
  height = qimg.height();
  data.resize(width * height);

  // Копирование данных
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      QColor color = qimg.pixelColor(x, y);
      data[y * width + x] = color.red();  // в grayscale все каналы одинаковые
    }
  }

  return true;
}

bool Picture::saveToFile(const string& filename) {
  if (!isValid()) return false;

  QImage qimg(width, height, QImage::Format_Grayscale8);

  // Заполнение QImage данными
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      unsigned char val = data[y * width + x];
      qimg.setPixelColor(x, y, QColor(val, val, val));
    }
  }

  return qimg.save(QString::fromStdString(filename));
}

bool Picture::isValid() const {
  if (width != 256 || height != 256) return false;
  if (data.empty()) return false;
  return true;
}

// Вертикальный медианный фильтр 5x1
void Picture::applyVerticalMedian5x1() {
  if (!isValid()) {
    throw runtime_error("Image is invalid or not loaded.");
  }

  vector<unsigned char> result(data);

  for (int x = 0; x < width; x++) {
    for (int y = 2; y < height - 2; y++) {
      unsigned char window[5];

      // Берём 5 пикселей по вертикали
      window[0] = data[(y - 2) * width + x];
      window[1] = data[(y - 1) * width + x];
      window[2] = data[y * width + x];
      window[3] = data[(y + 1) * width + x];
      window[4] = data[(y + 2) * width + x];

      // Медиана из 5 элементов
      nth_element(window, window + 2, window + 5);
      result[y * width + x] = window[2];
    }
  }
  data = result;
}

// Импульсный шум 50%
void Picture::applyImpulseNoise(double noisePercent) {
  if (!isValid()) {
    throw runtime_error("Image is invalid or not loaded");
  }

  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> prob(0.0, 1.0);

  for (auto& px : data) {
    if (prob(gen) < noisePercent) {
      px = (prob(gen) < 0.5) ? 0 : 255;  // черный или белый
    }
  }
}

// Создать тестовое изображение (вертикальный градиент)
void Picture::createTestImage() {
  width = height = 256;
  data.resize(width * height);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      data[y * width + x] = y;  // вертикальный градиент
    }
  }
}

// Доступ к пикселям
unsigned char& Picture::at(int x, int y) { return data[y * width + x]; }

const unsigned char& Picture::at(int x, int y) const {
  return data[y * width + x];
}

int Picture::getWidth() const { return width; }
int Picture::getHeight() const { return height; }