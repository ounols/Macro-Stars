
#ifdef _DEBUG
#pragma comment(lib, "lib/opencv_world340d.lib")
#else
#pragma comment(lib, "lib/opencv_world340.lib")
#endif
#pragma comment(lib, "winmm.lib")

//#include <cv.h>
//#include <highgui.h>
//#include <opencv2/core/core.hpp>
//#include "opencv2/imgproc/imgproc.hpp"
//#include <opencv2/highgui/highgui.hpp> 
//#include <opencv2/opencv.hpp>
//#include <iostream> 
//#include <Windows.h>
//#include <crtdbg.h>
#include "MainProc.h"
#include "MacroDef.h"
#include <iostream>
#include <opencv2/core/version.hpp>

#ifdef WIN32
#include <vld.h>
#endif
#include "GameClientMgr.h"
#include <fstream>


int main(int argc, char* argv[]) {

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::cout << "OpenCV Version : " << CV_VERSION << std::endl;

	MainProc* main_proc = new MainProc();

	for(int i = 1; i < argc; i++) {
		if(std::string(argv[i]) == "GunStars") {
			main_proc->SetGunStars(true);
			GAME->SetGunStarsPath(std::string(argv[i + 1]));
			std::cout << "\nGunstars : " << GAME->GetGunStarsPath() << "\n";
		}

		if (std::string(argv[i]) == "Market") {
			main_proc->SetMarket(true);
			std::cout << "\nSetting Open Market\n";
		}

		if (std::string(argv[i]) == "VPN") {
			GAME->SetIsVpn(true);
			std::cout << "\nSetting VPN\n";
		}
#ifdef WIN32
		if (std::string(argv[i]) == "auto_reboot") {
			GAME->SetIsAutoReboot(true);
			std::cout << "\nSetting auto reboot\n";
			SYSTEMTIME oTime;
			GetLocalTime(&oTime);
			oTime.wHour = (oTime.wHour + 9) % 24;

			std::cout << "time : " << oTime.wHour << "�� " << oTime.wMinute << "��\n";

		}
#endif
		//if (std::string(argv[i]) == "http") {
		//	main_proc->SetReport(true);
		//	std::cout << "\nhttp on\n";
		//}
	}

	while(!main_proc->b_isQuit) {
		main_proc->Update();
	}

	SAFE_DELETE(main_proc);

	return 0;
}
