
## How to install WSL onto Windows 10/11
> Windows Subsystem for Linux (WSL) is a compatibility layer for running Linux binary executables (in ELF format) natively on Windows 10, Windows 11,[3] and Windows Server 2019. In May 2019, WSL 2 was announced,[4] introducing important changes such as a real Linux kernel,[5] through a subset of Hyper-V features. Since June 2019, WSL 2 is available to Windows 10 customers through the Windows Insider program, including the Home edition

### use case (purpose)
GIVEN we want to use Ubuntu on Windows 10/11 (without VirtualBox)
WHEN we install WSL/WSL2 onto Windows 10/11
THEN we can utilize a real Linux kernel (albeit as a terminal box) 

### Prerequisites
	•	Windows 10/11 installed

### Resources
	•	How to install WSL2 (Windows Subsystem for Linux 2) on Windows 10
	•	Run Ubuntu Linux on Windows with This One Weird Trick
	•	How to install Linux GUI apps with WSL 2 on Windows 10

### Wish Case
	•	Open Start on Windows 10.
	•	Search for Command Prompt, right-click the top result, and select the Run as administrator option.
	•	Type the following command to install the WSL on Windows 10 and press Enter:
	•	wsl --install
	•	Restart your computer to finish the WSL installation on Windows 10
	•	Continue with the Linux distro setup as necessary.

Once you complete the steps, the required Linux components will automatically install the latest version of the Ubuntu Linux distribution.

### Summary
With a few simple steps we installed WSL/WSl2 onto your computer. In the case of WSL2 you now have a real Linux Kernel (albeit in terminal mode) to program serious applications with.

### Next Steps
	•	How to Install a specific distro onto WSL
	•	How to update WSL kernel

