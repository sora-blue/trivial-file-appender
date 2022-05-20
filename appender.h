//
// Created by HP on 2022/5/20.
//

#ifndef APPENDER_APPENDER_H
#define APPENDER_APPENDER_H

#include <functional>
#include <iostream>
#define A_LOG(x) std::cerr<<x<<std::endl

/*
 * View appender of the file, if it is appended.
 * @Parameters:
 *  filePath: source file location
 *  converter: (optional) convert content into other form, e.g., from Base64 to readable text
 * @Return value:
 *  If success, returns a pointer to content(if converter is there, converted content); else,
 *  returns nullptr.
 * */
char *ap_view(const char *filePath, std::function<void(char *)> converter);
char *ap_view(const char *filePath);

/*
 * Write an appender into new file.
 * @Parameters:
 *  filePath: source file location
 *  newPath: generated file location
 *  converter: (optional) convert content into other form, e.g., from readable text to base64
 * @Return value:
 *  If success, return 0; else, return -1.
 * @Description:
 *  You can also make $filePath and $newPAth the same without conflicts.
 * */
int ap_write(const char *filePath, const char *newPath, std::function<void(char *)> converter);
int ap_write(const char *filePath, const char *newPath);

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


#endif //APPENDER_APPENDER_H
