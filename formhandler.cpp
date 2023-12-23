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

// validateFormData() checks the validity of the list name and video directory path.
// The function performs the following validations:
// 1. Ensures that neither list name nor video directory path is empty.
//    - Returns ErrEmptyFields if either is empty.
// 2. Verifies that the list name does not exceed 20 characters in length.
//    - Returns ErrListNameTooLong if the list name is too long.
// 3. Checks if the list name contains only alphanumeric characters and spaces.
//    - Returns ErrInvalidListNameChars if the list name contains invalid characters.
// 4. Ensures that the video directory path does not exceed 255 characters.
//    - Returns ErrVideoDirPathTooLong if the path is too long.
// 5. Validates the format of the video directory path using a regular expression.
//    - Returns ErrInvalidVideoDirPathFormat if the format is invalid.
// Parameters:
// - listName: A string representing the name of the list.
// - videoDirPath: A string representing the file path of the video directory.
// Returns:
// - int: Returns an error code corresponding to the specific validation failure.
//   If all validations pass, returns SUCCESS.
int FormHandler::validateFormData(const std::string& listName, const std::string& videoDirPath) {
    if (listName.empty() || videoDirPath.empty()) {
        return FORMHANDLER_ERROR::ErrEmptyFields;
    }

    if (listName.length() > 20) {
        return FORMHANDLER_ERROR::ErrListNameTooLong;
    }

    if (!std::all_of(listName.begin(), listName.end(), [](char c) { return std::isalnum(c) || c == ' '; })) {
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
// 1. Validates the form data using the validateFormData function. If validation fails,
//    returns the corresponding error code from validateFormData.
// 2. Checks if the list name is unique among all lists using the isListNameUnique function.
//    If the list name is not unique, returns ErrListNameNotUnique.
// 3. Attempts to add a new list using the AddNewList method of the FileUtil class.
// Params:
// - listName: A string representing the name of the list.
// - videoDirPath: A string representing the file path of the video directory.
// Returns:
// - int: Returns an error code if validation fails or if the list name is not unique.
//   If AddNewList succeeds, returns the result of the AddNewList method (typically the ID of the new list).
int FormHandler::submitForm(const std::string& listName, const std::string& videoDirPath) {
    std::string error;
    int validationResult = validateFormData(listName, videoDirPath);

    if (validationResult != FORMHANDLER_ERROR::SUCCESS) {
        return validationResult;
    }

    if (!isListNameUnique(listName)) {
        return FORMHANDLER_ERROR::ErrListNameNotUnique;
    }

    return fileUtil_->AddNewList(listName, videoDirPath, &error);
}

// editForm() handles the editing of an existing list. It performs the following steps:
// 1. Validates the new form data using the validateFormData function. If validation fails,
//    returns the corresponding error code from validateFormData.
// 2. Checks if the new list name is unique among all lists except the current list using
//    the isListNameUnique function. If the new list name is not unique, returns ErrListNameNotUnique.
// 3. Attempts to edit the list using the EditList method of the FileUtil class.
// Params:
// - listID: An integer representing the ID of the list to be edited.
// - newListName: A string representing the new name of the list.
// - newVideoDirPath: A string representing the new file path of the video directory.
// Returns:
// - int: Returns an error code if validation fails or if the new list name is not unique.
//   If EditList succeeds, returns the result of the EditList method (typically 1 for success).
int FormHandler::editForm(int listID, const std::string& newListName, const std::string& newVideoDirPath) {
    std::string error;
    int validationResult = validateFormData(newListName, newVideoDirPath);

    if (validationResult != FORMHANDLER_ERROR::SUCCESS) {
        return validationResult;
    }

    if (!isListNameUnique(newListName, listID)) {
        return FORMHANDLER_ERROR::ErrListNameNotUnique;
    }

    return fileUtil_->EditList(listID, newListName, newVideoDirPath, &error);
}
