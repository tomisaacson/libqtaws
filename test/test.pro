include(../common.pri)
TEMPLATE = app
QT += network testlib
QT -= gui
CONFIG += testcase
SOURCES += test.cpp

# Disable automatic ASCII conversions (best practice, especially for i18n support).
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII

# Add the embedded resources.
RESOURCES = ../qrc/aws.qrc

# Neaten the output directories.
CONFIG(debug,debug|release):   DESTDIR = $$absolute_path($$OUT_PWD/../debug)
CONFIG(release,debug|release): DESTDIR = $$absolute_path($$OUT_PWD/../release)
LIBS += -L$$DESTDIR
TEMPDIR = $$DESTDIR/$$TARGET-tmp
MOC_DIR = $$DESTDIR/$$TARGET-tmp
OBJECTS_DIR = $$DESTDIR/$$TARGET-tmp
QMAKE_RPATHDIR += $$DESTDIR
RCC_DIR = $$DESTDIR/$$TARGET-tmp

# Link to the libqtaws library.
LIBS += -lqtaws

# Code coverage reporting (for Linux at least).
unix {
    # Enable gcov compile and link flags.
    QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
    QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage
    QMAKE_CXXFLAGS_RELEASE -= -O1 -O2 -O3

    # Generate gcov's gcda files by executing the test program.
    gcov.depends = $$DESTDIR/test
    gcov.target = $$TEMPDIR/test.gcda
    gcov.commands = $$DESTDIR/test

    # Generate an lcov tracefile from gcov's gcda files.
    lcov.depends = $$TEMPDIR/test.gcda
    lcov.target = $$DESTDIR/coverage.info
    lcov.commands = lcov --capture --base-directory ../src --directory $$TEMPDIR --output $$DESTDIR/coverage.info --quiet; \
                    lcov --remove $$DESTDIR/coverage.info '"/usr/include/*/*"' \
                         '"src/core/qmessageauthenticationcode.cpp"' \
                         '"*/test/*"' '*/*-tmp/*' --output $$DESTDIR/coverage.info --quiet

    # Generate HTML coverage reports from lcov's tracefile.
    coverage.depends = $$DESTDIR/coverage.info
    coverage.commands += genhtml --output-directory $$DESTDIR/coverage_html --prefix $$TOPDIR/src --quiet --title libqtaws $$DESTDIR/coverage.info

    # Include the above custom targets in the generated build scripts (eg Makefile).
    QMAKE_EXTRA_TARGETS += coverage gcov lcov

    # Clean up files generated by the above custom targets.
    QMAKE_CLEAN += build/*.gcda build/*.gcno build/coverage.info
    QMAKE_DISTCLEAN += -r coverage_html
}

include(core/core.pri)
