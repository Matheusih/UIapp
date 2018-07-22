QT += widgets
requires(qtConfig(fontcombobox))

HEADERS     =   mainwindow.h \
                diagramitem.h \
                diagramscene.h \
                arrow.h \
                diagramtextitem.h \
    graphicsproxywidget.h \
    graphicsview.h \
    myarrow.h
SOURCES     =   mainwindow.cpp \
                diagramitem.cpp \
                main.cpp \
                arrow.cpp \
                diagramtextitem.cpp \
                diagramscene.cpp \
    graphicsproxywidget.cpp \
    graphicsview.cpp \
    myarrow.cpp
RESOURCES   =   diagramscene.qrc \
    diagramscene.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/diagramscene
INSTALLS += target
