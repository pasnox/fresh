XUPProjectSettings {
	OTHERS_PLATFORM_TARGET_DEFAULT	= cpp-examples/cpp-examples_debug
	OTHERS_PLATFORM_TARGET_RELEASE	= cpp-examples/cpp-examples
	OTHERS_PLATFORM_TARGET_DEBUG	= cpp-examples/cpp-examples_debug
}

TEMPLATE	= subdirs
CONFIG	*= ordered

SUBDIRS	*= \
	fresh.pro \
	cpp-examples/examples.pro
