
## How to Enable Windows Subsystem 2 for Linux
> To install WSL with a specific distro on Windows 10, use these steps

### use case (purpose)
	GIVEN we may need to install a specific distro 
	WHEN we use WSL to install a specific distro 
	THEN we can use Ubuntu for specific purposes

### Prerequisites
	•	How to install WSL onto Windows 10/11

### Resources
	•	Run Ubuntu Linux on Windows with This One Weird Trick
	•	How to install Linux GUI apps with WSL 2 on Windows 10

### Wish Case
	•	Open Start
	•	Search for Command Prompt, right-click the top result, and select the Run as administrator option.
	•	Type the following command to install the WSL with a specific distro on Windows 10 and press Enter:
	•	wsl --install -d DISTRO-NAME
	•	In the command, remember to replace “DISTRO-NAME” for the name of the distro you want to install, such as Debian.
	•	Restart your computer.
	•	Continue with the Linux distro setup as necessary.
After you complete the steps, the Windows Subsystem for Linux 2 components will install with the distro of Linux you specified. 

### Summary
This has shown how to install a specific distro using WSL onto Windows 10/11

### Next Steps
	•	How to update WSL kernel
