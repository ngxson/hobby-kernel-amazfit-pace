#!/bin/bash

######################################################
#                                                    #
#              NUI Kernel build script               #
#                                                    #
#                  By ngxson (Nui)                   #
#                                                    #
######################################################

export ARCH=mips
export CROSS_COMPILE="/home/$USER/mipsel-linux-android-4.8/bin/mipsel-linux-android-"

if [ ! -d "/home/$USER/kernel-inwatch-out" ]; then
	mkdir "/home/$USER/kernel-inwatch-out"
fi

#make the zImage
make O="/home/$USER/kernel-inwatch-out" in901_defconfig
make O="/home/$USER/kernel-inwatch-out"