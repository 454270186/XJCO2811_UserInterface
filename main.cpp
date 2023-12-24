#include <QApplication>
#include <iostream>
#include "pagemanager.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    std::cout << "\n\n\033[1;32mBuild Successfully\033[0m\n";
    std::cout << "\n"
                 "     \033[1;32m_______..______     ______   .______     .___________.___   ___\033[0m     \n"
                 "    \033[1;32m/       ||   _  \\   /  __  \\  |   _  \\    |           |\\  \\ /  /\033[0m     \n"
                 "   \033[1;32m|   (----`|  |_)  | |  |  |  | |  |_)  |   `---|  |----` \\  V  /\033[0m      \n"
                 "    \033[1;32m\\   \\    |   ___/  |  |  |  | |      /        |  |       >   <\033[0m       \n"
                 "\033[1;32m.----)   |   |  |      |  `--'  | |  |\\  \\----.   |  |      /  .  \\\033[0m      \n"
                 "\033[1;32m|_______/    | _|       \\______/  | _| `._____|   |__|     /__/ \\__\\\033[0m     \n"
                 "                                                                         \n"
                 "\n";

    PageManager pagemanager;
    pagemanager.show();

    app.exec();
}
