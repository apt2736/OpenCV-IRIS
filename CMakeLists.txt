cmake_minimum_required(VERSION 3.10)
# Nama proyek
project(OpenCV_Learning)
# Mencari OpenCV yang terinstal di sistem
find_package(OpenCV REQUIRED)
# Menambahkan executable dari file main.cpp
add_executable(tugas1 camera.cpp)
add_executable(tugas2 RobotPosition.cpp)
# Menautkan library OpenCV
target_link_libraries(tugas1 ${OpenCV_LIBS})
target_link_libraries(tugas2 ${OpenCV_LIBS})