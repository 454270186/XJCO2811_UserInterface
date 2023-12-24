#ifndef LISTSETRESOURCE_H
#define LISTSETRESOURCE_H

#include <vector>

#include "fileutil.h"

class ListSetResource {
public:
    ListSetResource();

    FileUtil* fileUtil_;
    std::vector<ListInfo> ListInfo_;
    int currentBtnIndex_{0};
    bool hasUnfinishedNewList_{false};
    bool isChineseLanguage_{false};
    bool isListButtonClicked_{false};
};

#endif  // LISTSETRESOURCE_H
