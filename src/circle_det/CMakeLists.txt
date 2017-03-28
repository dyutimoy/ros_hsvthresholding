cmake_minimum_required(VERSION 2.8)
project(circle_det)

find_package(catkin REQUIRED COMPONENTS cv_bridge image_transport message_generation sensor_msgs)

generate_messages(DEPENDENCIES sensor_msgs)

find_package(OpenCV)

include_directories(include ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_executable(circle_det src/circle_det.cpp)
add_dependencies(circle_det ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(circle_det ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

catkin_package(CATKIN_DEPENDS cv_bridge image_transport  sensor_msgs)

