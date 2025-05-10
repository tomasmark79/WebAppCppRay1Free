// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#ifndef __ENGINE_HPP
#define __ENGINE_HPP

#include <Engine/version.h>
#include <filesystem>
#include <string>

#if defined(PLATFORM_WEB)
  #include <emscripten/emscripten.h>
#endif

#include "raylib.h"
#include <math.h> // Required for: sinf()

// Public API

namespace dotname {
  class Engine {
    const std::string libName_ = std::string ("Engine v.") + ENGINE_VERSION;

  public:
    Engine ();
    Engine (const std::filesystem::path& assetsPath);
    ~Engine ();

  private:
    // Static Raylib attributes declaration
    static int screenWidth_;
    static int screenHeight_;
    static Texture2D textureDotNameLogo_;
    static Image imageLogo_;
    static Font fontTopText_;
    static int textSize_;
    static int spacing_;
    static Camera3D camera_;

    static void initRaylibWindow (int width, int height, const std::string& title);
    static void initRaylib ();
    static void loopRaylib ();
    static void updateDrawFrame ();
  };

} // namespace dotname

#endif // __ENGINE_HPP