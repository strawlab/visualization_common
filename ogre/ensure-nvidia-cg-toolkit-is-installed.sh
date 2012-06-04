#!/bin/sh

# Older versions of the debian package nvidia-cg-toolkit did not
# include the actual toolkit files, they just included an installer
# program.  That program downloaded files from nvidia's servers and
# installed them.  Occasionally those servers seem to fail to serve at
# least one of the files, and the way the debian install script is
# written, it returns success regardless of any errors in the
# download.
#
# Therefore this script runs before the ogre build and tries to detect
# missing Cg files and install from toolkit files stored here in the
# ros "ogre" package.
#
# It depends on the existence of the nvidia-cg-toolkit-installer
# binary in /usr/bin, which is OK because that seems to install
# correctly before the failure.
#
# Lucid seems to be the last Ubuntu release which had the unreliable
# package.  If Lucid support is not needed at some point we can probably
# get rid of this.

THIS_DIR=`dirname $0`
OS=`uname`
INSTALLER=/usr/bin/nvidia-cg-toolkit-installer

if [ $OS != "Linux" ]; then
    echo "ensure-nvidia-cg-toolkit-is-installed.sh does not support OS '$OS', only Linux.  Presuming it is OK."
    exit 0
fi

if [ -e /usr/include/Cg/cg.h -a \
     -e /usr/include/Cg/cgGL.h -a \
     -e /usr/lib/libCg.so -a \
     -e /usr/lib/libCgGL.so ]; then
    echo "Package nvidia-cg-toolkit seems to be correctly installed."
    exit 0
fi

if [ -e /usr/include/Cg/cg.h -a \
     -e /usr/include/Cg/cgGL.h -a \
     -e /usr/lib64/libCg.so -a \
     -e /usr/lib64/libCgGL.so ]; then
    echo "Package nvidia-cg-toolkit seems to be correctly installed."
    exit 0
fi

if [ -e /opt/nvidia-cg-toolkit/include/Cg/cg.h -a \
     -e /opt/nvidia-cg-toolkit/include/Cg/cgGL.h -a \
     -e /opt/nvidia-cg-toolkit/lib/libCg.so -a \
     -e /opt/nvidia-cg-toolkit/lib/libCgGL.so ]; then
    echo "Package nvidia-cg-toolkit seems to be correctly installed."
    exit 0
fi

if [ -x $INSTALLER ]; then
    echo "Nvidia-cg-toolkit did not get installed correctly.  To fix this,"
    echo "we need to run the installer program directly on the toolkit files"
    echo "included in the 'ogre' ros package."
    echo "The install command requires 'sudo', which may now ask for your password."
    if sudo $INSTALLER -i -p $THIS_DIR/nvidia-cg-toolkits/ ; then
        echo "nvidia cg toolkit installed successfully."
        exit 0
    else
        echo "contents of /usr/include/Cg:"
        ls -al /usr/include/Cg
        echo 'contents of /usr/lib/*Cg*:'
        ls -l /usr/lib/*Cg*
        echo "nvidia cg toolkit install failed."
        exit 1
    fi
else
    echo "contents of /usr/include/Cg:"
    ls -al /usr/include/Cg
    echo 'contents of /usr/lib/*Cg*:'
    ls -l /usr/lib/*Cg*
    echo "nvidia installer program '$INSTALLER' not found and package was not correctly installed.  failing."
    exit 1
fi
