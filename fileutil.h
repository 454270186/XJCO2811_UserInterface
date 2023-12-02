#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <string>

#include "tinyxml2.h"

using std::string;


// FileUtil is the class for providing file operation
class FileUtil {
public:
    FileUtil(const string& XMLFilePath);

    void PrintAll();
    string GetVideosPathByListName(const string& listname);

private:
    string XMLFilePath_;
    tinyxml2::XMLDocument xmlParser_;
};

#endif // FILEUTIL_H
