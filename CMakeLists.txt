cmake_minimum_required(VERSION 3.3.1)

project(BabaYaga)

set(OpenCV_DIR "C:/.opencv/build")

find_package(OpenCV 4.5.4 REQUIRED)

include_directories(
	${OpenCV_INCLUDE_DIRS},
)

add_executable(BabaYaga
	main.cpp
	preprocess.cpp
	Player.cpp
	Game.cpp
	GameController.cpp
	# You can put here your .h and .cpp files to expand the example.
)

target_link_libraries(BabaYaga
	${OpenCV_LIBS}
)
