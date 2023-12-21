#include "mainwindowresource.h"

MainWindowResource::MainWindowResource() {
    // init fileutil and listinfo
    fileUtil_ = new FileUtil("../XJCO2811_UserInterface/videolist_data.xml");
    listinfo_ = fileUtil_->GetAllListsInfo();
}
