

## How to install a Linux Platform (Ubuntu 20.04.4)
> While intended to be cross-platform, at this point in time all instructions are based upon a Ubuntu 20.04.4 installation. This can be achived in multiple ways be that a native installation or a virtual machine made possible via a tool such as [Parallels Desktop for the iMac](https://www.parallels.com/ca/pd/general/?gclid=CjwKCAjwjZmTBhB4EiwAynRmD6SV4LdfM_DXaYiwTJhJJBddfc6JM_bDuAuWsUyyHUMj5zEQnC3wjBoCfgcQAvD_BwE) (try it for free).

 1. **GIVEN** we need to have a Linux installation to develop C++11/17 code
 2. **WHEN** we install the latest Ubuntu using Parallels Desktop
 3. **THEN** we can have all the benefits that come with a Linux environment

### Prerequisites
  - [Preferably a late model iMac](https://www.apple.com/ca/imac-24/?afid=p238%7CsOluZynmu-dc_mtid_1870765e38482_pcrid_571348307644_pgrid_125362851562_pntwk_g_pchan__pexid__&cid=aos-ca-kwGO-mac--slid---product-) (with a large screen)
  - [Parallels Desktop installed](https://www.parallels.com/ca/pd/general/?gclid=CjwKCAjwjZmTBhB4EiwAynRmD6SV4LdfM_DXaYiwTJhJJBddfc6JM_bDuAuWsUyyHUMj5zEQnC3wjBoCfgcQAvD_BwE)

 
### Wish Case
Assuming you have an iMac with Parallels Desktop installed

 - [ ] Goto Ubuntu.com and download [Ubuntu 20.04.4](https://releases.ubuntu.com/20.04/)
 - [ ] Press the + button to the top right corner of Parallels Desktop Control Center
 - [ ] Select *Install Windows or another OS from a DVD or image file*
 - [ ] Press *Continue*
 - [ ] Select *ubuntu-20.04.4-desktop-amd64.iso* in *~/Downloads* 
 - [ ] Fill out the details for an *Express Installation*
 - [ ] Rename the installation *Ubuntu 20.04.4 LTS* (for clarity)
 - [ ] Select *Customize settings before installation*
 - [ ] Press *Create*
 - [ ] Under *Options -> Sharing* share the Home folder (and unselect everything else)
 - [ ] Under *Options -> CPU & Memory* set Memory to 8192 
 - [ ] Under *Options -> Mouse & Keyboard* set Mouse & Keyboard to *Don't optimize for games*
 - [ ] Close the dialog and press the blue *Continue* button
 - [ ] Wait for the install to complete
 - [ ] Login in with the login credentials you provided
 - [ ] (Optional) connect to any online accounts you may wish to include 
 - [ ] (Optional) Setup a Ubuntu One / Live Patch account
 - [ ] Click *Next* until you see the desktop
 - [ ] Select *Update Now* on the Software Updater
 - [ ] To the left you should now see a small icon with a status bar slowly going right
 - [ ] That status bar shows you updates being installed (wait for that to complete)
 - [ ] Click on the icon you see In the bottom left corner 
 - [ ] Type "ter" in the search box, select Terminal
 - [ ] When the Terminal box opens execute the following
 - [ ] **sudo apt update**
 - [ ] **sudo apt upgrade**
 - [ ] **sudo apt autoremove** (click yes)
 - [ ] **sudo apt update**
 - [ ]  This time you should see the message "*All packages are up to date*."
 - [ ] **sudo reboot**
 - [ ] When you login again you now have a fully working Ubuntu instance
 - [ ] Shutdown the Ubuntu instance (top right corner click on the icon)
 - [ ] Select *Power Off -> Power Off*
 - [ ] Now in the Parallels Desktop Control Center select *Ubuntu 20.04.4 LTS*
 - [ ] Click the *Reclaim* button to reclaim unused hard disk space
 - [ ] Close the *Configuration* dialog
 - [ ] Right mouse click on *Ubuntu 20.04.4 LTS* and select *Clone...*
 - [ ] Replace the name *Copy of Ubuntu 20.04.4 LTS* with something meaningful (ex. buntu 20.04.4 <your project name>"
 - [ ] Start your cloned Ubuntu instance for your project (keeping the original one in reserve as a backup)

### Alternate Case
> **In the case of some other Linux installation / hardware configuration**<br/><br/>
> You do not need to use Ubuntu (or an iMac), (if you have some other hardware or software arrangement that is perfectly fine). As long as your system is supporting Linux kernel 5.13 or greater you should be able to use the tools necessary to compile a C++11/17 based project in a GUI environment capable of supporting Visual Studio Code (or similair tool).
> You do not need to use Ubuntu (or an iMac), (if you have some other hardware or software arrangement that is perfectly fine). As long as your system is supporting Linux kernel 5.13 or greater you should be able to use the tools necessary to compile a C++11/17 based project in a GUI environment capable of supporting Visual Studio Code (or similair tool).

### Summary 
Now you have a platform with which to install the C++11/17 tools necessary for *(perriera) /  extras_oci* cloned projects.

### Next Steps
 - [How to install the tools necessary for C++11/17 projects](https://github.com/perriera/extras_oci/blob/dev/docs/INSTALL.md)


