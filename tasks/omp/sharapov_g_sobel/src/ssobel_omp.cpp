// Copyright 2024 Sharapov Georgy

#include "omp/sharapov_g_sobel/include/ssobel_omp.hpp"

#include <omp.h>

#include <cmath>
#include <random>
#include <thread>

SSobelOmp::GrayScale SSobelOmp::getPixel(const std::vector<SSobelOmp::GrayScale>& image, size_t x, size_t y,
                                         size_t width, size_t height) {
  if (x > width - 1) x = width - 1;
  if (x < 0) x = 0;
  if (y > height - 1) y = height - 1;
  if (y < 0) y = 0;

  return image[y * width + x];
}

std::vector<SSobelOmp::RGB> SSobelOmp::generateColorImage(size_t width, size_t height, size_t seed) {
  std::vector<SSobelOmp::RGB> image;
  image.reserve(width * height);

  std::mt19937 gen(static_cast<unsigned int>(seed));
  std::uniform_int_distribution<> rgb(0, 255);

  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      SSobelOmp::RGB pixel;
      pixel.r = static_cast<uint8_t>(rgb(gen));
      pixel.g = static_cast<uint8_t>(rgb(gen));
      pixel.b = static_cast<uint8_t>(rgb(gen));
      image.push_back(pixel);
    }
  }

  return image;
}

std::vector<SSobelOmp::GrayScale> SSobelOmp::convertToGrayScale(const std::vector<SSobelOmp::RGB>& colorImage,
                                                                size_t width, size_t height) {
  std::vector<SSobelOmp::GrayScale> grayImage(width * height);

  int sizeImg = static_cast<int>(width * height);

#pragma omp parallel for
  for (int index = 0; index < sizeImg; ++index) {
    const auto& pixel = colorImage[index];
    grayImage[index].value = static_cast<uint8_t>(0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b);
  }
  return grayImage;
}

std::vector<SSobelOmp::GrayScale> SSobelOmp::SobelOperatorOmp(const std::vector<SSobelOmp::GrayScale>& grayImage,
                                                              size_t width, size_t height) {
  const int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  const int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

  std::vector<GrayScale> resultImg(width * height);
  int sizeImg = static_cast<int>(width * height);

#pragma omp parallel for
  for (int index = 0; index < sizeImg; ++index) {
    int i = index / width;
    int j = index % width;
    int sumX = 0;
    int sumY = 0;

    for (int x = -1; x <= 1; x++) {
      for (int y = -1; y <= 1; y++) {
        int posX = static_cast<int>(j) + y;
        int posY = static_cast<int>(i) + x;
        auto pixel = getPixel(grayImage, posX, posY, width, height);

        sumX += Gx[x + 1][y + 1] * pixel.value;
        sumY += Gy[x + 1][y + 1] * pixel.value;
      }
    }

    int sum = std::sqrt(sumX * sumX + sumY * sumY);

    sum = sum >= 200 ? 255 : 0;

    resultImg[index] = GrayScale{static_cast<uint8_t>(sum)};
  }

  return resultImg;
}

void SSobelOmp::printPixel(const std::vector<GrayScale>& image, int width, int height) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      GrayScale pixel = getPixel(image, x, y, width, height);
      std::cout << static_cast<int>(pixel.value) << "\t";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

bool SSobelOmp::validation() {
  try {
    internal_order_test();
  } catch (...) {
    return false;
  }

  return (taskData->inputs_count[0] == taskData->outputs_count[0] &&
          taskData->inputs_count[1] == taskData->outputs_count[1]);
}

bool SSobelOmp::pre_processing() {
  try {
    internal_order_test();

    imgWidth = taskData->inputs_count[0];
    imgHeight = taskData->inputs_count[1];
    imgSize = imgWidth * imgHeight;

    colored_img.reserve(imgSize);
    uint8_t* rawData = taskData->inputs[0];
#pragma omp parallel for
    for (int i = 0; i < imgSize; ++i) {
      colored_img[i] = (RGB{rawData[i * 3], rawData[i * 3 + 1], rawData[i * 3 + 2]});
    }

    grayscale_img = SSobelOmp::convertToGrayScale(colored_img, imgWidth, imgHeight);
  } catch (...) {
    return false;
  }
  return true;
}

bool SSobelOmp::run() {
  try {
    internal_order_test();

    result = SSobelOmp::SobelOperatorOmp(grayscale_img, imgWidth, imgHeight);
  } catch (...) {
    return false;
  }
  return true;
}

bool SSobelOmp::post_processing() {
  try {
    internal_order_test();

    int sizeImg = static_cast<int>(grayscale_img.size());

#pragma omp parallel for
    for (int i = 0; i < sizeImg; ++i) {
      auto* pixel = reinterpret_cast<SSobelOmp::GrayScale*>(taskData->outputs[0] + i);
      *pixel = result[i];
    }
  } catch (...) {
    return false;
  }
  return true;
}
