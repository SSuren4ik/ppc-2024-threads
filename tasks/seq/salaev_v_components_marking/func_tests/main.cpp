// Copyright 2023 Salaev Vladislav
#include <gtest/gtest.h>

#include "seq/salaev_v_components_marking/include/ops_seq.hpp"

using namespace SalaevSeq;

TEST(salaev_v_components_marking_seq_functional, correct_input_data) {
  // Create data
  uint32_t height = 5;
  uint32_t width = 5;
  std::vector<uint32_t> dimensions = {height, width};
  std::vector<uint8_t> input = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  std::vector<uint32_t> output(height * width, 0);
  std::vector<uint32_t> expected = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions.data()));
  taskDataSeq->inputs_count.emplace_back(dimensions.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(input.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  ImageMarking testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
}

TEST(salaev_v_components_marking_seq_functional, correct_pre_processing) {
  // Create data
  uint32_t height = 5;
  uint32_t width = 5;
  std::vector<uint32_t> dimensions = {height, width};
  std::vector<uint8_t> input = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
  std::vector<uint32_t> output(height * width, 0);
  std::vector<uint32_t> expected = {0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions.data()));
  taskDataSeq->inputs_count.emplace_back(dimensions.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(input.data()));
  taskDataSeq->inputs_count.emplace_back(input.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  ImageMarking testTaskSequential(taskDataSeq);
  testTaskSequential.validation();
  ASSERT_EQ(testTaskSequential.pre_processing(), true);
}

TEST(salaev_v_components_marking_seq_functional, rectangle) {
  // Create data
  uint32_t height = 4;
  uint32_t width = 8;
  std::vector<uint32_t> dimensions = {height, width};
  std::vector<uint8_t> in = {0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0,
                             0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1};
  std::vector<uint32_t> output(height * width, 0);
  std::vector<uint32_t> expected = {1, 1, 0, 0, 2, 2, 2, 0, 0, 0, 3, 0, 0, 0, 0, 3,
                                    1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 4, 0, 4, 0, 0};

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions.data()));
  taskDataSeq->inputs_count.emplace_back(dimensions.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  ImageMarking testTaskSequential(taskDataSeq);
  testTaskSequential.validation();
  testTaskSequential.pre_processing();
  ASSERT_EQ(testTaskSequential.run(), true);
}

TEST(salaev_v_components_marking_seq_functional, all_one) {
  // Create data
  uint32_t height = 15;
  uint32_t width = 15;
  std::vector<uint32_t> dimensions = {height, width};
  std::vector<uint8_t> in(height * width, 1);
  std::vector<uint32_t> output(height * width, 0);
  std::vector<uint32_t> expected(height * width, 0);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions.data()));
  taskDataSeq->inputs_count.emplace_back(dimensions.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  ImageMarking testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  ASSERT_EQ(testTaskSequential.post_processing(), true);
}

TEST(salaev_v_components_marking_seq_functional, all_zero) {
  // Create data
  uint32_t height = 15;
  uint32_t width = 15;
  std::vector<uint32_t> dimensions = {height, width};
  std::vector<uint8_t> in(height * width, 0);
  std::vector<uint32_t> output(height * width, 0);
  std::vector<uint32_t> expected(height * width, 1);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(dimensions.data()));
  taskDataSeq->inputs_count.emplace_back(dimensions.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(in.data()));
  taskDataSeq->inputs_count.emplace_back(in.size());
  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  ImageMarking testTaskSequential(taskDataSeq);
  ASSERT_EQ(testTaskSequential.validation(), true);
  testTaskSequential.pre_processing();
  testTaskSequential.run();
  testTaskSequential.post_processing();
  ASSERT_EQ(output, expected);
}
