FRESH_NAME=fresh
FRESH_VERSION=1.1.0
DEB_VERSION=-4-ppa1
FRESH_SUFFIX=.tar.gz
FRESH_LIB=$(FRESH_NAME)-$(FRESH_VERSION)
FRESH_LIB_SRC=$(FRESH_LIB)-src
DEB_FRESH_LIB=$(FRESH_NAME)_$(FRESH_VERSION)
FRESH_FILE=$(FRESH_LIB).tar.gz
FRESH_SRC_ARCHIVE=$(FRESH_LIB_SRC)$(FRESH_SUFFIX)
DEF_FRESH_ORIG_ARCHIVE=$(DEB_FRESH_LIB)-4.orig$(FRESH_SUFFIX)

NATIVE_DIST=$(shell lsb_release -cs)

ifeq ($(shell whoami),a)
	NAME=Andrei Kopats
	EMAIL=hlamer@tut.by
	PPA=monkeystudio/ppa
else
	NAME=Filipe Azevedo
	EMAIL=pasnox@gmail.com
	PPA=pasnox/ppa
endif
DEBNAME=$(NAME)
DEBFULLNAME=$(NAME)
DEBEMAIL=$(EMAIL)

DISTS = precise oneiric natty maverick lucid

all: help

#
# Help
#
help:
	@echo 'Top level targets:'
	@echo '  dput-all          Build and put all to launchpad'
	@echo '  clean             Remove all created files'
	@echo
	@echo 'For debugging:'
	@echo '  not-signed-all    Build source packages for all distributions. Not signed'
	@echo '  signed-all        Build source packages for all distributions. Signed'
	@echo '  dput-{distrib}    Build and put package for particular distribution to launchpad'
	@echo '  native-build      Build source and binary package for local PC distribution. Not signed'
	@echo '  build-{distrib}    Prepare directory with sources and debian files'
	@echo '  dput              Put source package to PPA'
	@echo '  native-build      Build .deb package on the current system (no chroot). For testing'
	@echo
	@echo 'Supported distribs are:' $(DISTS)

#
# Prepare sources
#

$(FRESH_SRC_ARCHIVE):
	cd .. && git archive --prefix=$(DEB_FRESH_LIB)/ new-deb-package | gzip -9 > debian/$@

build-%: $(FRESH_SRC_ARCHIVE)
	mkdir -p $@
	cd $@ && tar -xf ../$(FRESH_SRC_ARCHIVE)
	ln -s $(PWD)/$(FRESH_SRC_ARCHIVE) $@/$(DEF_FRESH_ORIG_ARCHIVE)
	mkdir $@/$(DEB_FRESH_LIB)/debian
	cp -R debian-common/* $@/$(DEB_FRESH_LIB)/debian
	if [ -d debian-$(DIST) ]; then \
	    cp -R debian-$(DIST)/* $@/$(DEB_FRESH_LIB)/debian; \
	fi
	sed -i 's/PUT_SERIES_HERE_WITH_SED/$*/g' $@/$(DEB_FRESH_LIB)/debian/changelog

#
# Build source package
#

build-%/$(DEB_FRESH_LIB)$(DEB_VERSION).dsc: build-%
	cd build-$*/$(DEB_FRESH_LIB) && debuild -S -uc -us

not-signed-all: $(foreach dist,$(DISTS),build-$(dist) build-$(dist)/$(DEB_FRESH_LIB)$(DEB_VERSION).dsc)

signed-%: build-% build-%/$(DEB_FRESH_LIB)$(DEB_VERSION).dsc
	cd build-$* && debsign *.changes

signed-all: $(foreach dist,$(DISTS),build-$(dist) signed-$(dist));

dput-%: signed-%
	cd build-$* && dput ppa:$(PPA) $(DEB_FRESH_LIB)*source.changes

dput-all: $(foreach dist,$(DISTS),dput-$(dist));


#
# Build package.
# For testing. Release package is built on launchpad
#
native-build: build-$(NATIVE_DIST)
	cd build-$(NATIVE_DIST)/$(DEB_FRESH_LIB) && debuild -uc -us -b

#
# Clean
#
clean:
	-rm -rf build-*
	-rm $(FRESH_SRC_ARCHIVE)

