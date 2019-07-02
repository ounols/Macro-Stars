#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>

int main () {

    bool isQuit = false;

    while(!isQuit) {
        FILE* fp = nullptr;
        fp = popen("ps aux | grep \"Macro\"", "r");

        char adb_result_str[1024] = {};
        std::string adb_result;

        fread(adb_result_str, 1024, 1, fp);
        adb_result = adb_result_str;

        std::string::size_type n;
        n = adb_result.find("macro-stars");

        if(n == std::string::npos) {
            pclose(fp);
            //system("shutdown -r now");
            return 0;
        }

        usleep(1000);

        pclose(fp);
    }

    return 0;
}