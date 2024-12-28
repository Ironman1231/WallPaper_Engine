#include "Wallpaper_withUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Wallpaper_withUI w;
    w.show();
    return a.exec();
}
