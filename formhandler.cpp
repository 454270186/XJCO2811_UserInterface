#include <iostream>
#include <cctype>
#include <algorithm>
#include <regex>

#include "formhandler.h"
#include "fileutil.h"

const std::string XMLFilePath ="../XJCO2811_UserInterface/videolist_data.xml";

FormHandler::FormHandler() {
    fileUtil_ = new FileUtil(XMLFilePath);
}

FormHandler::~FormHandler() {
    delete fileUtil_;
}

// validateFormData() checks the validity of the list name and video directory path:
// 1. Ensures that neither list name nor video directory path is empty.
// 2. Verifies that the list name does not exceed 20 characters in length.
// 3. Checks if the list name contains only alphanumeric characters and spaces.
// 4. Ensures that the video directory path does not exceed 255 characters.
// 5. Validates the format of the video directory path using a regular expression.
// Params:
// - listName: A string representing the name of the list.
// - videoDirPath: A string representing the file path of the video directory.
// Returns:
// - bool: Returns true if both the list name and video directory path pass all the checks;
//   otherwise, returns false.
bool FormHandler::validateFormData(const std::string& listName, const std::string& videoDirPath) {
    if (listName.empty() || videoDirPath.empty()) {
        return false;
    }
 
    if (listName.length() > 20) {
        return false;
    }

    if (!std::all_of(listName.begin(), listName.end(), [](char c) {
        return std::isalnum(c) || c == ' ';
    })) {
        return false;
    }

    if (videoDirPath.length() > 255) {
        return false;
    }

    std::regex pathPattern(R"(^(?:[a-zA-Z]:)?[\\/]?[\w\s-]+([\\/][\w\s-]+)*[\\/]?(\.[\w]+)?)");
    if (!std::regex_match(videoDirPath, pathPattern)) {
        return false;
    }

    return true;
}

bool FormHandler::isListNameUnique(const std::string& newListName) {
    auto lists = fileUtil_->GetAllListsInfo();
    for (const auto& list : lists) {
        if (list.name == newListName) {
            return false;
        }
    }
    return true;
}

bool FormHandler::isListNameUnique(const std::string& newListName, int currentListID) {
    auto lists = fileUtil_->GetAllListsInfo();
    for (const auto& list : lists) {
        if (list.name == newListName && list.id != currentListID) {
            return false;
        }
    }
    return true;
}


// submitForm() handles the submission of a new list. It performs the following steps:
// 1. Validates the form data using validateFormData function.
// 2. Attempts to add a new list using the AddNewList method of the FileUtil class.
// Params:
// - listName: A string representing the name of the list.
// - videoDirPath: A string representing the file path of the video directory.
// Returns:
// - int: Returns -1 if validation fails or if AddNewList encounters an error.
//   If AddNewList succeeds, returns the ID of the new list.
int FormHandler::submitForm(const std::string& listName, const std::string& videoDirPath) {
    std::string error;
    if (!validateFormData(listName, videoDirPath)) {
        return -1;
    }

    if (!isListNameUnique(listName)) {
        return -1;
    }
    
    return fileUtil_->AddNewList(listName, videoDirPath, &error);
}

// editForm() handles the editing of an existing list. It performs the following steps:
// 1. Validates the new form data using validateFormData function.
// 2. Attempts to edit the list using the EditList method of the FileUtil class.
// Params:
// - listID: An integer representing the ID of the list to be edited.
// - newListName: A string representing the new name of the list.
// - newVideoDirPath: A string representing the new file path of the video directory.
// Returns:
// - int: Returns -1 if validation fails or if EditList encounters an error.
//   If EditList succeeds, returns 1.
int FormHandler::editForm(int listID, const std::string& newListName, const std::string& newVideoDirPath) {
    std::string error;
    if (!validateFormData(newListName, newVideoDirPath)) {
        return -1;
    }

    if (!isListNameUnique(newListName, listID)) {
        return -1;
    }

    return fileUtil_->EditList(listID, newListName, newVideoDirPath, &error);
}
