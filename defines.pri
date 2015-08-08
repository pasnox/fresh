# some library infos
QMAKE_TARGET_COMPANY = "The Fresh Team"
QMAKE_TARGET_PRODUCT = "Fresh Framework"
QMAKE_TARGET_DESCRIPTION = "Qt Extension Framework"
QMAKE_TARGET_COPYRIGHT = "\\251 2005 - 2014 Filipe Azevedo and $$QMAKE_TARGET_COMPANY"
VERSION = 1.1.0

# make library exportable
DEFINES *= FRESH_CORE_BUILD

# Disable automatic casts
DEFINES *= \
    QT_NO_CAST_TO_ASCII \
    QT_NO_CAST_FROM_ASCII \
    QT_NO_CAST_FROM_BYTEARRAY \
    QT_NO_URL_CAST_FROM_STRING \
    QT_STRICT_ITERATORS

# Add shorters for literals
DEFINES *= \
    QL1S=QLatin1String \
    QL1C=QLatin1Char

greaterThan( QT_MAJOR_VERSION, 4 ) {
    DEFINES *= \
        QSL=QStringLiteral \
        QBAL=QByteArrayLiteral
} else {
    DEFINES *= \
        QSL=QString::fromUtf8 \
        QBAL=QByteArray
}
