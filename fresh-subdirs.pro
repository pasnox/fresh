XUP.OTHERS_PLATFORM_TARGET_DEFAULT = /ramdisk/fresh-subdirs/examples/cpp/examples
XUP.OTHERS_PLATFORM_TARGET_RELEASE = /ramdisk/fresh-subdirs/examples/cpp/examples
XUP.OTHERS_PLATFORM_TARGET_DEBUG = /ramdisk/fresh-subdirs/examples/cpp/examples_debug

TEMPLATE = subdirs
CONFIG *= ordered

SUBDIRS *=  \
    fresh.pro \
    examples/cpp/examples.pro