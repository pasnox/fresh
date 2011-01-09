XUPProjectSettings {
	OTHERS_PLATFORM_TARGET_DEFAULT	= examples/cpp/examples
	OTHERS_PLATFORM_TARGET_RELEASE	= examples/cpp/examples
	OTHERS_PLATFORM_TARGET_DEBUG	= examples/cpp/examples_debug
}

TEMPLATE	= subdirs
CONFIG	*= ordered

SUBDIRS	*=  \
	fresh.pro \
	examples/cpp/examples.pro
