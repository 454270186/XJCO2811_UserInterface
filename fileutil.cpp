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

// GetVideosPathByListName() returns the videos path according to the given listname
string FileUtil::GetVideosPathByListName(const string& listname) {
    return "Not implement yet";
}