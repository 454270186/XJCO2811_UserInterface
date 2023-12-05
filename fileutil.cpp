#include <iostream>
#include "fileutil.h"
#include "tinyxml2.h"

using namespace tinyxml2;

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
        return 1;
    } else {
        return lists.back().id;
    }
}

// PrintAll() prints out all content of the xml file
void FileUtil::PrintAll() {
    XMLElement* rootElement = xmlParser_.RootElement();
    if (rootElement && strcmp(rootElement->Name(), "lists") == 0) {
        // Traverse root element: <videolist>
        for (XMLElement* videolistElement = rootElement->FirstChildElement("videolist"); videolistElement; videolistElement = videolistElement->NextSiblingElement("videolist")) {
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
                std::cout << "ID: " << idText << ", Name: " << nameText << ", Video Path: " << videoPathText << std::endl;
            }
        }
    }
}

// AddNewList() writes a new list into video-list-data file by given listname and video directory path
// Params:
// - error: error is a output-parameter, It only be assigned when the return value is -1
// Returns:
// - -1: Some errors occur, the output-parameter error will be assigned with the error information.
// - new list id: if no error occurs, return new list id.
int FileUtil::AddNewList(const string& listname, const string& videoDirPath, string* error=nullptr) {
    // Create a new XML element for the new list
    XMLElement* newListElement = xmlParser_.NewElement("videolist");

    // Create child elements for the new list
    XMLElement* idElement = xmlParser_.NewElement("id");
    XMLElement* nameElement = xmlParser_.NewElement("name");
    XMLElement* videoPathElement = xmlParser_.NewElement("video_path");

    // Set text content for the child elements
    idElement->SetText(GetMaxID()+1);  // You need to implement GenerateNewListId() to generate a new unique id
    nameElement->SetText(listname.c_str());
    videoPathElement->SetText(videoDirPath.c_str());

    // Append child elements to the new list element
    newListElement->InsertEndChild(idElement);
    newListElement->InsertEndChild(nameElement);
    newListElement->InsertEndChild(videoPathElement);

    // Append the new list element to the root element
    XMLElement* rootElement = xmlParser_.RootElement();
    if (rootElement) {
        rootElement->InsertEndChild(newListElement);

        // Save the modified XML document to the file
        if (xmlParser_.SaveFile(XMLFilePath_.c_str()) == XML_SUCCESS) {
            // Successfully added a new list
            return atoi(idElement->GetText());
        } else {
            if (error) {
                *error = "Failed to save the XML file.";
            }
        }
    } else {
        if (error) {
            *error = "Invalid XML file format.";
        }
    }

    // Error occurred
    return -1;
}

// GetAllListsInfo() returns a vector of infomations of all lists
vector<ListInfo> FileUtil::GetAllListsInfo() {
    vector<ListInfo> listsInfo;

    XMLElement* rootElement = xmlParser_.RootElement();
    if (rootElement && strcmp(rootElement->Name(), "lists") == 0) {
        // Traverse root element: <videolist>
        for (XMLElement* videolistElement = rootElement->FirstChildElement("videolist"); videolistElement; videolistElement = videolistElement->NextSiblingElement("videolist")) {
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

// GetVideosPathByListName() returns the videos path according to the given listname
string FileUtil::GetVideosPathByListName(const string& listname) {
    return "Not implement yet";
}
