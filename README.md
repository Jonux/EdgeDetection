# Edge detection school project
© Joel Huttunen 2015

Project implements Canny edge detection method. All operations are done by hand. Only image loading is using Cimg-library. Ready image-modifying methods were not allowed to use in this project work. The project also provides basic commandline tools to generate different pictures.

***********************

### Install guide (Windows)

- Start Visual Studio 2015 -project
- Update files linking (Modify absolute paths), if needed.

********************

### Install guide (Unix)

Install depedency cimg-library version 1.6.4 or later. 
sudo apt-get install cimg-dev

Build the software:

g++ --std=c++11 -Wall -O2 -g -I./Headers Source/EdgeAlgorithms.cpp Source/main.cpp Source/Sobel.cpp -L/usr/X11R6/lib -lm -lpthread -lX11 

Run the software:
./a.out --help

