#ifndef FORMHANDLER_H
#define FORMHANDLER_H

#include <string>
#include "fileutil.h"

// namespace Error contains all errors that may occur in the formhandler
// These errors will be thrown out to all caller of formhandler,
// and error is needed to be handle
namespace FORMHANDLER_ERROR {

/* Client Error */
const int ErrListNameNotUnique = 10004; // List name is not unique
const int ErrEmptyFields = 10005;        // One or both fields (listName, videoDirPath) are empty
const int ErrListNameTooLong = 10006;    // listName length is greater than 20 characters
const int ErrInvalidListNameChars = 10007; // listName contains invalid characters
const int ErrVideoDirPathTooLong = 10008; // videoDirPath length is greater than 255 characters
const int ErrInvalidVideoDirPathFormat = 10009; // videoDirPath does not match the required format
const int ErrUnexpect = 10010; // unexpect error

/* No Error */
const int SUCCESS = 1;

}  // namespace FORMHANDLER_ERROR

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
    int validateFormData(const std::string& listName, const std::string& videoDirPath);
};

#endif  // FORMHANDLER_H
