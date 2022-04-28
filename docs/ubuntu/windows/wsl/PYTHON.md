
## How to debug a Python app using Visual Studio Code (on WSL2)
> The new introduction of the Windows subsystem for Linux allows developers to have full access to a Linux distribution within Windows and without the overhead of a virtual-box. This enhances the Windows experience significantly.

### use case (purpose)
	GIVEN we need to develop Python apps on Ubuntu
	WHEN we use Visual Studio Code (on WSL2) 
	THEN we can develop Python apps using Windows and Ubuntu
### Prerequisites
	•	How to install and use Visual Studio Code (on WSL/WSL2)
	•	How to apt update/upgrade your Ubuntu image instance
	•	Windows 10 is required
	•	No GUI (graphic user interface) required
### Resources
	•	Python setup on the Windows subsystem for Linux (WSL)
### Wish Case
	•	Open Visual Studio Code and connect to a Ubuntu instance
	•	Open a Terminal window (using Visual Studio Code) 
	•	Create a directory HelloWorldPython
	•	In Visual Studio Code go File → Open Folder ..
	•	Select HelloWorldPython
	•	To install Python run the following at the WSL:
	•	sudo apt update && upgrade
	•	sudo apt install python3 python3-pip ipython3
	•	This should install Python support to Ubuntu instance
	•	In the box that says "HELLOWORLDPYTHON" right mouse click and select New File
	•	Type in helloworld.py
	•	In the editor add this: 
	•	print("Hello, world in Python.")
	•	Press Ctrl-S to save the file
	•	Click on the Extensions icon and type @category:debuggers Python in the Search box
	•	Select the first item "Python" and click on the blue Install button
	•	When install click on the editor window and place a breakpoint on the single line of code (see diagram)
	•	Go Run→Start Debugging
	•	If it asks you what to debug select Single File
	•	It should run and stop on the breakpoint
	•	Hit the run arrow to complete the execution
### Summary
You have just added Python to your Ubuntu image and a Python extension to Visual Studio Code. Visual Studio turned around and ran all of that on an Ubuntu image from your Windows based editor.
### Next Steps
	•	How to debug a C++ app using Visual Studio Code (on WSL2)
