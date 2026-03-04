cmake_minimum_required(VERSION 3.16)
project(isdn3000e_lab4)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        polyscope
        GIT_REPOSITORY https://github.com/nmwsharp/polyscope.git
        GIT_TAG master
)

FetchContent_Declare(
        eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 3.4.0
)

FetchContent_Declare(
        libigl
        GIT_REPOSITORY https://github.com/libigl/libigl.git
        GIT_TAG v2.5.0
)

FetchContent_MakeAvailable(polyscope)
FetchContent_MakeAvailable(eigen)
FetchContent_MakeAvailable(libigl)

list(PREPEND CMAKE_PREFIX_PATH "/opt/openrobots")
find_package(pinocchio REQUIRED)

add_executable(isdn3000e_lab4 main.cpp
        tasks/task1.cpp
        tasks/task1.h
        tasks/task2.h
        tasks/task2.cpp
        tasks/task3.cpp
        tasks/task3.h
)

#find_package(PkgConfig REQUIRED)
#
#pkg_check_modules(URDFDOM REQUIRED urdfdom)
#pkg_check_modules(TINYXML2 REQUIRED tinyxml2)
#
#target_include_directories(isdn3000e_lab4 PRIVATE
#        ${URDFDOM_INCLUDE_DIRS}
#        ${TINYXML2_INCLUDE_DIRS}
#)
#
#target_link_libraries(isdn3000e_lab4 PRIVATE
#        ${URDFDOM_LIBRARIES}
#        ${TINYXML2_LIBRARIES}
#)
target_link_libraries(isdn3000e_lab4
        PRIVATE
        polyscope
        Eigen3::Eigen
        igl::core
        pinocchio::pinocchio
)


#add_custom_command(TARGET isdn3000e_lab4 POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/robot
#        $<TARGET_FILE_DIR:isdn3000e_lab4>/robot
#)
#
#
#
#file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets)
#configure_file(
#        ${CMAKE_SOURCE_DIR}/assets/bunny.obj
#        ${CMAKE_BINARY_DIR}/assets/bunny.obj
#        COPYONLY
#)

set(RUNTIME_DIR $<TARGET_FILE_DIR:isdn3000e_lab4>)

add_custom_command(TARGET isdn3000e_lab4 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/robot
            ${RUNTIME_DIR}/robot
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            ${RUNTIME_DIR}/assets
        COMMENT "Copying robot/ and assets/ to runtime directory"
)