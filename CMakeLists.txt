cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CursoOpenCV_UHU)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(p0_mat p0_mat.cpp)
target_link_libraries(p0_mat ${OpenCV_LIBS})

add_executable(p1_videoread p1_videoread.cpp)
target_link_libraries(p1_videoread ${OpenCV_LIBS})

add_executable(p1_videowrite p1_videowrite.cpp)
target_link_libraries(p1_videowrite ${OpenCV_LIBS})

add_executable(p1_collage p1_collage.cpp)
target_link_libraries(p1_collage ${OpenCV_LIBS})

add_executable(p2_canny p2_canny.cpp)
target_link_libraries(p2_canny ${OpenCV_LIBS})

add_executable(p2_blurring p2_blurring.cpp)
target_link_libraries(p2_blurring ${OpenCV_LIBS})

add_executable(p2_thresh p2_thresh.cpp)
target_link_libraries(p2_thresh ${OpenCV_LIBS})

add_executable(p3_hsv p3_hsv.cpp)
target_link_libraries(p3_hsv ${OpenCV_LIBS})

add_executable(p3_chroma p3_chroma.cpp)
target_link_libraries(p3_chroma ${OpenCV_LIBS})

add_executable(p4_contours p4_contours.cpp)
target_link_libraries(p4_contours ${OpenCV_LIBS})

add_executable(p4_conveyor p4_conveyor.cpp)
target_link_libraries(p4_conveyor ${OpenCV_LIBS})

add_executable(p4_ball p4_ball.cpp)
target_link_libraries(p4_ball ${OpenCV_LIBS})

add_executable(p5_face p5_face.cpp)
target_link_libraries(p5_face ${OpenCV_LIBS})

add_executable(p5_faceblur p5_faceblur.cpp)
target_link_libraries(p5_faceblur ${OpenCV_LIBS})

add_executable(p6_aruco p6_aruco.cpp)
target_link_libraries(p6_aruco ${OpenCV_LIBS})

add_executable(p7_qr p7_qr.cpp)
target_link_libraries(p7_qr ${OpenCV_LIBS})

