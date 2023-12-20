#ifndef ERROR_H
#define ERROR_H

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

#endif  // ERROR_H
