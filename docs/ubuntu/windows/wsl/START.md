
## How to start Visual Studio Code on WSL2 using Ubuntu 20.04.4 distro
> It is amazing easy to start up a Ubuntu instance using WSL2 and using Visual Studio Code

### use case (purpose)
	GIVEN we would like to develop Linux apps using Windows 10/11
	WHEN we install WSL/WSL2 and open it with Visual Studio Code
	THEN we can create applications and have them run on Ubuntu

### Prerequisites
	•	How to install WSL onto Windows 10/11
	•	How to install and use Visual Studio Code (on Windows)
### Resources
	•	Get started with the Windows Subsystem for Linux
	•	Cool WSL (Windows Subsystem for Linux) tips and tricks
	•	How to back up a Windows Subsystem for Linux (WSL) distribution
	•	Developing in WSL
	•	Remote development in WSL
	•	Python setup on the Windows subsystem for Linux (WSL)
### Wish Case
	•	Install WSL onto Windows 10/11
	•	Make sure WSL is using WSL2
	•	Install Visual Studio Code on Windows
	•	Open Start
	•	Search Visual Studio Code 
	•	(Do not select Run as administrator)
	•	The Visual Studio Code editor should come to the forground
	•	Make sure WSL Extension is in it's list of extensions (see diagram to the right)
	•	Otherwise type it in the search box and add it and enable it
	•	Once enabled there will be a small green icon in the bottom left (as shown) click on it
	•	A menu should show up select New WSL Window using Distro...
	•	As there should only be one distro installed select it
	•	With a successful connection open a new Terminal window  
	•	You are now running Visual Studio Code in Windows but are connected to a Ubuntu instance.
	•	Go File → Open Folder and you will see /home/<your username 
	•	This is a directory inside your Ubuntu image
	•	From here (and through the Terminal window) you can do all the How-to's related to Ubuntu (as you are inside a running Ubuntu instance)
### Alternate Case
WSL/Ubuntu instance didn't start
Reboot Windows and try again
### Summary
This has been a basic introduction to using Visual Studio Code with WSL2
### Next Steps
	•	How to debug a Python app using Visual Studio Code (on WSL2)
	•	How to debug a C++ app using Visual Studio Code (on WSL2)
