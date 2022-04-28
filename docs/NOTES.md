# perriera/extras_oci

Welcome to the perriera/extras_oci project.

[![CMake](https://github.com/mattcoding4days/extras/actions/workflows/cmake.yml/badge.svg?branch=dev)](https://github.com/mattcoding4days/extras/actions/workflows/cmake.yml)

<div align="center">
  <img width="442" height="320" src="assets/extras.png">
  <br>
</div>

## Who would want to use this?

> This project aims to get you up and running with C++17 based project with a 'git clone' operation. As well, this particular C++ project template comes with builtin [CPM](https://github.com/cpm-cmake/CPM.cmake) support. This means that you can incorporate other [github](https://github.com) projects, (or anything that uses the git tool) into your project with the simple inclusion of a CPM directive in in your CMakeLists.txt file.

## Prerequisites

To use this template you will need a Linux environment, (Ubuntu is idea).

1.  [Install pip](https://linuxize.com/post/how-to-install-pip-on-ubuntu-18.04/), (either pip 2 or pip 3)
2.  [Install cmake](https://cmake-python-distributions.readthedocs.io/en/latest/installation.html) using pip, (cmake 3.2.1 or better required)
3.  Create project on [Github.com](https://github.com) using **perriera/extras_oci** out of the box, (or as a Github template).

Once you have established these THREE STEPS you may do the following

    git clone https://github.com/<your project>
    cd <your project>
    mkdir build
    cd build
    cmake ..
    make
    cd ..
    build/run-unittests-extras_oci

If you have done everything correctly, you will see a Catch2 diagnostic with all green, (similar to the following):

    perry@ubuntu:~/Projects/extras_oci$ build/run-unittests-extras_oci
    ===============================================================================
    All tests passed (1 assertion in 2 test cases)

**Recommended**: The project now has been successfully created in git already. So add your first github tag:

    git tag -a v0.1.0 -m "Created extras_oci"
    git push origin v0.1.0

However, you not done yet:

Inside your favorite editor, (or search & replace command utility):

1.  Global search and replace **extras_oci** to <project_name> (case sensitive)
2.  Global search and replace **EXTRAS_OCI** to <PROJECT_NAME> (case sensitive)
3.  Change directory name '**include/extras_oci**' to '**include/<project_name>**'
4.  Change the links found in changelog.md such that they begin at 0.1.0 for your project.

For example, instead of:

    [Unreleased]: https://github.com/perriera/extras_oci/compare/v0.7.0...HEAD
    [0.7.0]: https://github.com/perriera/extras_oci/compare/v0.6.6...v0.7.0
    [0.6.6]: https://github.com/perriera/extras_oci/compare/v0.5.6...v0.6.0
    [0.5.6]: https://github.com/perriera/extras_oci/compare/v0.5.5...v0.5.6
    [0.5.5]: https://github.com/perriera/extras_oci/compare/v0.5.4...v0.5.5
    [0.5.4]: https://github.com/perriera/extras_oci/compare/v0.5.3...v0.5.4
    [0.5.3]: https://github.com/perriera/extras_oci/compare/v0.5.2...v0.5.3
    [0.5.2]: https://github.com/perriera/extras_oci/compare/v0.5.1...v0.5.2
    [0.5.1]: https://github.com/perriera/extras_oci/compare/v0.5.0...v0.5.1
    [0.5.0]: https://github.com/perriera/extras_oci/compare/v0.4.2...v0.5.0
    [0.4.2]: https://github.com/perriera/extras_oci/compare/v0.4.1...v0.4.2
    [0.4.1]: https://github.com/perriera/extras_oci/compare/v0.4.0...v0.4.1
    [0.4.0]: https://github.com/perriera/extras_oci/compare/v0.3.1...v0.4.0
    [0.3.1]: https://github.com/perriera/extras_oci/compare/v0.3.0...v0.3.1
    [0.3.0]: https://github.com/perriera/extras_oci/compare/v0.2.0...v0.3.0
    [0.2.0]: https://github.com/perriera/extras_oci/compare/v0.1.0...v0.2.0
    [0.1.0]: https://github.com/perriera/extras_oci/releases/tag/v0.1.0

Make it more like:

    [Unreleased]: https://github.com/perriera/extras_oci/compare/v0.2.0...HEAD
    [0.2.0]: https://github.com/perriera/extras_oci/compare/v0.1.0...v0.2.0
    [0.1.0]: https://github.com/perriera/extras_oci/releases/tag/v0.1.0

Also, while you are at, remove all entries to tags in changelog.md from **[0.2.0]** onward. As in there should only be two tag entries in your new project.

Change this:

    #### 0.8.0 (2021-09-03)
    ### Moved
    - Vendor files

    ...

    #### 0.3.1 (2021-09-03)
    ### Removed
    - Renamed identifier

    #### 0.3.0 (2021-09-03)
    ### Removed
    - extras_oci source code

    #### 0.2.0 (2021-09-03)
    ### Added
    - SHARED library support
    - (perriera) extras library support, (3.11.0)
    - CPM.cmake support

To this:

    #### 0.2.0 (2021-09-03)
    ### Added
    - SHARED library support
    - (perriera) extras library support, (3.11.0)
    - CPM.cmake support

Then change the version information in **include/version.h** to 0.2.0

    #define EXTRAS_OCI_VER_MAJOR 0
    #define EXTRAS_OCI_VER_MINOR 2
    #define EXTRAS_OCI_VER_PATCH 0

**Recommended**: You must make sure that the github tags you setup are always the same as the version number for the project. This has to be done by you manually, (at this point in time).

Then do the following:

    rm -rf build
    cd build
    make
    cd ..

This will re-create the project now with all references to extras_oci changed to extras_oci. To test this do the following:

    build/run-unittests-<press tab>

**Recommended**: The project now has been successfully created in git already. So add your first github tag:

    git tag -a v0.2.0 -m "Renamed C++ template to: extras_oci"
    git push origin v0.2.0

## Changelog.md & include/version.hpp

Remember, when you go to make changes to your changelog.md you should maintain the version number of your project using 'include/version.hpp'. For example, in your version.hpp will look something similiar to the following but all referenced to EXTRAS_OCI will be changed to extras_oci

**Recommended**: Inside your Github.com for your project, whenever you add a new tag, go the distance and make it a _release_ as well. Just go to your project on Github, select **tag**. Once the tags page is displayed left mouse click on the three dots you see for the newly added tag and select "Create Release"

## CPM Optimization

> Note: Be sure to have the desired **VERSION** of perrier/extras specified in the **VERSION** keyword of the CPMAddPackage specification, (in your CMakeLists.txt). Also, to optimize CPM support on your projects be sure to set the CPM_SOURCE_CACHE environment variable for shared CPM libraries. Typically you would add this to your `.bashrc` script. With this environment variable set, the CPM package manager will not need to reload after successive build/ directory resets, (aka. `rm -rf build`).

    export CPM_SOURCE_CACHE=$HOME/.cache/CPM

## CPM: Debian package, (Shared libraries) & gdb, (debugging) issue

**NOTE:** If you ever run into a situation where your debugging environment is not displaying the same variable names or code changes that you are currently working on be sure to check whether or not the source code you are working is not currently installed as a Debian package, (as a shared library).

    cd build
    sudo dpkg -r <package-name>

**Further**: Whenever your development environment appears to be acting strange, (as in the compiled code does not appear to be consistent with the source code) remember these three factors:

1.  That you can always backtrack your changes using your source repository.
2.  As well as reset your development environment simply by deleting your **build/** directory.
3.  Also, be sure to keep in mind whether the code you are working on is currently installed as a shared library.

## Visual Studio Code (build/ directory) Compilation Fail Safe

The industry practice of compiling to a build/ directory is supported here. When you detect some funny business going on with your development environment, (for example if the cmake utility keeps rebuilding everything), merely delete the build/ directory, (aka. `rm -rf build`) and recreate it. If you are using the Visual Studio Code environment you can press **SHIFT-CTRL-B**, (see `.vscode/tasks.conf`) which will force a rebuild of the directory, (including re-creating `build/` for you).

> Please note: At present whenever you re-open your project with Visual Studio Code the cmake utility may or may not be in sync with the projects, (you'll notice this when all the source files keep getting rebuilt). To correct this open a Terminal window **inside** the Visual Studio Code environment and delete the build/ directory, (aka. `rm -rf build`)then rebuild the project with **SHIFT-CTRL-B**.

## FakeIt2 C++ Mocking Framework

This project is already setup for use with [FakeIt](https://github.com/eranpeer/FakeIt). Anyone familiar with writing interface mocks, (as in [Mock Object](https://en.wikipedia.org/wiki/Mock_object) should find the built-in support for mocking interfaces using the FakeIt framework to be indispensible here).

## Catch2 C++ Test Case Framework

This project is already setup for use with [Catch2](https://github.com/catchorg/Catch2). Anyone familiar with writing test cases, (as in [TDD - Test Driven Development](https://en.wikipedia.org/wiki/Test-driven_development) should find the built-in support for Catch2 to be indispensible here).

## JIRA/Confluence/Bitbucket support

This project is already setup for use with [Jira (software)](<https://en.wikipedia.org/wiki/Jira_(software)>). Anyone familiar with writing JIRA entries as part of morning [SCRUM](<https://en.wikipedia.org/wiki/Scrum_(software_development)>) meetings, (or bi-weekly [SCRUM SPRINT](https://en.wikipedia.org/wiki/Scrum_Sprint) meetings) will find **perriera/extras_oci** to be well suited to the needs of JIRA/SCRUM/SPRINT.

## Conclusion

Your brand new C++ project is now setup and ready for use with **[perriera/extras](https://github.com/perriera/extras)**, (which comes bundled with [spdlog](https://github.com/gabime/spdlog), [cpr](https://github.com/libcpr/cpr), and [nlohmann json](https://github.com/nlohmann/json) support, plus others).
