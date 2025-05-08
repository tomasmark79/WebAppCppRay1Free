// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#include "../src/AppCore.hpp"
#include <gtest/gtest.h>

TEST (DemoLogic, HandlesArguments) {
  const char* argv[] = { "Demo", "--help" };
  EXPECT_EQ (runDemo (2, argv), 0);
}

TEST (DemoLogic, HandlesArgumentsNoLibrary) {
  const char* argv[] = { "Demo", "--omit" };
  EXPECT_EQ (runDemo (2, argv), 0);
}

TEST (DemoLogic, HandlesArgumentsLog2File) {
  const char* argv[] = { "Demo", "--log2file" };
  EXPECT_EQ (runDemo (2, argv), 0);
}
