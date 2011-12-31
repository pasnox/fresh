NAME=Andrei Kopats
EMAIL=hlamer@tut.by
DEBNAME=$(NAME)
DEBFULLNAME=$(NAME)
DEBEMAIL=$(EMAIL)

FRESH_NAME=fresh
FRESH_VERSION=1.1.0
FRESH_SUFFIX=.tar.gz
FRESH_LIB=$(FRESH_NAME)-$(FRESH_VERSION)
FRESH_LIB_SRC=$(FRESH_LIB)-src
DEB_FRESH_LIB=$(FRESH_NAME)_$(FRESH_VERSION)
FRESH_FILE=$(FRESH_LIB)$(FRESH_SUFFIX)
FRESH_SRC_ARCHIVE=$(FRESH_LIB_SRC)$(FRESH_SUFFIX)
DEF_FRESH_ORIG_ARCHIVE=$(DEB_FRESH_LIB)-4.orig$(FRESH_SUFFIX)
DEB_PATH=$(DEB_FRESH_LIB)/debian
FRESH_URL=https://github.com/downloads/pasnox/fresh/$(FRESH_SRC_ARCHIVE)

DIST=$(shell lsb_release -cs)

# "precise" "oneiric" "natty" "maverick" "lucid"
DISTS = lucid oneiric

all:
	@echo $(ALL_DEBS)

#
# Help
#
help:
	@echo 'Top level targets:'
	@echo 'signed-src   Create signed source package (.dsc, .chagnes)'
	@echo 'dput         Put source package to PPA'
	@echo 'native-build Build .deb package on the current system (no chroot). For testing'
	@echo 'clean        Clean all generated files'

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

#
# Build source package
#

build-%/$(DEB_FRESH_LIB)-4-ppa1.dsc: build-%
	cd build-$*/$(DEB_FRESH_LIB) && debuild -S -uc -us

all-src: $(foreach dist,$(DISTS),build-$(dist) build-$(dist)/$(DEB_FRESH_LIB)-4-ppa1.dsc)

signed-src: src
	DEBFULLNAME='$(DEBFULLNAME)' DEBEMAIL='$(DEBEMAIL)' debsign *.changes

dput: signed-src
	dput ppa:pasnox/ppa $(DEB_FRESH_LIB)*source.changes


#native-build: result $(DEB_FRESH_LIB) $(DEB_FRESH_LIB)/debian
#	cd $(DEB_FRESH_LIB) && debuild -uc -us -b

#
# Clean
#
clean:
	-rm -rf build-*
	-rm $(FRESH_SRC_ARCHIVE)

