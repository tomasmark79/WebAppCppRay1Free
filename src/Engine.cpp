// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

// https://www.raylib.com/cheatsheet/cheatsheet.html

#include <Engine/Engine.hpp>
#include <Assets/AssetContext.hpp>
#include <Logger/Logger.hpp>
#include <Utils/Utils.hpp>

#if defined(PLATFORM_WEB)
  #include <emscripten/emscripten.h>
#endif

namespace dotname {

  // Static Raylib attributes definition
  int Engine::screenWidth_;
  int Engine::screenHeight_;
  Texture2D Engine::textureDotNameLogo_;
  Image Engine::imageLogo_;
  Font Engine::fontTopText_;
  int Engine::textSize_;
  int Engine::spacing_;
  Camera3D Engine::camera_;
  Font Engine::fontFpsText_;

  Engine::Engine () {
    LOG_D_STREAM << libName_ << " constructed ..." << std::endl;
    AssetContext::clearAssetsPath ();
  }

  Engine::Engine (const std::filesystem::path& assetsPath) : Engine () {
    if (!assetsPath.empty ()) {
      AssetContext::setAssetsPath (assetsPath);
      LOG_D_STREAM << "Assets path given to the library\n"
                   << "╰➤ " << AssetContext::getAssetsPath () << std::endl;
      auto logo = std::ifstream (AssetContext::getAssetsPath () / "logo.png");

      initRaylib ();
    }
  }

  Engine::~Engine () {
    LOG_D_STREAM << libName_ << " ... destructed" << std::endl;
    UnloadTexture (Engine::textureDotNameLogo_); // Texture unloading
    UnloadFont (Engine::fontTopText_);           // Font unloading
    CloseWindow ();                              // Close window and OpenGL context
    LOG_D_STREAM << "Window closed!" << std::endl;
    AssetContext::clearAssetsPath ();
  }

  void Engine::initRaylibWindow (int width, int height, const std::string& title) {
    Engine::screenHeight_ = height;
    Engine::screenWidth_ = width;
    InitWindow (width, height, title.c_str ());
  }

  void Engine::initRaylib () {

#if defined(PLATFORM_WEB)
    int screenWidth = 800;
    int screenHeight = 450;
#else
    int screenWidth = 1920;
    int screenHeight = 1080;
#endif

    
    Engine::initRaylibWindow (screenWidth, screenHeight, "WebAppCppRay1 WebAppCppRay1 App by DotName");
    if (!IsWindowReady ()) {
      LOG_E_STREAM << "Window initialization failed!" << std::endl;
      return;
    }
    LOG_D_STREAM << "Window initialized successfully!" << std::endl;


    // load and resize DotName logo
    imageLogo_ = LoadImage ((AssetContext::getAssetsPath () / "logo.png").string ().c_str ());
    ImageResize (&Engine::imageLogo_, (float)imageLogo_.width / 1.9f,
                 (float)imageLogo_.height / 1.9f);
    textureDotNameLogo_ = LoadTextureFromImage (Engine::imageLogo_);

    // font
    textSize_ = screenWidth / 50;
    spacing_ = screenWidth / 200;
    fontTopText_ = LoadFont ((AssetContext::getAssetsPath () / "fonts" / "pixemon.ttf").string ().c_str ());
    if (fontTopText_.glyphCount == 0) {
      LOG_E_STREAM << "Font loading failed!" << std::endl;
    } else {
      LOG_D_STREAM << "Font loaded successfully!" << std::endl;
    }


    fontFpsText_ = LoadFont ((AssetContext::getAssetsPath () / "fonts" / "gomarice_g_type.ttf").string ().c_str ());
    if (fontFpsText_.glyphCount == 0) {
      LOG_E_STREAM << "Font loading failed!" << std::endl;
    } else {
      LOG_D_STREAM << "Font loaded successfully!" << std::endl;
    }

    loopRaylib ();
  }

  void Engine::loopRaylib () {

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop (updateDrawFrame, 0, 1);
#else
    SetTargetFPS (120);           // Set our game to run at 60 frames-per-second
    while (!WindowShouldClose ()) // Detect window close button or ESC key
    {
      Engine::updateDrawFrame ();
    }
#endif
  }

  // https://www.raylib.com/cheatsheet/cheatsheet.html

  //----------------------------------------------------------------------------------
  // Module Functions Definition
  //----------------------------------------------------------------------------------
  void Engine::updateDrawFrame (void) {

    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------
    // Initialize the camera
    camera_ = { Vector3{ 30.0f, 20.0f, 30.0f }, // Camera position
                Vector3{ 0.0f, 0.0f, 0.0f },    // Camera target
                Vector3{ 0.0f, 1.0f, 0.0f },    // Camera up vector
                70.0f,                          // Camera field-of-view Y
                CAMERA_PERSPECTIVE };           // Camera projection type

    // Specify the amount of blocks in each direction
    const int numBlocks = 15;

    double time = GetTime ();

    // Calculate time scale for cube position and size
    float scale = (2.0f + (float)sin (time)) * 0.7f;

    // Move camera around the scene
    double cameraTime = time * 0.8;
    camera_.position.x = (float)cos (cameraTime) * 70.0f;
    camera_.position.z = (float)sin (cameraTime) * 70.0f;

    // CubeSize boundaries
    float minCubeSize = 2.7f;
    float maxCubeSize = 3.7f;
    float currCubeSize = 2.7f;

    const char* floatingText = "WebAppCppRay1 WebAppCppRay1 App by";

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing ();

    ClearBackground (RAYWHITE);

    BeginMode3D (camera_);

    // DrawGrid (16, 5.0f);

    for (int x = 0; x < numBlocks; x++) {
      for (int y = 0; y < numBlocks; y++) {
        for (int z = 0; z < numBlocks; z++) {

          // Scale of the blocks depends on x/y/z positions
          float blockScale = (x + y + z) / 30.0f;

          // Scatter makes the waving effect by adding blockScale over time
          float scatter = sinf (blockScale * 20.0f + (float)(time * 4.0f));

          // Calculate the cube position
          Vector3 cubePos = { (float)(x - numBlocks / 2) * (scale * 3.0f) + scatter,
                              (float)(y - numBlocks / 2) * (scale * 2.0f) + scatter,
                              (float)(z - numBlocks / 2) * (scale * 3.0f) + scatter };

          // Pick a color with a hue depending on cube position for the rainbow color effect
          // NOTE: This function is quite costly to be done per cube and frame,
          // pre-catching the results into a separate array could improve performance
          Color cubeColor = ColorFromHSV ((float)(((x + y + z) * 18) % 360), 0.75f, 0.9f);

          // Calculate cube size
          if (currCubeSize > maxCubeSize)
            currCubeSize = minCubeSize;
          else
            currCubeSize += 0.1f;

          float cubeSize = (currCubeSize - scale) * blockScale;

          // And finally, draw the cube!
          if (y > (numBlocks / 2)) {
            DrawCube (cubePos, cubeSize, cubeSize, cubeSize, cubeColor);
          } else {
            DrawCubeWires (cubePos, cubeSize, cubeSize, cubeSize, cubeColor);
          }
        }
      }
    }

    EndMode3D ();

    // Center & Draw Greeting text
    int greeterTextWidth = MeasureTextEx (fontTopText_, floatingText, textSize_, spacing_).x;
    // int greeterTextHeight = MeasureTextEx (fontTopText_, floatingText, fontTopTextSize_, 0).y;
    int greeterTextX = (screenWidth_ - greeterTextWidth) / 2;
    int greeterTextY = 10;
    DrawTextEx (fontTopText_, floatingText, { (float)greeterTextX, (float)greeterTextY }, textSize_,
                spacing_, VIOLET);

    // Center & Draw FPS
    int fps = GetFPS ();
    std::string fpsText = std::to_string (fps) + " fps";
    int textWidth = MeasureText (fpsText.c_str (), textSize_);
    int textX = (screenWidth_ - textWidth) / 2;
    int textY = screenHeight_ - textSize_ - 10;
     DrawTextEx (fontFpsText_, fpsText.c_str (), { (float)textX, (float)textY }, textSize_, spacing_, VIOLET);
    
    // Center & Draw Logo DotName from Assets
    int logoWidth = textureDotNameLogo_.width;
    int logoHeight = textureDotNameLogo_.height;
    int logoX = (screenWidth_ / 2) - logoWidth / 2;
    int logoY = logoHeight + textSize_;
    DrawTexture (textureDotNameLogo_, logoX, logoY, WHITE);

    EndDrawing ();
    //----------------------------------------------------------------------------------
  }

} // namespace dotname