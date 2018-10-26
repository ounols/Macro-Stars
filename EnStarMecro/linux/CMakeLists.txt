cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_STANDARD 11)
SET(RUNTIME_OUTPUT_DIRECTORY ../EnStarMecro/)
project( MacroStars.app )

find_package( OpenCV REQUIRED opencv_core opencv_highgui imgproc )
# include_directories( /usr/local/include/opencv4 )
include_directories(/usr/include)
include_directories(/usr/local/include)
include_directories(../EnStarMecro)

link_directories(/usr/lib)
link_directories(/usr/lib32)
link_directories(/usr/local/lib)

file(GLOB SOURCES_SRC
                        "../EnStarMecro/*.cpp"
                        "../EnStarMecro/*.h")


add_executable( MacroStars.app ${SOURCES_SRC} )



target_link_libraries( MacroStars.app opencv_core )
target_link_libraries( MacroStars.app opencv_highgui )
target_link_libraries( MacroStars.app imgproc )
target_link_libraries( MacroStars.app ${OpenCV_LIBS} )