#include "listsetresource.h"

ListSetResource::ListSetResource() {
    fileUtil_ = new FileUtil("../XJCO2811_UserInterface/videolist_data.xml");
    ListInfo_ = fileUtil_->GetAllListsInfo();
}
