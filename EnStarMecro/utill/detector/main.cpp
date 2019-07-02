#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>

int main () {

    bool isQuit = false;
    std::cout << "[매크로 감지기] 실행 준비 완료. 실행\n";

    while(!isQuit) {
        FILE* fp = nullptr;
        fp = popen("pgrep \"MacroStars.app\"", "r");

        char adb_result_str[1024] = {};
        std::string adb_result;

        fread(adb_result_str, 1024, 1, fp);
        adb_result = adb_result_str;

        

        if(adb_result.empty()) {
            pclose(fp);
            std::cout << "[매크로 감지기] 매크로가 실행되고 있지 않음. 5초 후 재부팅 실시.\n";
            usleep(5000 * 1000);
            system("shutdown -r now");
            return 0;
        }


        usleep(10000 * 1000);

        pclose(fp);
    }

    return 0;
}