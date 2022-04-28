

## How to setup ubuntu-20.04.4-desktop-amd64 (on VirtualBox)

>Here is how to setup VirtualBox for use with Ubuntu

### use case (purpose)
	• GIVEN we need to setup ubuntu-20.04.4 LTS on the laptop/workstation
	• WHEN we access the Internet via ZScaler support
	• THEN we expediate the Ubuntu installation process 
	• AND have the most bug free, well-supported Ubuntu ready for use

### Prerequisites
	•	Access to the Internet
	•	How to install VirtualBox (v6.1.32) onto Windows 10/11
	•	Enough hard disk space (perhaps 10-20 GB)
	•	Running Windows 10/11

#### External Resources
	•	Download Ubuntu Desktop (for 18.04) 
	•	VDI Vs VHD Vs VMDK: Learn the Different Virtual Disk Formats

### Wish Case ([Download Ubuntu 20.04.4 LTS](https://releases.ubuntu.com/20.04/))
	•	On Windows Download Ubuntu 20.04.4 LTS to Downloads folder
	•	Open VirtualBox and select Machine → New 
	•	In the Name field enter Ubuntu 20.04 LTS click Next
	•	Set Memory Size to 8192 click Next
	•	Select Create a virtual hard disk now click Create
	•	Select VMDK (Virtual Machine Disk) click Next
	•	Select Dynamically Allocated click Next
	•	In File location and size specify 256 GB click Next
	•	You will now see a new virtual machine in VirtualBox
	•	Click Settings on toolbar 
	•	Click Advanced → General
	•	Set Shared Clipboard to Bidirectional
	•	Set Drag n Drop to Bidirectional
	•	Click System → Processor
	•	Set Processor(s) to 2
	•	Click System → Display
	•	Set Video Memory to 64mb
	•	Activate Enable 3D Acceleration
	•	Click System → Network
	•	Under Adaptor 1 click Advanced
	•	Set Adaptor Type to Paravirtualized Network (virtio)
	•	Under Adaptor 2 click Enable Network Adaptor
	•	Set Attached to Host-only Adaptor
	•	Set Adaptor Type to Intel Pro/1000 T Server 
	•	Click the Ok button at the bottom of the dialog
	•	On the toolbar click the green Start button
#### Install Ubuntu
	•	On the Select startup-disk dialog click the small yellow-green icon (to the bottom right)
	•	Click the Add button (with the plus sign in it) 
	•	In the Downloads folder select ubuntu-18.04.6-desktop-amd64
	•	Click Choose click Start
	•	Wait for the Ubuntu installation process to start
	•	Click Install Ubuntu
	•	Select keyboard preference lick Continue
	•	Click Minimal installation
	•	Unclick Downloads while installing Ubuntu
	•	Click Install third party software for graphics and Wifi ...
	•	Click Continue
	•	Click Use LVM with the new Ubuntu Installation 
	•	Make sure Erase disk and install Ubuntu is also selected
	•	Click Install Now
	•	When asked Write the changes to disks? click  Continue
	•	Select New York (EST) as the time zone click  Continue
	•	Enter your name and password
	•	Make sure Require my password to login is also selected
	•	click  Continue
	•	Go get a coffee...
	•	When the installation completes click on Restart Now 
	•	Press Enter again to "remove the installation medium"
	•	You will be presented with two dialogs
	•	Select the Software Updater dialog and click Install Now
	•	Wait for the updater to complete (watch small icon to the left)
	•	Restart the Ubuntu instance
	•	Log back in and open a Terminal window
	•	Execute sudo apt autoremove
#### Make an LWV
	•	At this point it is a good idea to make an LWV
	•	Log off / shutdown the new Ubuntu image and make a LWV
	•	Do How to create a Ubuntu image LWV, (last working version)
	•	Rename the LWV to Ubuntu 18.04 LTS.zip (as it has not been upgraded yet) [i]
#### Upgrade Ubuntu to 20.04.4 LTS
	•	Log back into your new Ubuntu installation
	•	Open a Terminal box: 
	•	Execute sudo do-release-upgrade
	•	When asked to Continue [yN] select y and hit Enter twice
	•	Go refill your coffee... (give it an hour, maybe two)
	•	When asked Remove obsolete packages? click y and hit Enter once
	•	When told Restart required click y and hit Enter once
	•	When Ubuntu comes back up it'll be version 20.04.4 LTS
	•	Log back in
	•	Open a Terminal box:
	•	Do How to apt update/upgrade your Ubuntu image instance
	•	Make sure you did a sudo apt autoremove 
	•	When all that comes back all clear do a sudo reboot
#### Set the screen size
	•	Open the Ubuntu Settings dialog
	•	Go down to Displays 
	•	(Move the dialog so that you can see the right side of it)
	•	Try different Resolution combinations till you find one that is right for you
	•	Turn on or off Fractional Scaling based on your personal preference
	•	Click the Apply button
	•	Click Keep Changes if you see the dialog
	•	Above the Ubuntu image there is a menu
	•	Click View → Full-screen Mode 
	•	Ubuntu will have a small toolbar at the bottom of the screen
	•	Click on the blue icon near to the right to change back from Full-screen mode [ii]
	•	See How to set screen size for Ubuntu for more info
#### Set the Terminal background
	•	Select or Open a Terminal window 
	•	Do How to set transparency on the Terminal window
### Add VirtualBox Additions & local file access
	•	Do How to add local file access to Ubuntu (on VirtualBox)
### Create a LWV for Ubuntu 20.04.4 LTS
	•	You now have a fully working Ubuntu 20.04.4 LTS
	•	Shutdown the Ubuntu instance 
	•	Make a LWV for it now (should only require 4GB) and have it on hand for if ever you need to do a complete reinstall of all the tools.
	•	Restart Ubuntu and do whatever Next Steps are necessary
### Alternate Case
#### GRUB update boot record?
> Mid way through the upgrade you may be asked to upgrade the GRUB, (this happens during the 18.04 upgrade). If it happens again you want to check the first check box and leave everything else unchecked.
### Alternate Case
#### Keyboard or mouse not working properly
> Across the top of the Ubuntu image you will see a menu. Select Machine→ ACPI Shutdown and restart it again from VirtualBox.
### Alternate Case
#### Why not install ubuntu-20.04.4 image directly?
> That was attempted (at least three times) and the results were less than desirable. What was found however was that by install 18.04 and upgrading from there the outcome was much more stable and responsive.
### Alternate Case
#### Why not VDI or VHD format?
> The VMDK format has a much better reputation when it comes to performance and we are seeing a huge difference in the responsiveness of the Ubuntu image (under VirtualBox) since making use of it.
" Unofficial tests also show that VMDK is significantly faster than VDI or VHD. "
-- by Alex Hunter, (Parallels.com)
See VDI Vs VHD Vs VMDK: Learn the Different Virtual Disk Formats
### Summary
You may notice bit of a performance improvement with this setup. This is largely due to the source being the latest from Ubuntu.com, the use of the VMDK file format as well as removing unnecessary packages. Please come back to this page and make a comment below as to how your experienc e with the new Ubuntu 20.04.4 has been for you.
### Next Steps
	•	How to resize the Ubuntu image
### Footnotes
[i] You will want to keep Ubuntu 18.04 LTS.zip around as it will occupy only 4 GB of space. If ever the case arises where Ubuntu 20.04 LTS is having issues you have a good place to start again.
[ii] If you don't see the Ubuntu menu in the bottom left corner, then the resolution you have selected won't any good to you (as you won't be able to easily select the Ubuntu menu system)
