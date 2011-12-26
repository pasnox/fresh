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
ARCH=i386

DISTS = squeeze oneiric
ARCHS = i386 amd64

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

$(DEB_FRESH_LIB): $(FRESH_SRC_ARCHIVE)
	tar -xf $<

$(DEF_FRESH_ORIG_ARCHIVE): $(FRESH_SRC_ARCHIVE)
	ln -s $< $@

$(DEB_FRESH_LIB)/debian: $(DEB_FRESH_LIB)
	mkdir $@
	cp -R debian-common/* $@
	if [ -d debian-$(DIST) ]; then \
	    cp -R debian-$(DIST)/* $@; \
	fi

#
# Build source package
#

src: $(DEF_FRESH_ORIG_ARCHIVE) $(DEB_FRESH_LIB) $(DEB_FRESH_LIB)/debian
	cd $(DEB_FRESH_LIB) && debuild -S -uc -us

signed-src: src
	DEBFULLNAME='$(DEBFULLNAME)' DEBEMAIL='$(DEBEMAIL)' debsign *.changes

dput: signed-src
	dput ppa:pasnox/ppa $(DEB_FRESH_LIB)*source.changes

#
# pbuilder configuration
#
check-pbuilder-rc:
	if [ ! -f $${HOME}/.pbuilderrc ]; then \
		echo; \
		echo ~/.pbuilderrc does not exist. Copy it to your home:;\
		echo '    ' cp `pwd`/pbuilderrc $${HOME}/.pbuilderrc; \
		echo; \
		exit 1; \
	fi
	# If the file exists, lets hope it is valid


check-pbuilder-base-%-i386: check-pbuilder-rc
	if [ ! -f /var/cache/pbuilder/$*-i386-base.tgz ]; then \
		echo; \
		echo pbuilder chroot for distribution $* architecture i386 does not exists. Create it with command:;\
		echo '    ' sudo DIST=$* ARCH=i386 pbuilder create; \
		echo; \
		exit 1; \
	fi

check-pbuilder-base-%-amd64: check-pbuilder-rc
	if [ ! -f /var/cache/pbuilder/$*-amd64-base.tgz ]; then \
		echo; \
		echo pbuilder chroot for distribution $* architecture amd64 does not exists. Create it with command:;\
		echo '    ' sudo DIST=$* ARCH=amd64 pbuilder create; \
		echo; \
		exit 1; \
	fi

#
# Build binary packages
#
$(foreach dist,$(DISTS),result/$(dist)):
	mkdir -p $@

# A bit of copy-pasting, but, easier than generic solution
result/%/$(DEB_FRESH_LIB)-ppa_i386.deb: result/% check-pbuilder-base-%-i386 $(DEB_FRESH_LIB)*.dsc
	@echo sudo DIST=$* ARCH=i386 pbuilder --build $(DEB_FRESH_LIB)*.dsc
	@echo cp /var/cache/pbuilder/$*-i386/result/* result/$*

result/%/$(DEB_FRESH_LIB)-ppa_amd64.deb: result/% check-pbuilder-base-%-amd64 $(DEB_FRESH_LIB)*.dsc
	@echo sudo DIST=$* ARCH=amd64 pbuilder --build $(DEB_FRESH_LIB)*.dsc
	@echo cp /var/cache/pbuilder/$*-amd64/result/* result/$*

ALL_DEBS = $(foreach dist,$(DISTS),\
				$(foreach arch,$(ARCHS),\
					result/$(dist)/$(DEB_FRESH_LIB)-ppa_$(arch).deb))
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
	rm -fr $(DEB_FRESH_LIB)
	rm -f $(FRESH_NAME)*$(FRESH_SUFFIX)
	rm -f $(FRESH_NAME)*.build
	rm -f $(FRESH_NAME)*.upload
	rm -f $(FRESH_NAME)*.dsc
	rm -f $(FRESH_NAME)*.changes
	rm -f *.deb
	rm -rf result

