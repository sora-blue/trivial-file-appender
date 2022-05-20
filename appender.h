#ifndef APPENDER_APPENDER_H
#define APPENDER_APPENDER_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
namespace ap{
    /*
     * View appender of the file, if it is appended.
     * @Parameters:
     *  filePath: source file location
     * @Return value:
     *  If success, returns a pointer to content; else,
     *  returns nullptr.
     * */
    char *ap_view(const char *filePath);

    /*
     * Write an appender into new file.
     * @Parameters:
     *  filePath: source file location
     *  newPath: generated file location
     *  content: msg to write
     * @Return value:
     *  If success, return 0; else, return -1.
     * @Description:
     *  You can also make $filePath and $newPAth the same without conflicts.
     * */
    int ap_write(const char *filePath, const char *newPath, const char *content);

    /*
     * Delete an appender, write the last into new file.
     * @Parameters:
     *  filePath: source file location
     *  newPath: generated file location
     * @Return value:
     *  If success, return 0; else, return -1.
     * @Description:
     *  You can also make $filePath and $newPAth the same without conflicts.
     * */
    int ap_delete(const char *filePath, const char *newPath);

}
#endif //APPENDER_APPENDER_H
