#!/bin/sh -e

#export NAME="Filipe Azevedo"
export NAME="Nox P@sNox"
export EMAIL="pasnox@gmail.com"
export DEBNAME="$NAME"
export DEBFULLNAME="$NAME"
export DEBEMAIL="$EMAIL"

FRESH_NAME="fresh"
FRESH_VERSION="1.1.0"
FRESH_LIB="$FRESH_NAME"_"$FRESH_VERSION"
DEB_FRESH_LIB="$FRESH_NAME-$FRESH_VERSION"
FRESH_LIB_SRC="$FRESH_LIB-src"
FRESH_SUFFIX=".tar.gz"
FRESH_FILE="$FRESH_LIB$FRESH_SUFFIX"
FRESH_FILE_SRC="$FRESH_LIB_SRC$FRESH_SUFFIX"
FRESH_URL="https://github.com/downloads/pasnox/fresh/$FRESH_FILE_SRC"
FRESH_ARCHIVE_FILE=""

# load usefull functions
. ./functions.sh

# loader
banner "Packaging Fresh for Debian..."

# Getting the sources and uncompressing
if [ ! -d "$DEB_FRESH_LIB" ]; then
    banner "Getting sources..."
    
    if [ ! -e "$FRESH_FILE" ] && [ ! -e "$FRESH_FILE_SRC" ] ; then
        if [ -e "../fresh.pro" ] ; then
            cd ..
            git archive --prefix="$FRESH_LIB/" master | gzip -9 > "debian/$FRESH_FILE"
            FRESH_ARCHIVE_FILE="$FRESH_FILE"
            cd "$START_PWD"
        else
            wget "$FRESH_URL"
            FRESH_ARCHIVE_FILE="$FRESH_FILE_SRC"
        fi
    fi
    
    arc=`basename "$FRESH_ARCHIVE_FILE"`
    unpack "$arc"
    arc=`basename "$FRESH_ARCHIVE_FILE" $FRESH_SUFFIX`
    
    if [ "$arc" != "$DEB_FRESH_LIB" ] ; then
        mv "$arc" "$DEB_FRESH_LIB"
    fi
fi

DEBIAN_PATH="$START_PWD/$DEB_FRESH_LIB/debian"

cd "$DEB_FRESH_LIB"
if [ ! -d "$DEBIAN_PATH" ] ; then
    banner "Creating debian package folder..."
    dh_make -c "lgpl3" -f "$START_PWD/$FRESH_ARCHIVE_FILE" --createorig -l << EOF
EOF
fi

cd "$DEBIAN_PATH"
#if [ -e *.ex ] || [ -e *.EX ] ; then
    banner "Deleting temp files in debian folder..."
    rm *.ex *.EX > /dev/null 2>&1
#fi

if [ -e "$START_PWD/rules" ] ; then
    banner "Copying package files..."
    DEB_FILES="changelog control copyright rules"
    for file in $DEB_FILES ; do
        cp -f "$START_PWD/$file" .
    done
fi

#debuild -uc -us

debuild -S
cd ..
sudo pbuilder build ../*.dsc
