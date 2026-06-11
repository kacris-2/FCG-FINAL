cmake_minimum_required(VERSION 3.28)


if(POLICY CMP0000)
    set(CMAKE_POLICY_VERSION_MINIMUM "3.5")
endif()

project(fcg-final LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_BUILD_AUDIO OFF CACHE INTERNAL "")
set(SFML_BUILD_NETWORK OFF CACHE INTERNAL "")

include(FetchContent)

# =========================================================
# SFML
# =========================================================
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(SFML)

# =========================================================
# GLM 
# =========================================================
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW ON
)


set(BUILD_SHARED_LIBS_SAVED ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Forzar estático para GLM" FORCE)

FetchContent_MakeAvailable(glm)


set(BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS_SAVED})

# =========================================================
# OPENGL 
# =========================================================
find_package(OpenGL REQUIRED)

# =========================================================
# Tappa 01
# =========================================================
if(APPLE)
    add_executable(Tappa01 MACOSX_BUNDLE Cartella-Tappa01/main.cpp)
else()
    add_executable(Tappa01 Cartella-Tappa01/main.cpp)
endif()

target_link_libraries(Tappa01 PRIVATE sfml-graphics)

if(APPLE)
    target_link_libraries(Tappa01 PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# =========================================================
# Tappa 02
# =========================================================
if(APPLE)
    add_executable(Tappa02 MACOSX_BUNDLE Cartella-Tappa02/main.cpp)
else()
    add_executable(Tappa02 Cartella-Tappa02/main.cpp)
endif()

target_link_libraries(Tappa02 PRIVATE sfml-graphics)

if(APPLE)
    target_link_libraries(Tappa02 PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# =========================================================
# Tappa 03
# =========================================================
if(APPLE)
    add_executable(Tappa03 MACOSX_BUNDLE Cartella-Tappa03/main.cpp)
else()
    add_executable(Tappa03 Cartella-Tappa03/main.cpp)
endif()

target_link_libraries(Tappa03 PRIVATE sfml-graphics)

if(APPLE)
    target_link_libraries(Tappa03 PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# =========================================================
# Tappa 04 
# =========================================================
if(APPLE)
    add_executable(Tappa04
        MACOSX_BUNDLE
        Cartella-Tappa04/main.cpp
        external/src/glad.c
    )
else()
    add_executable(Tappa04
        Cartella-Tappa04/main.cpp
        external/src/glad.c
    )
endif()

target_include_directories(Tappa04 PRIVATE
    external/include
)

target_link_libraries(Tappa04 PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    glm::glm       
    OpenGL::GL
)

if(APPLE)
    target_link_libraries(Tappa04 PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# =========================================================
# Tappa 05 
# =========================================================
if(APPLE)
    add_executable(Tappa05
        MACOSX_BUNDLE
        Cartella-Tappa05/main.cpp   
        external/src/glad.c
    )
else()
    add_executable(Tappa05
        Cartella-Tappa05/main.cpp
        external/src/glad.c
    )
endif()

target_include_directories(Tappa05 PRIVATE
    external/include
)

target_link_libraries(Tappa05 PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    glm::glm       
    OpenGL::GL
)

if(APPLE)
    target_link_libraries(Tappa05 PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# =========================================================
# SHADERS 
# =========================================================
file(COPY shaders DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})