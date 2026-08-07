cmake_minimum_required(VERSION 3.24) # 3.24+ is best for FetchContent
project(2d_physics LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Enable the compilation database export
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

function(link_sdl3_xc target_name)
    set(SDL3_FRAMEWORK "${CMAKE_SOURCE_DIR}/third_party/SDL3.xcframework")
    if (NOT SDL3_FRAMEWORK)
        message(FATAL_ERROR "SDL3.xcframework not found!")
    endif ()
    if (APPLE)
        # Remove Quarantine Attribute
        execute_process(COMMAND xattr -r -d com.apple.quarantine "${SDL3_FRAMEWORK}" OUTPUT_QUIET ERROR_QUIET)
    endif ()
    message(STATUS ${SDL3_FRAMEWORK})
    target_link_libraries(${target_name} PRIVATE "${SDL3_FRAMEWORK}")
    set_target_properties(${target_name} PROPERTIES
            XCODE_EMBED_FRAMEWORKS "${SDL3_FRAMEWORK}"
            XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY ON
    )
endfunction()

function(link_sdl3_gfx target_name)
    set(SDL3_GFX "${CMAKE_SOURCE_DIR}/third_party/SDL3_gfx")
    target_sources(${target_name} PRIVATE ${SDL3_GFX}/SDL3_gfxPrimitives.c ${SDL3_GFX}/SDL3_rotozoom.c)
    target_include_directories(${target_name} PRIVATE ${SDL3_GFX})
endfunction()

function(copy_asset target_name)
    if (IOS)
        add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/assets"
                "$<TARGET_BUNDLE_DIR:${target_name}>/assets"
                COMMENT "Copying assets folder into iOS app bundle"
        )
        message(STATUS "Copy asset")
    elseif (APPLE AND NOT IOS)
        # For macOS, this copies assets next to the binary
        add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "DEBUG: Running post-build copy..."
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/assets"
                "$<TARGET_FILE_DIR:${target_name}>/assets"
                COMMENT "Copying assets folder next to macOS executable"
        )
    endif ()

endfunction()

function(get_cpp_sources target_name result)
    file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
    set(${result} ${SOURCES} PARENT_SCOPE)
    # debug
    message(STATUS "get_cpp_sources:")
    foreach (src IN LISTS SOURCES)
        message(STATUS "    ${src}")
    endforeach ()
endfunction()

option(USE_SANITIZERS "Enable ASan and UBSan" OFF)
option(BUILD_TESTS "Build unit tests" OFF)

function(enable_sanitizers target_name)
    if (USE_SANITIZERS)
        message(STATUS "Enabling Address and Undefined Behavior Sanitizers for ${target_name}")
        target_compile_options(${target_name} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${target_name} PRIVATE -fsanitize=address,undefined)
    endif ()
endfunction()

# --- Main executable ---
file(GLOB_RECURSE PHYSICS_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/Physics/*.cpp")
file(GLOB SDL_WINDOW_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(2d_physics ${SDL_WINDOW_SOURCES} ${PHYSICS_SOURCES})
link_sdl3_gfx(2d_physics)
link_sdl3_xc(2d_physics)
copy_asset(2d_physics)
enable_sanitizers(2d_physics)

# --- Unit tests ---
# test option (defaults to OFF, so CI/users opt in explicitly)
option(BUILD_TESTS "Build unit tests" OFF)

if (BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.15.3
    )
    FetchContent_MakeAvailable(Catch2)

    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/tests/*.cpp")
    add_executable(physics_tests ${TEST_SOURCES} ${PHYSICS_SOURCES})
    target_include_directories(physics_tests PRIVATE "${CMAKE_SOURCE_DIR}/src")
    target_link_libraries(physics_tests PRIVATE Catch2::Catch2WithMain)

    include(CTest)
    include(Catch)
    catch_discover_tests(physics_tests)
endif ()

