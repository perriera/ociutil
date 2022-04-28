
## How to setup the tools necessary for your new project
> In this step we need to install all  the tools used by this project (that you may or may not already have installed.

 1. **GIVEN** we need to have C++17, cmake 3.2.1, pip installed
 2. **WHEN** we follow the instructions listed here
 3. **THEN** we can press Ctrl-B inside Visual Studio Code

### Prerequisites
  - [How to clone your project (with this template)](https://github.com/perriera/extras_oci/blob/dev/docs/CLONE.md)

 
### Wish Case
Now that you have your project cloned we need to make sure you have the tools necessary to compile properly:

 - [ ] First put **sudo** into ready accept mode
	
		sudo ls
	
 - [ ] Now copy and paste the following for a Linux environment

		sudo apt update
		sudo apt install -y build-essential libtool autotools-dev automake pkg-config git clangd cppcheck clang-tidy python3-pip checkinstall

 - [ ] Install CMake 3.21

		pip install clang-format
		pip install cmake-format
		pip install cmake

 - [ ] Add these enironment variables (aka.  `vi ~/.bashrc `)

		
		export PATH=$HOME/.local/bin:${PATH}
		export CPM_SOURCE_CACHE=$HOME/.cache/CPM
		export LD_LIBRARY_PATH=/usr/local/lib:${LD_LIBRARY_PATH}

 - [ ] Then source it ... 

		source ~/.bashrc

 - [ ] Now install Visual Studio Code

		sudo snap install --classic code # or code-insiders

 - [ ] Now start Visual Studio Code

		cd <into your project directory>
		code .

### Summary 
Now you have all the tools necessary to compile your new C++17 project complete with cmake 3.21 support. Press Ctrl-B (inside Visual Studio Code) to see your first successful compile.

### Next Steps
 - [How to setup your changelog.md](https://github.com/perriera/extras_oci/blob/dev/docs/CHANGELOG.md)


