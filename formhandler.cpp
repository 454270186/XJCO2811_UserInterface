#include <algorithm>
#include <cctype>
#include <iostream>
#include <regex>

#include "fileutil.h"
#include "formhandler.h"

const std::string XMLFilePath = "../XJCO2811_UserInterface/videolist_data.xml";

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
int FormHandler::validateFormData(const std::string& listName, const std::string& videoDirPath) {
    if (listName.empty() || videoDirPath.empty()) {
        return FORMHANDLER_ERROR::ErrEmptyFields;
    }

    if (listName.length() > 20) {
        return FORMHANDLER_ERROR::ErrListNameTooLong;
    }

    if (!std::all_of(listName.begin(), listName.end(), [](char c) { 
        return std::isalnum(c) || c == ' '; })) {
        return FORMHANDLER_ERROR::ErrInvalidListNameChars;
    }

    if (videoDirPath.length() > 255) {
        return FORMHANDLER_ERROR::ErrVideoDirPathTooLong;
    }

    std::regex pathPattern(R"(^(\/|[a-zA-Z]:)?(?:\.{1,2}\/)?(?:[\w.-]+\/)*(?:[\w.-]+)?$)");
    if (!std::regex_match(videoDirPath, pathPattern)) {
        return FORMHANDLER_ERROR::ErrInvalidVideoDirPathFormat;
    }

    return FORMHANDLER_ERROR::SUCCESS;
}


// isListNameUnique() without list ID checks if a given list name is unique across all lists.
// It iterates through all existing lists and compares their names with the provided name.
// Params:
// - newListName: A string representing the name of the list to be checked for uniqueness.
// Returns:
// - bool: Returns true if the provided name is unique; otherwise, returns false.
bool FormHandler::isListNameUnique(const std::string& newListName) {
    auto lists = fileUtil_->GetAllListsInfo();
    for (const auto& list : lists) {
        if (list.name == newListName) {
            return false;
        }
    }
    return true;
}

// isListNameUnique() with list ID checks if a given list name is unique across all lists,
// excluding the list with the specified ID. This is useful for editing an existing list
// where the name should be unique among all other lists except itself.
// Params:
// - newListName: A string representing the new name of the list to be checked for uniqueness.
// - currentListID: An integer representing the ID of the list being edited.
// Returns:
// - bool: Returns true if the provided name is unique among all lists except the one with the given ID;
//   otherwise, returns false.
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
// 2. Checks if the list name is unique among all lists using isListNameUnique function.
// 3. Attempts to add a new list using the AddNewList method of the FileUtil class.
// Params:
// - listName: A string representing the name of the list.
// - videoDirPath: A string representing the file path of the video directory.
// Returns:
// - int: Returns -1 if validation fails, if the list name is not unique, or if AddNewList encounters an error.
//   If AddNewList succeeds, returns the ID of the new list.
int FormHandler::submitForm(const std::string& listName, const std::string& videoDirPath) {
    std::string error;
    int validationResult = validateFormData(listName, videoDirPath);
    std::cout<<"shit6"<<std::endl;

    if (validationResult != FORMHANDLER_ERROR::SUCCESS) {
        std::cout<<"shit7"<<std::endl;
        return validationResult;
    }

    std::cout<<"shit8"<<std::endl;

    if (!isListNameUnique(listName)) {
        std::cout<<"shit9"<<std::endl;
        return FORMHANDLER_ERROR::ErrListNameNotUnique;
    }

    std::cout<<"shit10"<<std::endl;

    return fileUtil_->AddNewList(listName, videoDirPath, &error);
}

// editForm() handles the editing of an existing list. It performs the following steps:
// 1. Validates the new form data using validateFormData function.
// 2. Checks if the new list name is unique among all lists except the current list using isListNameUnique function.
// 3. Attempts to edit the list using the EditList method of the FileUtil class.
// Params:
// - listID: An integer representing the ID of the list to be edited.
// - newListName: A string representing the new name of the list.
// - newVideoDirPath: A string representing the new file path of the video directory.
// Returns:
// - int: Returns -1 if validation fails, if the new list name is not unique, or if EditList encounters an error.
//   If EditList succeeds, returns 1.
int FormHandler::editForm(int listID, const std::string& newListName, const std::string& newVideoDirPath) {
    std::string error;
    int validationResult = validateFormData(newListName, newVideoDirPath);
    std::cout<<"shit1"<<std::endl;

    if (validationResult != FORMHANDLER_ERROR::SUCCESS) {
        std::cout<<"shit2"<<std::endl;
        return validationResult;
    }

    std::cout<<"shit3"<<std::endl;

    if (!isListNameUnique(newListName, listID)) {
        std::cout<<"shit4"<<std::endl;
        return FORMHANDLER_ERROR::ErrListNameNotUnique;
    }
    std::cout<<"shit5"<<std::endl;

    return fileUtil_->EditList(listID, newListName, newVideoDirPath, &error);
}
