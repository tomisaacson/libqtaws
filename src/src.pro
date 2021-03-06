# Create a library.
TARGET = qtaws
TEMPLATE = lib
DEFINES += QTAWS_LIBRARY
CONFIG += warn_on
QT += network
QT -= gui

# Disable automatic ASCII conversions (best practice, especially for i18n support).
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII

# Add the embedded resources.
RESOURCES = ../qrc/aws.qrc

# Neaten the output directories.
CONFIG(debug,debug|release):  DESTDIR = ../debug
CONFIG(release,debug|release):DESTDIR = ../release
MOC_DIR = $$DESTDIR/$$TARGET-tmp
OBJECTS_DIR = $$DESTDIR/$$TARGET-tmp
RCC_DIR = $$DESTDIR/$$TARGET-tmp

include(core/core.pri)
include(sns/sns.pri)
include(sqs/sqs.pri)
