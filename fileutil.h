#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <string>
#include <vector>

#include "tinyxml2.h"

using std::string;
using std::vector;

class ListInfo {
public:
    int id;
    string name{""};
    string videoDirPath{""};
};

// FileUtil is the class for providing file operation
class FileUtil {
public:
    FileUtil(const string& XMLFilePath);

    void PrintAll();
    string GetVideosPathByListName(const string& listname);
    vector<ListInfo> GetAllListsInfo();

    int AddNewList(const string& listname, const string& videoDirPath, string* error);
    int EditList(int listID, const string& newListName, const string& newVideoDirPath, string* error);
    int DelListByID(int listID, string* error);

private:
    int GetMaxID();

    string XMLFilePath_;
    tinyxml2::XMLDocument xmlParser_;
};

#endif  // FILEUTIL_H
