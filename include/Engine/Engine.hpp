// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#ifndef __ENGINE_HPP
#define __ENGINE_HPP

#include <Engine/version.h>
#include <filesystem>
#include <string>

// Public API

namespace dotname {

  class Engine {

    const std::string libName_ = std::string ("Engine v.") + ENGINE_VERSION;

  public:
    Engine ();
    Engine (const std::filesystem::path& assetsPath);
    ~Engine ();
  };

} // namespace dotname

#endif // __ENGINE_HPP