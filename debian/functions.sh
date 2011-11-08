#!/bin/sh -e

###############################################################
# Check for & install required tools
###############################################################

PATH="$HOME/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
START_PWD="$PWD"

###############################################################
# Support functions
###############################################################

# Display a banner
# $1= message
banner() {
    echo ""
    echo "*********************************************************************"
    printf "%-.80s\n" "$1"
    echo "*********************************************************************"
    echo ""
}

# Unpack an archive
# $1= filename
unpack() {
    echo "Extracting `basename "$1"` ..."
    case "$1" in
        *.tar.gz) tar -zxf $@ ;;
        *.tar.bz2) tar -jxf $@ ;;
        *.bz2) bunzip2 -k $@ ;;
        *.zip) unzip -a -q $@ ;;
        *) die "Unknown archive type: $1" ;;
    esac
}

# Install a package
# $1= package
install_pkg() {
    echo "Installing $1 ..."
    if which apt-get > /dev/null 2>&1 ; then
        sudo apt-get install $1 || die "Failed to install $1"
    elif which yum >/dev/null 2>&1 ; then
        sudo yum install $1 || die "Failed to install $1"
    elif which urpmi >/dev/null 2>&1 ; then
        sudo urpmi $1 || die "Failed to install $1"
    else
        die "Don't know how to install $1"
    fi
}

# Copy a file
# $1= source, $2= target
copy_file() {
    if [ ! -e $2 ]; then
        cp "$1" "$2"
    fi
}
