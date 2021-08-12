#include "TetrisPanel.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TetrisPanel w;
    w.show();
    return a.exec();
}
