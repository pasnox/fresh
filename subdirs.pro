XUPProjectSettings {
	OTHERS_PLATFORM_TARGET_DEFAULT	= examples/examples_debug
	OTHERS_PLATFORM_TARGET_RELEASE	= examples/examples
	OTHERS_PLATFORM_TARGET_DEBUG	= examples/examples_debug
}

TEMPLATE	= subdirs
CONFIG	*= ordered

SUBDIRS	*= \
	fresh.pro \
	examples/examples.pro