
## How to add local file access to Ubuntu (using VirtualBox)
> Do at least one LWV (or clone) for your Ubuntu image before adding local file access to it as there are complications related to VirtualBox when accessing shared files from the Ubuntu image.

### use case
	GIVEN we need to access local files occasionally from the Ubuntu image
	WHEN we setup Shared files on Windows and add them to VirtualBox
	THEN the Ubuntu image can read/write to that Shared directory

### Prerequisites
	•	How to clone Ubuntu (on VirtualBox)
	•	How to create a Ubuntu image LWV, (last working version)
	•	How to add VirtualBoxAdditions.iso to VirtualBox 6
	•	How to zoom in and zoom out your Ubuntu image
	•	How to setup shared folders in VirtualBox 6

### External Resources
	•	How to Install VirtualBox Guest Additions in Ubuntu

### Wish Case
#### Installation of VirtualBox Guest Additions 
	•	Do How to apt update/upgrade your Ubuntu image instance
	•	Open a Terminal box:
	•	sudo apt install build-essential dkms linux-headers-$(uname -r)
	•	Next, from the Virtual Machine menu bar, go to Devices => click on Insert Guest Additions CD image as shown in the screenshot. This helps to mount the Guest Additions ISO file inside your virtual machine.
	•	Next, you will get a dialog window, prompting you to Run the installer to launch it.
	•	A terminal window will be opened from which the actual installation of VirtualBox Guest Additions will be performed. Once the installation is complete, press [Enter] to close the installer terminal window. Then power off your Ubuntu guest OS to change some settings from VirtualBox manager as explained in the next step.
	•	Now to enable Shared Clipboard and Drag’n’Drop functionality between Guest and Host Machine. Go to General => Advanced and enable the two options (Shared Clipboard and Drag’n’Drop) as you wish, from the drop down options. Then click OK to save the settings and boot your system, login and test if everything is working fine.
#### Create a Shared folder on Drive C:
	•	In the Windows File Explorer click on "This PC"
	•	Then double click on the C: drive
	•	Select your Downloads folder
	•	Put a sample text file into that folder
	•	In Windows File Explorer select that folder
	•	Right mouse click → Properties
	•	In the Sharing tab click the Share button
	•	Select your name and click the Share button near the bottom
	•	Your should see the message: Your folder is shared.
	•	Select Done
#### Create a Mount point in Ubuntu
	•	Open a Terminal box
	•	Create a mount point under /mnt call it Windows
	•	sudo mkdir /mnt/Windows
	•	sudo adduser $USER vboxsf
	•	You should relog (or reboot) for the adduser to take full effect
#### Link the Shared folder with the Mount point
	•	In the menu above the Ubuntu image
	•	Go Devices → Shared Folders → Shared Folder Settings ... 
	•	Select Machine Folders
	•	Click the top small blue icon to the right of the dialog
	•	Just like in the video select "Other ..." in Folder Path
	•	Then navigate to C: drive
	•	Then select the Shared directory that you set up
	•	Select it 
	•	Then click Auto-mount on
	•	Then click Make Permanent on
	•	In the Mount point enter /mnt/Windows
	•	Click the Ok button
#### Test the Shared folder with the Mount point
	•	Open a Terminal box
	•	Enter ls /mnt/Windows
	•	ls /mnt/Windows 
	•	You should see something like the following:
	•	dev@ubuntu-vb:~$ ls /mnt/Windows/ -la
	•	total 441
	•	drwxrwx--- 1 root vboxsf      0 Feb 11 17:17 .
	•	drwxr-xr-x 3 root root     4096 Feb 11 17:18 ..
	•	-rwxrwx--- 1 root vboxsf  28946 Aug 15  2017 repo
	•	-rwxrwx--- 1 root vboxsf     27 Apr  5  2017 repo.cmd
	•	dev@ubuntu-vb:~$ 
### Alternate Case
#### /mnt/c on Ubuntu (WSL/WSL2)
> On WSL/WSL2 they already share drive C as /mnt/c. Hence, anything downloaded in Windows to ~/Downloads can be found in /mnt/c/Downloads

### Alternate Case
#### Virtualbox shared folder permissions
> On a fresh Ubuntu install the $USER needs to be added to a special group.

>		sudo adduser $USER vboxsf 

> For more information see Virtualbox shared folder permissions

### Alternate Case
#### Downloads folder is already Shared
> In this case there is no need to change properties (see diagram to the right)

#### Alternate Case
On the non-updated v0.1 Ubuntu 16.04 the shared drive will show up in the file manager prefixed with the letters sf_ (as soon as your reboot).

### Additional Task: Reconnect Shared directory & Mount point
In an effort to make sure that everything is wired together properly. You should be able to disconnect the Shared directory from the Mount point, (using Devices → Shared Folders) then do a terminal ls -la /mnt/Windows, (which should show up as inaccessible). The add it again, (as per instructions above) and issue another ls -la /mnt/Windows, (and this time you should see the contents again). 
#### for example
	dev@ubuntu-vb:~$ ls /mnt/Windows/ -la
	total 441
	drwxrwx--- 1 root vboxsf      0 Feb 11 17:17 .
	drwxr-xr-x 3 root root     4096 Feb 11 17:18 ..
	-rwxrwx--- 1 root vboxsf 413383 Jan 24 12:09 Global_Town_Hall_2022.ics
	-rwxrwx--- 1 root vboxsf  28946 Aug 15  2017 repo
	-rwxrwx--- 1 root vboxsf     27 Apr  5  2017 repo.cmd
	dev@ubuntu-vb:~$ ls /mnt/Windows/ -la
	ls: cannot access '/mnt/Windows/': No such file or directory
	dev@ubuntu-vb:~$ ls /mnt/Windows/ -la
	total 441
	drwxrwx--- 1 root vboxsf      0 Feb 11 17:17 .
	drwxr-xr-x 3 root root     4096 Feb 11 18:38 ..
	-rwxrwx--- 1 root vboxsf 413383 Jan 24 12:09 Global_Town_Hall_2022.ics
	-rwxrwx--- 1 root vboxsf  28946 Aug 15  2017 repo
	-rwxrwx--- 1 root vboxsf     27 Apr  5  2017 repo.cmd
	dev@ubuntu-vb:~$ 

### Alternate Case
#### Fix shared folder stops working after software update/upgrade:
Tested for ubuntu 20 only (Courtesy of @Shafik, Peter (P.M.) )
Fix shared folder stops working after software update/upgrade:

	sudo apt install virtualbox-guest-utils

Check the group owner of the /mnt/Windows/directory

	ls -tla /media

If the group owner is root, then set it to vboxsf

	sudo chown -R :vboxsf /mnt/Windows/

Then add the dev user to vboxsf group

	sudo adduser dev vboxsf
	cat /etc/group | grep vbox (to confirm)
	sudo reboot

### Summary
> Adding local file access to an Ubuntu image through VirtualBox is not a trivial matter. As the procedure to actually link to a shared directory is actually a little bit more detailed in the video supplied on YouTube. 

### Next Steps
	•	How to add the ...
