## How to setup your changelog.md
> The ability to keep track of your changes to source code comes in very handy in the later stages of your projects development, (as well as look much more professional). 

 1. **GIVEN** we need to keep track of source code changes
 2. **WHEN** we take the time to manage a changelog.md file
 3. **THEN** we can compare version differences much easier

### Prerequisites
- [How to clone your project (with this template)](https://github.com/perriera/extras_oci/blob/dev/docs/CLONE.md)
- [How to setup the tools necessary for your new project](https://github.com/perriera/extras_oci/blob/dev/docs/SETUP.md)
 
### Wish Case
Now that you have your project compiles properly we need to setup your changelog.md with v0.1.0 (to support **neatly** all future versions).


WORK IN PROGREE
(ignore this page)


 - [ ] Open a Terminal box and be in your project's directory the do:

		git add . 
		git commit -m "added v0.1.0"
		git push

 - [ ] The above has placed the entire project (where is, as is) as v0.1.0 into your Github.com repository (check your repository to see this).

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


