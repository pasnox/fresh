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

DIST=oneiric
ARCH=i386

all:

#
# Prepare sources
#

$(FRESH_SRC_ARCHIVE):
	cd .. && git archive --prefix=$(DEB_FRESH_LIB)/ new-deb-package | gzip -9 > debian/$@

$(DEB_FRESH_LIB): $(FRESH_SRC_ARCHIVE)
	tar -xf $<

$(DEF_FRESH_ORIG_ARCHIVE): $(FRESH_SRC_ARCHIVE)
	ln -s $< $@

$(DEB_FRESH_LIB)/debian:
	mkdir $@
	cp -R debian-common/* $@
	cp -R debian-squeeze/* $@

#
# Build source package
#

src: $(DEF_FRESH_ORIG_ARCHIVE) $(DEB_FRESH_LIB) $(DEB_FRESH_LIB)/debian
	cd $(DEB_FRESH_LIB) && debuild -S -uc -us

sign-src: src
	DEBFULLNAME='$(DEBFULLNAME)' DEBEMAIL='$(DEBEMAIL)' debsign *.changes

dput: sign-src
	dput ppa:pasnox/ppa $(DEB_FRESH_LIB)*source.changes

#
# Build binary packages
#
result:
	mkdir $@
	
deb: src result
	#sudo DIST=$(DIST) ARCH=$(ARCH) pbuilder --build $(DEB_FRESH_LIB)*.dsc
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

