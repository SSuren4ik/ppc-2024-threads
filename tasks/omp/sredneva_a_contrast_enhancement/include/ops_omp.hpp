// Copyright 2023 Sredneva Anastasiya
#pragma once

#include <string>
#include <vector>

#include "core/task/include/task.hpp"

std::vector<uint8_t> getRandomPicture(int n, int m, uint8_t min, uint8_t max);

class ContrastEnhancement_OMP_Sequential : public ppc::core::Task {
 public:
  explicit ContrastEnhancement_OMP_Sequential(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<uint8_t> input_ = {};
  std::vector<uint8_t> res = {};
  int n{}, m{};
  uint8_t min{}, max{};
};

class ContrastEnhancement_OMP_Parallel : public ppc::core::Task {
 public:
  explicit ContrastEnhancement_OMP_Parallel(std::shared_ptr<ppc::core::TaskData> taskData_)
      : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  std::vector<uint8_t> input_ = {};
  std::vector<uint8_t> res = {};
  int n{}, m{};
  uint8_t min{}, max{};
};
