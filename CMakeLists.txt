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
# GLM (Declaración en tu posición exacta)
# =========================================================
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW ON
)

# =========================================================
# IMGUI BASE & IMGUI-SFML (Líneas corregidas sin romper tu orden)
# =========================================================
# 1. Download the official core Dear ImGui library first
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.1
    GIT_SHALLOW ON
)

# SINTAXIS MODERNA: Reemplaza las líneas antiguas de Populate para quitar el Warning
FetchContent_MakeAvailable(imgui)

# 2. Expose the downloaded path to imgui-sfml so it finds 'imgui.h'
set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "Path to ImGui core headers" FORCE)

# 3. Now safely download and build the SFML binding bridge
FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
    GIT_TAG v3.0
    GIT_SHALLOW ON
)

# Force ImGui-SFML to build statically integrated with our FetchContent SFML
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(IMGUI_SFML_USE_SYSTEM_IMGUI OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(imgui-sfml)

# =========================================================
# GLM MAKE AVAILABLE (Tu orden exacto que soluciona el error)
# =========================================================
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
# Tappa 06 
# =========================================================
if(APPLE)
    add_executable(Tappa06
        MACOSX_BUNDLE
        Cartella-Tappa06/main.cpp
        external/src/glad.c
    )
else()
    add_executable(Tappa06
        Cartella-Tappa06/main.cpp
        external/src/glad.c
    )
endif()

target_include_directories(Tappa06 PRIVATE
    external/include
)

target_link_libraries(Tappa06 PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    glm::glm       
    OpenGL::GL
)

if(APPLE)
    target_link_libraries(Tappa06 PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# =========================================================
# Tappa 07 
# =========================================================
if(APPLE)
    add_executable(Tappa07
        MACOSX_BUNDLE
        Cartella-Tappa07/main.cpp
        external/src/glad.c
    )
else()
    add_executable(Tappa07
        Cartella-Tappa07/main.cpp
        external/src/glad.c
    )
endif()

target_include_directories(Tappa07 PRIVATE
    external/include
)

target_link_libraries(Tappa07 PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    glm::glm       
    OpenGL::GL
)

if(APPLE)
    target_link_libraries(Tappa07 PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# =========================================================
# Tappa 08 
# =========================================================
if(APPLE)
    add_executable(Tappa08
        MACOSX_BUNDLE
        Cartella-Tappa08/main.cpp   
        external/src/glad.c
    )
else()
    add_executable(Tappa08
        Cartella-Tappa08/main.cpp
        external/src/glad.c
    )
endif()

target_include_directories(Tappa08 PRIVATE
    external/include
)

target_link_libraries(Tappa08 PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    glm::glm       
    OpenGL::GL
)

if(APPLE)
    target_link_libraries(Tappa08 PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# =========================================================
# Tappa 09 
# =========================================================
if(APPLE)
    add_executable(Tappa09
        MACOSX_BUNDLE
        Cartella-Tappa09/main.cpp
        external/src/glad.c
    )
else()
    add_executable(Tappa09
        Cartella-Tappa09/main.cpp
        external/src/glad.c
    )
endif()

target_include_directories(Tappa09 PRIVATE
    external/include
)

target_link_libraries(Tappa09 PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    glm::glm       
    OpenGL::GL
)

if(APPLE)
    target_link_libraries(Tappa09 PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# =========================================================
# Tappa 10 - INTERFAZ GRÁFICA IMGUI (MODO PRO)
# =========================================================
if(APPLE)
    add_executable(Tappa10
        MACOSX_BUNDLE
        Cartella-Tappa10/main.cpp
        external/src/glad.c
    )
else()
    add_executable(Tappa10
        Cartella-Tappa10/main.cpp
        external/src/glad.c
    )
endif()

target_include_directories(Tappa10 PRIVATE
    external/include
)

target_link_libraries(Tappa10 PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    glm::glm       
    OpenGL::GL
    ImGui-SFML::ImGui-SFML
)

if(APPLE)
    target_link_libraries(Tappa10 PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# =========================================================
# SHADERS and TEXTURES 
# =========================================================
file(COPY shaders DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY textures DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})