cmake_minimum_required(VERSION 3.22)
project(LSM_RANSAC)

set(CMAKE_CXX_STANDARD 14)

add_executable(ransac main.cpp Method/RANSAC.hpp Method/LSM.hpp
        Model/Linear.cpp
        Model/Plane.cpp)

find_package(camel-euclid-vector REQUIRED)
find_package( OpenCV REQUIRED )

find_package(Python2 COMPONENTS Development NumPy)
target_include_directories(ransac PRIVATE ${Python2_INCLUDE_DIRS} ${Python2_NumPy_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
target_link_libraries(ransac Python2::Python Python2::NumPy ${OpenCV_LIBS} camel-euclid-vector)
