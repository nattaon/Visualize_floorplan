cmake_minimum_required(VERSION 2.8.8)

project(Visualize_floorplan)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# ${QT_INCLUDE_DIRS} ${QT_LIBRARIES} and so on are all Qt4 Macro Definitions!!!!!
# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5OpenGL)
find_package(Qt5Xml)

#message( [qt] ${Qt5Core_INCLUDE_DIRS} )
#message( [qt] ${Qt5Core_LIBRARIES} )
#message( [qt] ${Qt5Widgets_INCLUDE_DIRS} )
#message( [qt] ${Qt5Widgets_LIBRARIES} )

# cpp files
aux_source_directory(. SRC_LIST)

# ui files
qt5_wrap_ui(ui_FILES mainwindow.ui)
# resource files
#qt5_add_resources(qrc_FILES resource.qrc)

message( [Main] ${SRC_LIST} ) # ./main.cpp./mainwindow.cpp
message( [Main] ${ui_FILES} ) # build/ui_mainwindow.h
#message( [Main] ${qrc_FILES} )# build/qrc_resource.cpp

# Tell CMake to create the helloworld executable
add_executable(${PROJECT_NAME} ${SRC_LIST} ${ui_FILES} ${qrc_FILES})

qt5_use_modules(${PROJECT_NAME} Core Widgets OpenGL Xml Gui)

# Use the Widgets module from Qt 5.
#qt5_use_modules(helloworld Widgets)

# link other libraries
#target_link_libraries (${PROJECT_NAME} ${SPEC_OPENCV_LIBS})
