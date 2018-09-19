#include "seriport.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Seriport w;

    w.show();

    return a.exec();
}
