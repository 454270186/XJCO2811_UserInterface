#ifndef FORMHANDLER_H
#define FORMHANDLER_H

#include <string>

#include "fileutil.h"

class FormHandler {
public:
    FormHandler();
    ~FormHandler();

    bool validateFormData(const std::string& listName, const std::string& videoDirPath);

    int submitForm(const std::string& listName, const std::string& videoDirPath);

private:
    FileUtil* fileUtil_;
};

#endif // FORMHANDLER_H
