# Engine

A framework for experimenting with geometry and other things

The framework uses OpenGL-OpenCL interoperability for graphics and other things.
It also uses SDL2 to setup the window, draw text, and load images.

If you have linux you can just install the dependencies like this:
```
	# sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libglew-dev nvidia-opencl-dev
```
If you don't have nvidia the opencl dependency would likely look different.
You can find it with
```
	# apt-cache search opencl
```
Then you can just get make and run the program
```
	# sudo apt install make
	# make run
```

TODO
----
Fix multiple windows

Get Opencl to work cross-platform

Setup GUI environment
