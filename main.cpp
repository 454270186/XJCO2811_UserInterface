#include <QApplication>
#include <fstream>
#include <iostream>
#include <string>
#include "listset.h"

int main(int argc, char** argv) {
    //    const std::string XMLFilePath = "../XJCO2811_UserInterface/videolist_data.xml";
    //    std::ofstream fileStream(XMLFilePath, std::ios::trunc);
    //    if (fileStream.is_open()) {
    //        fileStream << "<lists>\n</lists>" << std::endl;
    //        fileStream.close();
    //    }
    QApplication app(argc, argv);

    //MainWindow w;
    ListSet w;
    w.show();

    app.exec();
}
