cmake_minimum_required(VERSION 2.8)
project( Detector.app )

include_directories(/usr/include)
include_directories(/usr/local/include)
include_directories(EnStarMecro)

link_directories(/usr/lib)
link_directories(/usr/lib32)
link_directories(/usr/local/lib)


file(GLOB SOURCES_SRC
                        "*.cpp"
                        "*.h")


add_executable( Detector.app ${SOURCES_SRC} )


# add_custom_command(TARGET ADB.app POST_BUILD COMMAND bash "-c" "./b.sh")