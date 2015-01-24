This README file contains information on the contents of the
hybris layer.

Please see the corresponding sections below for details.


Dependencies
============

This layer depends on:

  URI: git://git.openembedded.org/bitbake
  branch: master

  URI: git://git.openembedded.org/openembedded-core
  layers: meta
  branch: master

  URI: git://git.yoctoproject.org/xxxx
  layers: xxxx
  branch: master


Patches
=======

Please submit any patches against the hybris layer to the
xxxx mailing list (xxxx@zzzz.org) and cc: the maintainer:

Maintainer: XXX YYYYYY <xxx.yyyyyy@zzzzz.com>


Table of Contents
=================

  I. Adding the hybris layer to your build
 II. Dirty Hacks
III. Misc


I. Adding the hybris layer to your build
=================================================

--- replace with specific instructions for the hybris layer ---

In order to use this layer, you need to make the build system aware of
it.

Assuming the hybris layer exists at the top-level of your
yocto build tree, you can add it to the build system by adding the
location of the hybris layer to bblayers.conf, along with any
other layers needed. e.g.:

  BBLAYERS ?= " \
    /path/to/yocto/meta \
    /path/to/yocto/meta-yocto \
    /path/to/yocto/meta-yocto-bsp \
    /path/to/yocto/meta-aosp-hybris \
    "

II. Dirty Hacks
========

Since I building aosp-hybris via Vmware Fusion Share Folder, some
package use *hardlink* need to be patched, following are packages I
apply patch to switch hardlink to softlink.

recipes-devtools/pkgconfig/files/0001-convert-hardlink-to-softlink.patch


III. Misc
========

--- replace with specific information about the hybris layer ---
