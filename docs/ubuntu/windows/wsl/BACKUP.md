
## How to backup/restore Linux distros on WSL/WSL2
> To start using WSL2 or covert WSL distros to WSL2, use these steps

### use case (purpose)
	GIVEN that we want to benefit from using a native Linux kernel on Windows
	WHEN we configure WSL to operate as WSL2
	THEN we get all the benefits that come from a native Linux kernel

### Prerequisites
	•	How to install WSL onto Windows 10/11
### Resources
	•	Enable Windows Subsystem for Linux 2
### Wish Case
	•	Download this WSL 2 kernel update (required)
	•	Double-click the wsl_update_x64.msi file and apply the update
	•	Open Start
	•	Search for PowerShell, right-click the top result, and select the Run as administrator option
	•	Type the following command to set Windows Subsystem for Linux 2 your default architecture for new distros that you install and press Enter:
	•	wsl --set-default-version 2
	•	See diagram to the right
	•	(Optional) Type the following command to convert the distro from WSL to WSL 2 and press Enter:
	•	wsl --set-version Ubuntu 2
	•	In the command, make sure to change Ubuntu for the distro’s name you want to convert. If you do not know the name of the distro, use the wsl -l -v command.
	•	To confirm distros are using the Windows Subsystem for Linux 2, use these steps:
	•	wsl --list --verbose
	•	See diagram to the right
After you complete the steps, you will know if the process has been completed successfully or if you need to troubleshoot any of the steps.

### Alternate Case
WSL2 already installed
It may not be necessary to download anything as WSL2 may already be on your system. To confirm this execute wsl --list --verbose in a PowerShell first.

### Summary
This has been a brief description on how to make sure that WSL2 is in use on your Windows 10/11 platform.

### Next Steps
	•	How to Install a specific distro onto WSL
	•	How to install and use Visual Studio Code (on Windows)
	•	How to install and use Visual Studio Code (on WSL/WSL2)

