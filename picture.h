#ifndef PICTURE_H
#define PICTURE_H

#include <algorithm>
#include <fstream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

class Picture {
 public:
  Picture() = default;

  bool loadRaw256(const string& filename);
  bool loadFromFile(const string& filename);  // для PNG/JPG
  bool saveToFile(const string& filename);  // для сохранения PNG/JPG
  bool isValid() const;

  void applyVerticalMedian5x1();  // вертикальный медианный 5x1
  void applyImpulseNoise(double noisePercent = 0.50);  // 50% шум

  unsigned char& at(int x, int y);
  const unsigned char& at(int x, int y) const;

  int getWidth() const;
  int getHeight() const;

  void createTestImage();  // создать тестовое изображение

 private:
  int width = 0;
  int height = 0;
  vector<unsigned char> data;
};

#endif