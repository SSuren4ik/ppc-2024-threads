// Copyright 2023 Salaev Vladislav
#pragma once

#include <algorithm>
#include <random>
#include <set>
#include <thread>
#include <utility>
#include <vector>
#include <unordered_map>

#include "core/task/include/task.hpp"

namespace SalaevSeq {

class ImageMarking : public ppc::core::Task {
 public:
  explicit ImageMarking(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<std::vector<uint8_t>> source = {};
  std::vector<std::vector<uint32_t>> destination = {};
  uint32_t height{}, width{};
  void resolve_labels(std::vector<int>& labels);
};

}  // namespace SalaevSeq