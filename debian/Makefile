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

$(foreach dist,$(DISTS),build-$(dist)):
	mkdir -p $@

build-%/$(DEB_FRESH_LIB): $(FRESH_SRC_ARCHIVE) build-%
	cd build-$* && tar -xf ../$(FRESH_SRC_ARCHIVE)

build-%/$(DEF_FRESH_ORIG_ARCHIVE): $(FRESH_SRC_ARCHIVE) build-%
	ln -s $(PWD)/$(FRESH_SRC_ARCHIVE) $@

build-%/$(DEB_FRESH_LIB)/debian: build-%/$(DEB_FRESH_LIB)
	mkdir $@
	#mkdir $@
	#cp -R debian-common/* $@
	#if [ -d debian-$(DIST) ]; then \
	#    cp -R debian-$(DIST)/* $@; \
	#fi

#
# Build source package
#


build-%/$(DEB_FRESH_LIB)*.dsc: $(DEB_FRESH_LIB)/debian $(DEF_FRESH_ORIG_ARCHIVE)
	cd $*/$(DEB_FRESH_LIB) && debuild -S -uc -us

src: $(DEB_FRESH_LIB)*.dsc;

signed-src: src
	DEBFULLNAME='$(DEBFULLNAME)' DEBEMAIL='$(DEBEMAIL)' debsign *.changes

dput: signed-src
	dput ppa:pasnox/ppa $(DEB_FRESH_LIB)*source.changes

#
# pbuilder configuration
#

#
# Build binary packages
#


# A bit of copy-pasting, but, easier than generic solution
result/%/$(DEB_FRESH_LIB)-ppa_i386.deb: result/% check-cowbuilder-base-%-i386 $(DEB_FRESH_LIB)*.dsc
	APTCACHEHARDLINK=no cowbuilder-dist $* i386 build $(DEB_FRESH_LIB)*.dsc
	@echo cp /var/cache/pbuilder/$*-i386/result/* result/$*

result/%/$(DEB_FRESH_LIB)-ppa_amd64.deb: result/% check-cowbuilder-base-%-amd64 $(DEB_FRESH_LIB)*.dsc
	cowbuilder-dist $* amd64 build $(DEB_FRESH_LIB)*.dsc
	@echo cp /var/cache/pbuilder/$*-amd64/result/* result/$*

ALL_DEBS = $(foreach dist,$(DISTS),\
				$(foreach arch,$(ARCHS),\
					result/$(dist)/$(DEB_FRESH_LIB)-ppa_$(arch).deb))

list-debs:
	@for dist in $(ALL_DEBS); do echo $$dist; done

all-debs: $(ALL_DEBS)

native-build: result $(DEB_FRESH_LIB) $(DEB_FRESH_LIB)/debian
	cd $(DEB_FRESH_LIB) && debuild -uc -us -b

#
# Prepare repository
#
result/repos:
	mkdir $@

repos: result/repos deb
	cd result && \
		for deb in `ls result`; do \
			cd result/repos && reprepo includedeb oneiric 
#
# Clean
#
clean:
	rm -rf build-*
	#rm -fr $(DEB_FRESH_LIB)
	#rm -f $(FRESH_NAME)*$(FRESH_SUFFIX)
	#rm -f $(FRESH_NAME)*.build
	#rm -f $(FRESH_NAME)*.upload
	#rm -f $(FRESH_NAME)*.dsc
	#rm -f $(FRESH_NAME)*.changes
	#rm -f *.deb
	#rm -rf result

