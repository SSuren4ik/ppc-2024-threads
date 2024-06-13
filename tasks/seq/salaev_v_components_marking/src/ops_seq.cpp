// Copyright 2023 Salaev Vladislav

#include "seq/salaev_v_components_marking/include/ops_seq.hpp"

using namespace SalaevSeq;

bool ImageMarking::validation() {
  internal_order_test();
  height = reinterpret_cast<uint32_t*>(taskData->inputs[0])[0];
  width = reinterpret_cast<uint32_t*>(taskData->inputs[0])[1];
  return (height * width == taskData->inputs_count[1] && taskData->inputs_count[1] == taskData->outputs_count[0]);
}

bool ImageMarking::pre_processing() {
  internal_order_test();
  height = reinterpret_cast<uint32_t*>(taskData->inputs[0])[0];
  width = reinterpret_cast<uint32_t*>(taskData->inputs[0])[1];
  source.resize(height);
  destination.resize(height);
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j)
      source[i].push_back(reinterpret_cast<uint8_t*>(taskData->inputs[1])[i * width + j]);
    destination[i].resize(width, 0);
  }
  return true;
}

bool ImageMarking::run() {
  internal_order_test();
  int nextLabel = 1;
  std::vector<int> labels(height * width, -1);

  // First pass
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      if (source[i][j] == 0) {
        std::set<int> neighboringLabels;
        if (i > 0 && labels[(i - 1) * width + j] != -1) neighboringLabels.insert(labels[(i - 1) * width + j]);
        if (j > 0 && labels[i * width + (j - 1)] != -1) neighboringLabels.insert(labels[i * width + (j - 1)]);

        if (neighboringLabels.empty()) {
          labels[i * width + j] = nextLabel;
          ++nextLabel;
        } else {
          labels[i * width + j] = *neighboringLabels.begin();
          for (auto label : neighboringLabels) {
            std::replace(labels.begin(), labels.end(), label, *neighboringLabels.begin());
          }
        }
      }
    }
  }

  // Second pass
  for (size_t i = 0; i < height; ++i) {
    for (size_t j = 0; j < width; ++j) {
      if (source[i][j] == 0) {
        destination[i][j] = labels[i * width + j];
      }
    }
  }

  return true;
}

bool ImageMarking::post_processing() {
  internal_order_test();
  for (size_t i = 0; i < height; ++i)
    for (size_t j = 0; j < width; ++j)
      reinterpret_cast<uint32_t*>(taskData->outputs[0])[i * width + j] = destination[i][j];
  return true;
}