# CPM - package manager
include(CPM)

# OpenGL
find_package(OpenGL REQUIRED)

# glad
set(GLAD_DIR ${CMAKE_CURRENT_LIST_DIR}/glad)
set(glad_SOURCE_DIR ${GLAD_DIR}/include CACHE INTERNAL "")
add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)

# stb_image
set(STB_IMAGE_DIR ${CMAKE_CURRENT_LIST_DIR}/stb_image)
set(stb_image_SOURCE_DIR ${STB_IMAGE_DIR} CACHE INTERNAL "")
add_library(stb_image STATIC ${STB_IMAGE_DIR}/stb_image.cpp)
target_include_directories(stb_image PRIVATE ${STB_IMAGE_DIR})

#stb_easy_font
set(STB_EASY_FONT_DIR ${CMAKE_CURRENT_LIST_DIR}/stb_easy_font)
set(STB_EASY_FONT_SOURCE_DIR ${STB_EASY_FONT_DIR} CACHE INTERNAL "")
add_library(stb_easy_font STATIC ${STB_EASY_FONT_DIR}/stb_easy_font.cpp)
target_include_directories(stb_easy_font PRIVATE ${STB_EASY_FONT_DIR})

#stb_image_write
set(STB_IMAGE_WRITE_DIR ${CMAKE_CURRENT_LIST_DIR}/stb_image_write)
set(STB_IMAGE_WRITE_SOURCE_DIR ${STB_IMAGE_WRITE_DIR} CACHE INTERNAL "")
add_library(stb_image_write STATIC ${STB_IMAGE_WRITE_DIR}/stb_image_write.cpp)
target_include_directories(stb_image_write PRIVATE ${STB_IMAGE_WRITE_DIR})

#stb_image_resize
set(STB_IMAGE_RESIZE_DIR ${CMAKE_CURRENT_LIST_DIR}/stb_image_resize)
set(STB_IMAGE_RESIZE_SOURCE_DIR ${STB_IMAGE_RESIZE_DIR} CACHE INTERNAL "")
add_library(stb_image_resize STATIC ${STB_IMAGE_RESIZE_DIR}/stb_image_resize2.cpp)
target_include_directories(stb_image_resize PRIVATE ${STB_IMAGE_RESIZE_DIR})

#stb_truetype
set(STB_TRUETYPE_DIR ${CMAKE_CURRENT_LIST_DIR}/stb_truetype)
set(STB_TRUETYPE_SOURCE_DIR ${STB_TRUETYPE_DIR} CACHE INTERNAL "")
add_library(stb_truetype STATIC ${STB_TRUETYPE_DIR}/stb_truetype.cpp)
target_include_directories(stb_truetype PRIVATE ${STB_TRUETYPE_DIR})

# other
CPMAddPackage("gh:assimp/assimp@5.2.5")
CPMAddPackage("gh:glfw/glfw#3.4")
CPMAddPackage("gh:g-truc/glm#1.0.1")
CPMAddPackage("gh:ocornut/imgui@1.90.4")
CPMAddPackage("gh:gabime/spdlog@1.13.0")
CPMAddPackage("gh:kcat/openal-soft#1.23.1")

set(imgui_SOURCE_DIR ${imgui_SOURCE_DIR} CACHE INTERNAL "")
add_library(imgui STATIC ${imgui_SOURCE_DIR}/imgui.cpp
					     ${imgui_SOURCE_DIR}/imgui_demo.cpp
					     ${imgui_SOURCE_DIR}/imgui_draw.cpp
						 ${imgui_SOURCE_DIR}/imgui_tables.cpp
					     ${imgui_SOURCE_DIR}/imgui_widgets.cpp)
						 