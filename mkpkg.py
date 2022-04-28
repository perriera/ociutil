"""
A simple script to create a debian package
using checkinstall.

Author: Matt Williams
"""

import argparse
import os
import subprocess as sp
import sys
from pathlib import Path

# Paranoid platform check
if sys.platform != "linux":
    sys.stderr.write(f"{sys.platform}' is currently not supported\n")
    sys.exit(1)

# check if user is root
if os.getenv("USER") == "root":
    sys.stderr.write(
        "\033[1;31mError\033[0m We humbly request that you do not run this script as root \n"
    )
    sys.exit(1)


class Colors:
    """
    Struct to represent colors
    """

    BBlack = "\033[1;30m"
    BRed = "\033[1;31m"
    BGreen = "\033[1;32m"
    BYellow = "\033[1;33m"
    BBlue = "\033[1;34m"
    BPurple = "\033[1;35m"
    BCyan = "\033[1;36m"
    BWhite = "\033[1;37m"
    Reset = "\033[0m"


class Packager:
    """
    A simple class that uses checkinstall to build
    a debian package
    """

    PKGNAME = "extras_oci"

    def __init__(self, args: argparse.Namespace):
        self.pkg_root_dir = Path(__file__).resolve(strict=True).parent
        self.build_dir = Path(f"{self.pkg_root_dir}") / "build"
        self.shell: str = os.getenv("SHELL")
        self.pkg_version: str = args.version
        self.pkg_license = "MIT"
        self.pkg_maintainer = "'mattcoding4days perriera'"

    @staticmethod
    def error(message: str) -> None:
        """
        outputs colorized error message and exits
        :return: None
        """
        sys.stderr.write(
            f"{Colors.BRed}ERROR{Colors.Reset}  {Colors.BWhite}{message}{Colors.Reset}\n"
        )
        sys.exit(1)

    @staticmethod
    def warn(message: str) -> None:
        """
        outputs colorized warning message
        :return: None
        """
        sys.stdout.write(
            f"{Colors.BYellow}WARNING{Colors.Reset}  {Colors.BWhite}{message}{Colors.Reset}\n"
        )

    @staticmethod
    def info(message: str) -> None:
        """
        outputs colorized info message
        :return: None
        """
        sys.stdout.write(
            f"{Colors.BGreen}INFO{Colors.Reset}  {Colors.BBlue}{message}{Colors.Reset}\n"
        )

    def __exec_command(self, command: str) -> None:
        """
        wrapper around subproccess, if a command fails to execute properly,
        error_msg will print, and promptly exit the entire script.
        :return: None
        """
        self.info(f"Running: {command}")
        try:
            sp.run(command, check=True, shell=True, executable=self.shell)
            self.info("Done.. \n")
        except sp.CalledProcessError as error:
            self.error(f"{error}")

    def __is_installed(self, command: str) -> bool:
        """
        wrapper around subproccess, to test if a program is already installed,
        this will not work for libraries etc, but only for executables
        :return: True, False
        """
        cmd = f"command -v {command}"
        ret: sp.CompletedProcess = sp.run(
            cmd, check=False, capture_output=True, shell=True
        )
        if ret.returncode != 0:
            # the program is not installed
            return False

        self.info(f"Found: {command}")
        return True

    def build_pkg(self) -> None:
        """
        build the package with parameters that were fed to us
        """
        if self.pkg_root_dir.stem != Packager.PKGNAME:
            self.error("Please run the script from project root")

        if not self.build_dir.exists():
            self.error("Please compile the library in a build directory first")

        self.info("Found: build dir")
        if not self.__is_installed("checkinstall"):
            self.error("checkinstall is not installed on the system")

        self.__exec_command(
            f"cd {str(self.build_dir)} && sudo checkinstall -y --pkgname={self.PKGNAME}"
            f" --pkgversion={self.pkg_version} --pkglicense={self.pkg_license}"
            f" --maintainer={self.pkg_maintainer} --install=yes"
        )


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="mkpkg",
        description=f"Create a debian package for the {Packager.PKGNAME} library",
    )
    parser.add_argument(
        "version",
        type=str,
        help=f"The current version of the {Packager.PKGNAME} library",
    )

    arguments = parser.parse_args()
    package = Packager(arguments)
    package.build_pkg()
