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
FRESH_FILE_SRC=$(FRESH_LIB_SRC)$(FRESH_SUFFIX)
DEB_FRESH_ORIG_FILE=$(DEB_FRESH_LIB)-4.orig$(FRESH_SUFFIX)
DEB_PATH=$(DEB_FRESH_LIB)/debian
FRESH_URL=https://github.com/downloads/pasnox/fresh/$(FRESH_FILE_SRC)

DIST=oneiric
ARCH=i386

all:

clean:
	rm -fr $(DEB_FRESH_LIB)
	rm -f $(FRESH_NAME)*$(FRESH_SUFFIX)
	rm -f $(FRESH_NAME)*.build
	rm -f $(FRESH_NAME)*.upload
	rm -f $(FRESH_NAME)*.dsc
	rm -f $(FRESH_NAME)*.changes
	rm -rf result

test:
	echo $(FRESH_FILE)


$(FRESH_FILE_SRC):
	cd .. && git archive --prefix=$(DEB_FRESH_LIB)/ master | gzip -9 > debian/$@

$(DEB_FRESH_LIB): $(FRESH_FILE_SRC)
	tar -xf $<

$(DEB_FRESH_ORIG_FILE): $(FRESH_FILE_SRC)
	ln -s $< $@

src: $(DEB_FRESH_ORIG_FILE) $(DEB_FRESH_LIB)
	cd $(DEB_FRESH_LIB) && debuild -S -uc -us

sign-src: src
	DEBFULLNAME='$(DEBFULLNAME)' DEBEMAIL='$(DEBEMAIL)' debsign *.changes

dput: sign-src
	dput ppa:pasnox/ppa $(DEB_FRESH_LIB)*source.changes

result:
	mkdir $@
	
result/repos:
	mkdir $@

#deb: src result
deb: result
	#sudo DIST=$(DIST) ARCH=$(ARCH) pbuilder --build $(DEB_FRESH_LIB)*.dsc
	cp /var/cache/pbuilder/$(DIST)-$(ARCH)/result/* result

repos: result/repos deb
	cd result && \
		for deb in `ls result`; do \
			cd result/repos && reprepo includedeb oneiric 

