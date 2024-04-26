// Copyright 2024 Zorin Oleg
#include <gtest/gtest.h>

#include <vector>

#include "tbb/zorin_o_crs_matmult/include/crs_matmult_tbb.hpp"

TEST(Zorin_O_CRS_MatMult_TBB, incorrect_matrix_sizes) {
  // Create data
  int p = 11;
  int q = 10;
  int s = 11;
  int r = 9;
  std::vector<double> lhs_in(p * q);
  std::vector<double> rhs_in(s * r);
  std::vector<double> out(p * r);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(lhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(p);
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(rhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(s);
  taskDataTBB->inputs_count.emplace_back(r);
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(p);
  taskDataTBB->outputs_count.emplace_back(r);

  // Create Task
  CRSMatMult testTaskTBB(taskDataTBB);
  ASSERT_FALSE(testTaskTBB.validation());
}

TEST(Zorin_O_CRS_MatMult_TBB, zero_rhs_matrix) {
  // Create data
  int p = 11;
  int q = 10;
  int r = 9;
  std::vector<double> lhs_in = getRandomMatrix(p, q);
  std::vector<double> rhs_in(q * r);
  std::vector<double> out(p * r);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(lhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(p);
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(rhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs_count.emplace_back(r);
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(p);
  taskDataTBB->outputs_count.emplace_back(r);

  // Create Task
  CRSMatMult testTaskTBB(taskDataTBB);
  ASSERT_TRUE(testTaskTBB.validation());
  ASSERT_TRUE(testTaskTBB.pre_processing());
  ASSERT_TRUE(testTaskTBB.run());
  ASSERT_TRUE(testTaskTBB.post_processing());
  for (int i = 0; i < p * r; ++i) {
    EXPECT_DOUBLE_EQ(out[i], 0.0);
  }
}

TEST(Zorin_O_CRS_MatMult_TBB, zero_lhs_matrix) {
  // Create data
  int p = 11;
  int q = 10;
  int r = 9;
  std::vector<double> lhs_in(p * q);
  std::vector<double> rhs_in = getRandomMatrix(q, r);
  std::vector<double> out(p * r);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(lhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(p);
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(rhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs_count.emplace_back(r);
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(p);
  taskDataTBB->outputs_count.emplace_back(r);

  // Create Task
  CRSMatMult testTaskTBB(taskDataTBB);
  ASSERT_TRUE(testTaskTBB.validation());
  ASSERT_TRUE(testTaskTBB.pre_processing());
  ASSERT_TRUE(testTaskTBB.run());
  ASSERT_TRUE(testTaskTBB.post_processing());
  for (int i = 0; i < p * r; ++i) {
    EXPECT_DOUBLE_EQ(out[i], 0.0);
  }
}

TEST(Zorin_O_CRS_MatMult_TBB, identity_rhs_matrix) {
  // Create data
  int p = 11;
  int q = 10;
  int r = q;
  std::vector<double> lhs_in = getRandomMatrix(p, q);
  std::vector<double> rhs_in = getIdentityMatrix(q);
  std::vector<double> out(p * r);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(lhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(p);
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(rhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs_count.emplace_back(r);
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(p);
  taskDataTBB->outputs_count.emplace_back(r);

  // Create Task
  CRSMatMult testTaskTBB(taskDataTBB);
  ASSERT_TRUE(testTaskTBB.validation());
  ASSERT_TRUE(testTaskTBB.pre_processing());
  ASSERT_TRUE(testTaskTBB.run());
  ASSERT_TRUE(testTaskTBB.post_processing());
  for (int i = 0; i < p * r; ++i) {
    EXPECT_DOUBLE_EQ(out[i], lhs_in[i]);
  }
}

TEST(Zorin_O_CRS_MatMult_TBB, identity_lhs_matrix) {
  // Create data
  int p = 11;
  int q = p;
  int r = 10;
  std::vector<double> lhs_in = getIdentityMatrix(p);
  std::vector<double> rhs_in = getRandomMatrix(q, r);
  std::vector<double> out(p * r);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(lhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(p);
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(rhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs_count.emplace_back(r);
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(p);
  taskDataTBB->outputs_count.emplace_back(r);

  // Create Task
  CRSMatMult testTaskTBB(taskDataTBB);
  ASSERT_TRUE(testTaskTBB.validation());
  ASSERT_TRUE(testTaskTBB.pre_processing());
  ASSERT_TRUE(testTaskTBB.run());
  ASSERT_TRUE(testTaskTBB.post_processing());
  for (int i = 0; i < p * r; ++i) {
    EXPECT_DOUBLE_EQ(out[i], rhs_in[i]);
  }
}

TEST(Zorin_O_CRS_MatMult_TBB, matmult_with_answer) {
  // Create data
  int p = 4;
  int q = 5;
  int r = 4;
  std::vector<double> lhs_in{
      0, 10, 0, 0, 0, 0, 5, 3, 0, 0, 1, -1, -1, 0, 0, 0, 0, -5, 0, 20,
  };
  std::vector<double> rhs_in{
      1, 1, 0, 0, 0, 5, 9, 9, 0, 0, 0, -1, 13, 7, 0, 0, 7, 0, 8, 0,
  };
  std::vector<double> ans{
      0, 50, 90, 90, 0, 25, 45, 42, 1, -4, -9, -8, 140, 0, 160, 5,
  };
  std::vector<double> out(p * r);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(lhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(p);
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(rhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs_count.emplace_back(r);
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(p);
  taskDataTBB->outputs_count.emplace_back(r);

  // Create Task
  CRSMatMult testTaskTBB(taskDataTBB);
  ASSERT_TRUE(testTaskTBB.validation());
  ASSERT_TRUE(testTaskTBB.pre_processing());
  ASSERT_TRUE(testTaskTBB.run());
  ASSERT_TRUE(testTaskTBB.post_processing());
  for (int i = 0; i < p * r; ++i) {
    EXPECT_DOUBLE_EQ(out[i], ans[i]);
  }
}

TEST(Zorin_O_CRS_MatMult_TBB, special_matmult) {
  // Create data
  int p = 11;
  int q = 10;
  int r = 11;
  std::vector<double> lhs_in(p * q);
  for (int i = 0; i < p; ++i) {
    if (i % 2 == 0)
      for (int j = 0; j < q; ++j) {
        lhs_in[i * q + j] = 1.0;
      }
  }
  std::vector<double> rhs_in(q * r);
  for (int i = 0; i < q; ++i) {
    for (int j = 0; j < r; ++j) {
      if (j % 2 == 0) rhs_in[i * r + j] = 1.0;
    }
  }
  std::vector<double> out(p * r);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataTBB = std::make_shared<ppc::core::TaskData>();
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(lhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(p);
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs.emplace_back(reinterpret_cast<uint8_t *>(rhs_in.data()));
  taskDataTBB->inputs_count.emplace_back(q);
  taskDataTBB->inputs_count.emplace_back(r);
  taskDataTBB->outputs.emplace_back(reinterpret_cast<uint8_t *>(out.data()));
  taskDataTBB->outputs_count.emplace_back(p);
  taskDataTBB->outputs_count.emplace_back(r);

  // Create Task
  CRSMatMult testTaskTBB(taskDataTBB);
  ASSERT_TRUE(testTaskTBB.validation());
  ASSERT_TRUE(testTaskTBB.pre_processing());
  ASSERT_TRUE(testTaskTBB.run());
  ASSERT_TRUE(testTaskTBB.post_processing());
  for (int i = 0; i < p; ++i) {
    for (int j = 0; j < r; ++j) {
      if (i % 2 == 0 && j % 2 == 0)
        EXPECT_DOUBLE_EQ(out[i * r + j], q);
      else
        EXPECT_DOUBLE_EQ(out[i * r + j], 0.0);
    }
  }
}