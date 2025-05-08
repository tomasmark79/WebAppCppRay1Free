// MIT License
// Copyright (c) 2024-2025 Tomáš Mark

#include <Engine/Engine.hpp>
#include <Assets/AssetContext.hpp>
#include <Logger/Logger.hpp>
#include <Utils/Utils.hpp>

#if defined(PLATFORM_WEB)
  #include <emscripten/emscripten.h>
#endif

#include <raylib.h>
#include <math.h> // Required for: sinf()

namespace dotname {

  Texture2D texture;
  int virtualMain ();

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

      virtualMain ();
    }
  }

  Engine::~Engine () {
    LOG_D_STREAM << libName_ << " ... destructed" << std::endl;
  }

  //----------------------------------------------------------------------------------
  // Global Variables Definition
  //----------------------------------------------------------------------------------
  int screenWidth = 800;
  int screenHeight = 450;

  //----------------------------------------------------------------------------------
  // Module Functions Declaration
  //----------------------------------------------------------------------------------
  void UpdateDrawFrame (void); // Update and Draw one frame

  //----------------------------------------------------------------------------------
  // Main Entry Point
  //----------------------------------------------------------------------------------
  int virtualMain () {
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow (screenWidth, screenHeight, "WebAppCppRay1 Demo App by DotName");
    texture = LoadTexture ((AssetContext::getAssetsPath () / "logo.png").string ().c_str ());

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop (UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS (60); // Set our game to run at 60 frames-per-second

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose ()) // Detect window close button or ESC key
    {
      UpdateDrawFrame ();
    }
#endif

    UnloadTexture (texture);
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow (); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
  }

  //----------------------------------------------------------------------------------
  // Module Functions Definition
  //----------------------------------------------------------------------------------
  void UpdateDrawFrame (void) {
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------
    // Initialize the camera
    Camera3D camera = { 0 };
    camera.position = Vector3{ 30.0f, 20.0f, 30.0f }; // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f }; // Camera up vector (rotation towards target)
    camera.fovy = 70.0f;                       // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;    // Camera projection type

    // Specify the amount of blocks in each direction
    const int numBlocks = 15;

    double time = GetTime ();

    // Calculate time scale for cube position and size
    float scale = (2.0f + (float)sin (time)) * 0.7f;

    // Move camera around the scene
    double cameraTime = time * 0.3;
    camera.position.x = (float)cos (cameraTime) * 40.0f;
    camera.position.z = (float)sin (cameraTime) * 40.0f;

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing ();

    ClearBackground (RAYWHITE);

    BeginMode3D (camera);

    DrawGrid (10, 5.0f);

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
          float cubeSize = (2.4f - scale) * blockScale;

          // And finally, draw the cube!
          DrawCube (cubePos, cubeSize, cubeSize, cubeSize, cubeColor);
        }
      }
    }

    EndMode3D ();

    DrawFPS (10, 10);
    EndDrawing ();
    //----------------------------------------------------------------------------------
  }

} // namespace dotname