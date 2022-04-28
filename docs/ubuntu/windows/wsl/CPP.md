
## How to debug a C++ app using Visual Studio Code (on WSL2)
> Running and debugging your first program on Ubuntu running through WSL/WSL2 was your first experience in debugging using WSL/WSL2. In this chapter we are going to do pretty much the same thing except with a modern C++17 application.

### use case (purpose)
	GIVEN that we want to develop C++ apps on WSL/WSL2 using Visual Studio Code
	WHEN we start an Ubuntu image using Visual Studio Code on WSL/WSL2
	THEN we can develop C++ apps as if we were on native Linux platform
### Prerequisites
	•	How to debug a Python app using Visual Studio Code (on WSL2)
### Wish Case
	•	Open Visual Studio Code on Windows 
	•	Connect to a Linux image (over WSL/WSL2)
	•	Go to this web page: https://github.com/perriera/extras
	•	sudo apt update
	•	sudo apt install build-essential libtool autotools-dev automake 
	•	sudo apt install pkg-config git clangd cppcheck clang-tidy 
	•	pip inst
	•	vi ~/.bashrc
	•	Added this to the end of .bashrc
	•	export PATH=/home/perry/.local/bin:${PATH}
	•	Exit out of Visual Studio Code and restart it again
	•	Execute cd ~
	•	Execute git clone https://github.com/perriera/extras.git
	•	Go File→Open Folder ... select /home/<username>/extras
	•	Press Ctrl-Shift-B
	•	That should result in a good compile.
	•	In the Terminal box type:
	•	build/run-unittests-extras
	•	That should execute but a single error should be detected
	•	Using the File Explorer, navigate to that source code (see diagram to the right)
	•	Attempt to put a breakpoint on line 100 (nothing should show up)
	•	Go to Extensions and type C++ in the search box (see diagram to the right)
	•	Select and install both C++ Intellisense and C++ Extension Pack
	•	Click Yes & Yes to CMake Tools Extension
	•	Now go back to dock_exceptions.cpp and place a breakpoint on line 100
	•	On the Visual Studio Code menu go Run→Start Debugging
	•	An exception dialog should appear indicating that you do not have gdb installed
	•	In the Terminal install gdb:
	•	sudo apt install gdb
	•	On the Visual Studio Code menu go Run→Start Debugging
	•	The code should run and stop on line 100 (see diagram to the right)
	•	Congratulations (you have just made it to the land of the living)
Now it turns out that line 100 of dock_exceptions.cpp is operating as expected. However, there is a legitimate exception going off for a perfectly logical reason. Let's see if you can figure out what that reason is (and in the meantime, marvel over the responsiveness of Visual Studio Code in this configuration).
### Alternate Case
Can't open workspace
Click Retry
### Summary
We have just demonstrated how much simpler, better, faster and generally speaking WAY MORE COOL it is to use Visual Studio Code on WSL/WSL2. 
### Next Steps
	•	How to use the ...

