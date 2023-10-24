#include "controller.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller controller;

    controller.show();

    std::cout<<"After show"<<std::endl;
    return a.exec();
}
