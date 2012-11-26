XUP.OTHERS_PLATFORM_TARGET_DEFAULT = /ramdisk/fresh-subdirs/bin/examples
XUP.OTHERS_PLATFORM_TARGET_RELEASE = /ramdisk/fresh-subdirs/bin/examples
XUP.OTHERS_PLATFORM_TARGET_DEBUG = /ramdisk/fresh-subdirs/bin/examples_debug

TEMPLATE = subdirs
CONFIG *= ordered

SUBDIRS *=  \
    fresh.pro \
    examples/cpp/examples.pro