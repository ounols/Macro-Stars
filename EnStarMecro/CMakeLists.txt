cmake_minimum_required(VERSION 2.8)
project( MacroStars.app )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(/usr/include)
include_directories(/usr/local/include)
include_directories(EnStarMecro)

link_directories(/usr/lib)
link_directories(/usr/lib32)
link_directories(/usr/local/lib)


file(GLOB SOURCES_SRC
                        "EnStarMecro/*.cpp"
                        "EnStarMecro/*.h")


add_executable( MacroStars.app ${SOURCES_SRC} )


target_link_libraries( MacroStars.app ${OpenCV_LIBS} )
add_custom_command(TARGET MacroStars.app POST_BUILD COMMAND bash "-c" "./b.sh")
