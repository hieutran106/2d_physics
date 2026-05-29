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

get_cpp_sources(2d_physics SOURCES)
add_executable(2d_physics ${SOURCES})
link_sdl3_gfx(2d_physics)
link_sdl3_xc(2d_physics)
copy_asset(2d_physics)

option(USE_SANITIZERS "Enable ASan and UBSan" OFF)
if (USE_SANITIZERS)
    message(STATUS "Building with Address and Undefined Behavior Sanitizers")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
endif ()