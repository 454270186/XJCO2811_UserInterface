#ifndef FORMHANDLER_H
#define FORMHANDLER_H

#include <string>
#include "fileutil.h"

class FormHandler {
public:
    FormHandler();
    ~FormHandler();

    int submitForm(const std::string& listName, const std::string& videoDirPath);
    int editForm(int listID, const std::string& newListName, const std::string& newVideoDirPath);

private:
    FileUtil* fileUtil_;
    bool isListNameUnique(const std::string& newListName);
    bool isListNameUnique(const std::string& newListName, int currentListID);
    bool validateFormData(const std::string& listName, const std::string& videoDirPath);
};

#endif // FORMHANDLER_H
