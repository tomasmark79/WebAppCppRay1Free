// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#include "../src/AppCore.hpp"
#include <gtest/gtest.h>

TEST (WebAppCppRay1Logic, HandlesArguments) {
  const char* argv[] = { "WebAppCppRay1", "--help" };
  EXPECT_EQ (runWebAppCppRay1 (2, argv), 0);
}

TEST (WebAppCppRay1Logic, HandlesArgumentsNoLibrary) {
  const char* argv[] = { "WebAppCppRay1", "--omit" };
  EXPECT_EQ (runWebAppCppRay1 (2, argv), 0);
}

TEST (WebAppCppRay1Logic, HandlesArgumentsLog2File) {
  const char* argv[] = { "WebAppCppRay1", "--log2file" };
  EXPECT_EQ (runWebAppCppRay1 (2, argv), 0);
}
