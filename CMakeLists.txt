cmake_minimum_required(VERSION 3.15)
project(LoadTube C)

set(CMAKE_C_STANDARD 11)

# Chemin Homebrew SDL3
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

# Ajouter l'exécutable
add_executable(LoadTube main.c)

# Lier SDL3 en donnant le chemin complet du dylib
target_link_libraries(LoadTube PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)