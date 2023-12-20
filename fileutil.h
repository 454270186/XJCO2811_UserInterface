#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <string>
#include <vector>

#include "tinyxml2.h"

using std::string;
using std::vector;

// namespace Error contains all errors that may occur in the FileUtil
// These errors will be thrown out to all caller of FileUtil,
// and error is needed to be handle
namespace ERROR {

/* Internal Error */
const int ErrXMLParserInit = 10000;  // error while init XML parser
const int ErrXMLChangeSave = 10001;  // error while save change of XML file
const int ErrInvalidXML = 10002;     // invalid XML file format
const int ErrUnexpect = 12134;       // unexpect error

/* Client Error */
const int ErrListIDNotFound = 10003;  // error while list id not found

/* No Error */
const int SUCCESS = 1;  // No error occurs, operate successfully
}  // namespace ERROR

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
