
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

#include <vld.h>



void main() {

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::cout << "OpenCV Version : " << CV_VERSION << std::endl;

	MainProc* main_proc = new MainProc();

	while(!main_proc->b_isQuit) {
		main_proc->Update();
	}

	SAFE_DELETE(main_proc);

	
}
