#!/bin/sh

BOOTFLAG="/dev/_android_boot_done"

# remove previous /dev/_android_boot_done
rm $BOOTFLAG

if [ ! -d /dev/cpuctl ] && [ -d /sys/fs/cgroup/cpu ] ; then
        mkdir /dev/cpuctl
        mount --bind /sys/fs/cgroup/cpu /dev/cpuctl
fi

echo "Waiting for droid-hal-init come up ..."

while true; do
        echo "Checking for boot done flag ..."
        [ -f $BOOTFLAG ] && break
        sleep 1
done
