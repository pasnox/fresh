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

all:

#
# Help
#
help:
	@echo 'Top level targets:'
	@echo 'src          Create source package (.dsc, .chagnes). Package is not signed'
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
check_pbuilder_rc:
	if [ ! -f $${HOME}/.pbuilderrc ]; then \
		echo; \
		echo ~/.pbuilderrc does not exist. Copy it to your home:;\
		echo '    ' cp `pwd`/pbuilderrc $${HOME}/.pbuilderrc; \
		echo; \
		exit 1; \
	fi
	# If the file exists, lets hope it is valid

test: check_pbuilder_base

check_pbuilder_base:
	if [ ! -f /var/cache/pbuilder/$(DIST)-$(ARCH)-base.tgz ]; then \
		echo; \
		echo pbuilder chroot for distribution $(DIST) architecture $(ARCH) does not exists. Create it with command:;\
		echo '    ' sudo DIST=$(DIST) ARCH=$(ARCH) pbuilder create; \
		echo; \
		exit 1; \
	fi

check_pbuilder: check_pbuilder_rc check_pbuilder_base

#
# Build binary packages
#
result:
	mkdir $@
	
deb: check_pbuilder src result 
	sudo DIST=$(DIST) ARCH=$(ARCH) pbuilder --build $(DEB_FRESH_LIB)*.dsc
	cp /var/cache/pbuilder/$(DIST)-$(ARCH)/result/* result

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

