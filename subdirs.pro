XUPProjectSettings {
	OTHERS_PLATFORM_TARGET_DEFAULT	= examples/examples_debug
	OTHERS_PLATFORM_TARGET_RELEASE	= examples/examples
}

TEMPLATE	= subdirs
CONFIG	*= ordered

SUBDIRS	*= fresh.pro examples/examples.pro