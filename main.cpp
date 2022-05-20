#include <iostream>
#include "appender.h"
int main() {
    // write test
    ap::ap_write("D:\\EnglishRoot\\playground\\test.jpg", "D:\\EnglishRoot\\playground\\test2.jpg", "For you who won't melt down.");
    // view test
    char *msg = ap::ap_view("D:\\EnglishRoot\\playground\\test2.jpg");
    printf("%s\n", msg);
    // delete test
    ap::ap_delete("D:\\EnglishRoot\\playground\\test2.jpg", "D:\\EnglishRoot\\playground\\test3.jpg");
    ap::ap_view("D:\\EnglishRoot\\playground\\test3.jpg");
    return 0;
}
