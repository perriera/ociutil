
## How to install a specific distro onto WSL
> You need to be able to backup and restore your distros (within the WSL) environment.

### use case (purpose)
	GIVEN we need to be able to backup and restore our work
	WHEN we run the WSL commands for export and import
	THEN we can rest assured that our distros can be backed up

### Prerequisites
	•	How to install WSL onto Windows 10/11
	•	Not be running the WSL/WSL2 instance 

### Wish Case
	•	Open up a PowerShell or a CMD.exe instance in Windows
	•	Execute wsl -l -v to get a full list of the distribution names. This is important because you need to match exactly, and if you have "Ubuntu-20.04" you will need to use that. Just "Ubuntu" won't work.
	•	C:\WINDOWS\system32>wsl -l -v
	•	  NAME            STATE           VERSION
	•	* Ubuntu-20.04    Running         2
	•	
	•	C:\WINDOWS\system32>
	•	Execute cd command to navigate into the directory you want to save your export to. It will save to whatever directory you're at the command line in, which by default will be your users folder.
	•	mkdir C:/Users/<username>/Backups
	•	cd C:/Users/<username>/Backups 
	•	Once you're happy with the above, the export command follows this template:
	•	 wsl --export Ubuntu-20.04 ubuntubackup-20220402.tar
	•	Syntax: wsl --export (distribution) (filename.tar)
Make sure to add the .tar extension as well to your filename otherwise you may run into some issues.

### Alternate Case
Importing a previously exported .tar file
If you're using the export feature to backup one of your WSL Linux distributions then there's a good chance you might also want to use it at some point. Fortunately, should the moment arise, there is an equally simple command template to import your .tar files to a specified install location.
The exact command follows this syntax:
 wsl --import (distribution) (install location) (file location and filename) 
Where an example would be:
wsl --import Ubuntu-20.04 
C:\Users\pande219\AppData\Local\Packages\Ubuntu 
C:\Users\pande219\Backups\ubuntubackup-20220402.tar
Where pande219 is substituted for your username
To check it imported correctly, run the wsl --list command and you should now see your newly imported distribution.

### Alternate Case
Removing a Linux distro from WSL/WSL2
To remove a distro:
wsl -l -v
wsl --unregister Ubuntu-20.04 
Alternate Case
Adding a Linux distro to WSL/WSL2
To add a distro:
wsl --list --online
The following is a list of valid distributions that can be installed.
Install using 'wsl --install -d <Distro>'.

NAME            FRIENDLY NAME
Ubuntu          Ubuntu
Debian          Debian GNU/Linux
kali-linux      Kali Linux Rolling
openSUSE-42     openSUSE Leap 42
SLES-12         SUSE Linux Enterprise Server v12
Ubuntu-16.04    Ubuntu 16.04 LTS
Ubuntu-18.04    Ubuntu 18.04 LTS
Ubuntu-20.04    Ubuntu 20.04 LTS

### Summary
This has been a description of how to export and import any WSL distro you may have. 

### Next Steps
	•	How to install and use Visual Studio Code (on WSL/WSL2)
