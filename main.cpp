#include "listset.h"
#include <iostream>
#include <fstream>
#include <string>
#include <QApplication>

int main(int argc, char** argv) {
    const std::string XMLFilePath = "../XJCO2811_UserInterface/videolist_data.xml";
    std::ofstream fileStream(XMLFilePath, std::ios::trunc);
    if (fileStream.is_open()) {
        fileStream << "<list>\n</list>" << std::endl;
        fileStream.close();
    }
    QApplication app(argc, argv);

    //MainWindow w;
    ListSet w;
    w.show();
    int i;
    int j;

    app.exec();
}