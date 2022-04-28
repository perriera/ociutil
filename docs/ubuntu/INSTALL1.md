## Recommended Development Environment
Recommended GCC environment:

	sudo apt update
	sudo apt install -y build-essential libtool autotools-dev automake 
	sudo apt install -y pkg-config git clangd cppcheck clang-tidy 
    sudo apt install -y python3-pip

[More information on pip here](https://linuxize.com/post/how-to-install-pip-on-ubuntu-18.04/)

	pip install clang-format
	pip install cmake-format
	pip install cmake

WARNING: The scripts cmake, cpack and ctest are installed in '/home/perry/.local/bin' which is not on PATH.
Consider adding this directory to PATH or, if you prefer to suppress this warning, use --no-warn-script-location. 
 
	vi ~/.bashrc

Add this to the end of `~/.bashrc `

	export PATH=$HOME/.local/bin:${PATH}
    export CPM_SOURCE_CACHE=$HOME/.cache/CPM
    export LD_LIBRARY_PATH=/usr/local/lib:${LD_LIBRARY_PATH}

Then source it ... 

    source ~/.bashrc

## Visual Studio Code extensions
When prompted be sure to add the recommended extensions for C++, CMake and other source code support. 

	 sudo snap install --classic code # or code-insiders
	 code .

## .vscode directory
Inside the `extras/` project directory you'll see 4 files:

 - c_cpp_properties.json
 - launch.json
 - settings.json
 - tasks.json

These govern things like which C++ standard to use, (in this case **C++17**), which test case to launch, file associations and key associations, (such that if you type **shift-control-B** a compile/make will begin). You may have to restart the editor once the path has been edited. 

# Extras Package Compilation
If you have not already done so install a local copy of extras onto your system, :
 
     git clone https://github.com/perriera/extras.git 
     cd extras
     mkdir build
     cd build
     cmake ..
     make
     ./run-unittests-extras

If the tests come back similar to the following you have a successful install:

	perry@ubuntu:~/Projects/extras$ build/run-unittests-extras 

	===============================================================================
	All tests passed (76 assertions in 30 test cases)


## Local installation
One is to install it locally onto your system. Just be sure to have your LD_LIBRARY_PATH setup to be able to find the installed extras package.

     export LD_LIBRARY_PATH=/usr/local/lib
     

### sudo checkinstall
While the `sudo make install` will work for most applications, should you wish to uninstall you will need to install **extras** using a slightly different approach. To be able to uninstall you will need to install the Ubuntu **checkinstall** package: [here](https://help.ubuntu.com/community/CheckInstall). 

`sudo apt-get update && sudo apt-get install checkinstall`

With the checkinstall package installed your installation process now becomes:
    
     git clone https://github.com/perriera/extras.git 
     cd extras
     mkdir build
     cd build
     cmake ..
     make
     ./run-unittests-extras
     sudo checkinstall

**Note**: During the checkinstall process be sure to change the name of the package from the default: **build** to **extras**. It'll be item #2 on the checkinstall parameter listings, (you can safely use the defaults for the rest of the items on the checkinstall).

Then when you need to uninstall this package:

    sudo dpkg -r extras

Should you run into a strange situation to where you issued the above command but the **extras** package appears to still be installed, run the following:

    sudo dpkg -r build

As you may have installed it earlier without changing the name of the package from **build** to **extras**.

Just be sure to have the desired **VERSION** of perrier/extras specified in the **VERSION** keyword above. To optimize CPM support on your projects be sure to set the environment variable for shared CPM libraries:

	export CPM_SOURCE_CACHE=$HOME/.cache/CPM

For more about CPM [see here](https://github.com/cpm-cmake/CPM.cmake)

## CMakeLists.txt
You just include the **extras** library to any targets in your CMakeLists.txt target_include_libraries specs

For example:

    target_link_libraries(run-unittests-extras
       chessmind::library
       extras
    )
    
The 2.3.8 version of the extras libraries installs the debug version by default. If you wish to use a production version of the library you can either comment out this line in the main CMakeLists.txt file and recompile:

    # add additional project options
    option(BUILD_TESTS "Build tests" ON)
    option(DEBUG "Debug Build" ON)

Or, as of 3.2.1 you must set DEBUG to false, (& BUILD_TESTS=false) on the command line of the cmake command and it will cause a production version of the library to be generated instead, (just be sure to delete the build directory before hand):

    cd ..
    rm -rf build
    cd build
    cmake -DDEBUG=false -DBUILD_TESTS=false ..
    make
    ./run-unittests-extras
    sudo make install

### FYI: ModernCppStarter
> perriera/cpp_starter is based on ModernCppStarter 
> 

*"Setting up a new C++ project usually requires a significant amount of preparation and boilerplate code, even more so for modern C++ projects with tests, executables and continuous integration. This template is the result of learnings from many previous projects and should help reduce the work required to setup up a modern C++ project." -- The Lartians</br>*

https://github.com/TheLartians/ModernCppStarter/blob/master/LICENSE</br>
Copyright (c) 2019 The Lartians

[cpp_starter](https://github.com/perriera/cpp_starter)<br/>
This project will get you up and running with the latest CPM support as well as this library, (a real time saver).



