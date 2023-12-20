#include "fileutil.h"

#include <iostream>

#include "tinyxml2.h"

using namespace tinyxml2;
using namespace ERROR;

// Constructor of FileUtil needs a XML File Path
FileUtil::FileUtil(const string& XMLFilePath) : XMLFilePath_(XMLFilePath) {
    std::cout << "XML File Path: " << XMLFilePath_ << std::endl;
    XMLError err = xmlParser_.LoadFile(XMLFilePath_.c_str());
    if (err != XML_SUCCESS) {
        std::cout << "读取 xml 失败：" << xmlParser_.ErrorStr() << std::endl;
        return;
    }
}

int FileUtil::GetMaxID() {
    auto lists = GetAllListsInfo();
    if (lists.empty()) {
        return 0;
    } else {
        return lists.back().id;
    }
}

// PrintAll() prints out all content of the xml file
void FileUtil::PrintAll() {
    XMLElement* rootElement = xmlParser_.RootElement();
    if (rootElement && strcmp(rootElement->Name(), "lists") == 0) {
        // Traverse root element: <videolist>
        for (XMLElement* videolistElement = rootElement->FirstChildElement("videolist"); videolistElement;
             videolistElement = videolistElement->NextSiblingElement("videolist")) {
            // get <id> element
            XMLElement* idElement = videolistElement->FirstChildElement("id");
            const char* idText = idElement ? idElement->GetText() : nullptr;

            // get <name> element
            XMLElement* nameElement = videolistElement->FirstChildElement("name");
            const char* nameText = nameElement ? nameElement->GetText() : nullptr;

            // get <video_path> element
            XMLElement* videoPathElement = videolistElement->FirstChildElement("video_path");
            const char* videoPathText = videoPathElement ? videoPathElement->GetText() : nullptr;

            // print out
            if (idText && nameText && videoPathText) {
                std::cout << "ID: " << idText << ", Name: " << nameText << ", Video Path: " << videoPathText
                          << std::endl;
            }
        }
    }
}

// AddNewList() writes a new list into video-list-data file by given listname
// and video directory path
// Params:
// - error: error is a output-parameter, It only be assigned when the return
// value is -1
// Returns:
// Error number in error.h
int FileUtil::AddNewList(const string& listname, const string& videoDirPath, string* error = nullptr) {
    // Create a new XML element for the new list
    XMLElement* newListElement = xmlParser_.NewElement("videolist");

    // Create child elements for the new list
    XMLElement* idElement = xmlParser_.NewElement("id");
    XMLElement* nameElement = xmlParser_.NewElement("name");
    XMLElement* videoPathElement = xmlParser_.NewElement("video_path");

    // Set text content for the child elements
    idElement->SetText(GetMaxID() + 1);  // You need to implement GenerateNewListId() to
                                         // generate a new unique id
    nameElement->SetText(listname.c_str());
    videoPathElement->SetText(videoDirPath.c_str());

    // Append child elements to the new list element
    newListElement->InsertEndChild(idElement);
    newListElement->InsertEndChild(nameElement);
    newListElement->InsertEndChild(videoPathElement);

    // Append the new list element to the root element
    XMLElement* rootElement = xmlParser_.RootElement();
    if (rootElement && strcmp(rootElement->Name(), "lists") == 0) {
        rootElement->InsertEndChild(newListElement);

        // Save the modified XML document to the file
        if (xmlParser_.SaveFile(XMLFilePath_.c_str()) == XML_SUCCESS) {
            // Successfully added a new list
            return ERROR::SUCCESS;
        } else {
            if (error) {
                *error = "Failed to save the XML file.";
            }
            return ERROR::ErrXMLChangeSave;
        }
    } else {
        if (error) {
            *error = "Invalid XML file format.";
        }
        return ERROR::ErrInvalidXML;
    }

    // Error occurred
    return ERROR::ErrUnexpect;
}

// GetAllListsInfo() returns a vector of infomations of all lists
vector<ListInfo> FileUtil::GetAllListsInfo() {
    vector<ListInfo> listsInfo;

    //    xmlParser_.Clear();
    //    XMLError err = xmlParser_.LoadFile(XMLFilePath_.c_str());
    //    if (err != XML_SUCCESS) {
    //        std::cout << "读取 xml 失败：" << xmlParser_.ErrorStr() << std::endl;
    //        return listsInfo;
    //    }

    XMLElement* rootElement = xmlParser_.RootElement();
    if (rootElement && strcmp(rootElement->Name(), "lists") == 0) {
        // Traverse root element: <videolist>
        for (XMLElement* videolistElement = rootElement->FirstChildElement("videolist"); videolistElement;
             videolistElement = videolistElement->NextSiblingElement("videolist")) {
            ListInfo listInfo;

            XMLElement* idElement = videolistElement->FirstChildElement("id");
            listInfo.id = idElement ? atoi(idElement->GetText()) : -1;

            XMLElement* nameElement = videolistElement->FirstChildElement("name");
            listInfo.name = nameElement ? nameElement->GetText() : "";

            XMLElement* videoPathElement = videolistElement->FirstChildElement("video_path");
            listInfo.videoDirPath = videoPathElement ? videoPathElement->GetText() : "";

            listsInfo.push_back(listInfo);
        }
    }

    return listsInfo;
}

// EditList() edits existing list by given list id;
// Returns:
// Error number in error.h
int FileUtil::EditList(int listID, const string& newListName, const string& newVideoDirPath, string* error = nullptr) {
    XMLElement* rootElement = xmlParser_.RootElement();
    // check validation of xml file structure
    if (!rootElement || strcmp(rootElement->Name(), "lists") != 0) {
        if (error) {
            *error = "invalid xml file format";
        }
        return ERROR::ErrInvalidXML;
    }

    bool isFound = false;
    for (XMLElement* videolistEle = rootElement->FirstChildElement("videolist"); videolistEle;
         videolistEle = videolistEle->NextSiblingElement("videolist")) {
        // find target id
        XMLElement* idElement = videolistEle->FirstChildElement("id");
        int currentListID = idElement ? atoi(idElement->GetText()) : -1;

        if (currentListID != listID) {
            continue;
        }

        isFound = true;
        XMLElement* nameElement = videolistEle->FirstChildElement("name");
        if (nameElement) {
            nameElement->SetText(newListName.c_str());
        }

        XMLElement* videoDirElement = videolistEle->FirstChildElement("video_path");
        if (videoDirElement) {
            videoDirElement->SetText(newVideoDirPath.c_str());
        }

        if (xmlParser_.SaveFile(XMLFilePath_.c_str()) == XML_SUCCESS) {
            return ERROR::SUCCESS;
        } else {
            return ERROR::ErrXMLChangeSave;
        }
    }

    if (!isFound) {
        if (error) {
            *error = "list with id " + std::to_string(listID) + " not found";
        }
        return ERROR::ErrListIDNotFound;
    }

    return ERROR::ErrUnexpect;
}

// DelListByID() deletes a list by given id.
// if list ID does not exist, error occurs.
int FileUtil::DelListByID(int listID, string* error) {
    XMLElement* rootElement = xmlParser_.RootElement();
    // check validation of xml file structure
    if (!rootElement || strcmp(rootElement->Name(), "lists") != 0) {
        if (error) {
            *error = "invalid xml file structure";
        }
        return ERROR::ErrInvalidXML;
    }

    for (XMLElement* videolistEle = rootElement->FirstChildElement("videolist"); videolistEle;
         videolistEle = videolistEle->NextSiblingElement("videolist")) {
        XMLElement* idElement = videolistEle->FirstChildElement("id");
        int currentListID = idElement ? atoi(idElement->GetText()) : -1;

        if (currentListID == listID) {
            rootElement->DeleteChild(videolistEle);

            // save change
            if (xmlParser_.SaveFile(XMLFilePath_.c_str()) == XML_SUCCESS) {
                return ERROR::SUCCESS;
            } else {
                if (error) {
                    *error = "error while save chenge into file";
                }
                return ERROR::ErrXMLChangeSave;
            }
        }
    }

    if (error) {
        *error = "list with id " + std::to_string(listID) + " not found";
    }
    return ERROR::ErrListIDNotFound;
}

// GetVideosPathByListName() returns the videos path according to the given
// listname
string FileUtil::GetVideosPathByListName(const string& listname) {
    return "Not implement yet";
}
