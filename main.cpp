#include "musicwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MusicWidget w;
    w.show();
    return a.exec();
}
