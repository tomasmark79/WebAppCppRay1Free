# MIT License Copyright (c) 2024-2025 Tomáš Mark
function(emscripten target isRequiredHtml)
    if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
        message(STATUS "Emscripten detected")

        #define PLATFORM_WEB
        target_compile_definitions(${target} PRIVATE PLATFORM_WEB)

        if(isRequiredHtml EQUAL 1)
            message(STATUS "html target requested")
            set_target_properties(${target} PROPERTIES SUFFIX ".html")
        endif()

        set_target_properties(
            ${target}
            PROPERTIES
                LINK_FLAGS
                "-s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1"
        )

        if(ENABLE_EMSCRIPTEN_PTHREAD)
            message(STATUS "Emscripten pthread enabled")
            set_target_properties(${target} PROPERTIES COMPILE_FLAGS "-s USE_PTHREADS=1 -pthread")
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-s USE_PTHREADS=1 -pthread")
        endif()
    endif()

    # Checks if OSX and links appropriate frameworks (Only required on MacOS)
    if(APPLE)
        target_link_libraries(${target} PRIVATE "-framework IOKit")
        target_link_libraries(${target} PRIVATE "-framework Cocoa")
        target_link_libraries(${target} PRIVATE "-framework OpenGL")
    endif()

endfunction()
