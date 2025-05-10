# MIT License Copyright (c) 2024-2025 Tomáš Mark

function(emscripten target isRequiredHtml)
    if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
        message(STATUS "Emscripten environment detected")

        # define PLATFORM_WEB
        target_compile_definitions(${target} PRIVATE PLATFORM_WEB)

        set_target_properties(${target} PROPERTIES OUTPUT_NAME "${target}")

        if(isRequiredHtml EQUAL 1)
            message(STATUS "html target requested")
            set_target_properties(${target} PROPERTIES SUFFIX ".html")
        endif()

        # available switches
        set(_wasm "-s WASM=1")
        set(_o3 "-O3")
        set(_assertions "-s ASSERTIONS=1")
        set(_asyncify "-s ASYNCIFY")
        set(_gl_egpaddress "-s GL_ENABLE_GET_PROC_ADDRESS=1")
        set(_raylib "-s USE_GLFW=3")
        set(_sdl2_sdl2 "-s USE_SDL=2")
        set(_sdl2_image "-s USE_SDL_IMAGE=2")
        set(_sdl2_ttf "-s USE_SDL_TTF=2")
        set(_sdl2_net "-s USE_SDL_NET=2")
        set(_sdl2_mixer "-s USE_SDL_MIXER=2")
        set(_pthread "-s USE_PTHREADS=1 -pthread")

        # manually set flags is the best way
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS "${_pthread}")
        set_target_properties(${target} PROPERTIES LINK_FLAGS
                                                   "${_raylib} ${_wasm} ${_o3} ${_pthread} --preload-file ../../../../assets@share/${target}/assets")

    endif()

    # Checks if OSX and links appropriate frameworks (Only required on MacOS)
    if(APPLE)
        target_link_libraries(${target} PRIVATE "-framework IOKit")
        target_link_libraries(${target} PRIVATE "-framework Cocoa")
        target_link_libraries(${target} PRIVATE "-framework OpenGL")
    endif()

endfunction()
