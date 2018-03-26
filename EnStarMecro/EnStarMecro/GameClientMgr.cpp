#include "GameClientMgr.h"
#include <opencv2/opencv.hpp>
#include "ResMgr.h"

#include <stdio.h>
#include <errno.h>   /* errno */

using namespace cv;

#define popen _popen
#define pclose _pclose

IMPLEMENT_SINGLETON(GameClientMgr);


GameClientMgr::GameClientMgr() {

}


GameClientMgr::~GameClientMgr() {

	if (m_screenImage != nullptr) {
		cvReleaseImage(&m_screenImage);
		SAFE_DELETE(m_screenImage);
	}


}


void GameClientMgr::GetGameHWND() {

	if (m_screenImage != nullptr) {
		cvReleaseImage(&m_screenImage);
		SAFE_DELETE(m_screenImage);
	}

	m_isSupportExecOut = true;

	system("adb\\adb kill-server");
	system("adb\\adb start-server");
	system("adb\\adb connect 127.0.0.1:62001");	//녹스 연결

	//ADB 설정
	FILE* fpipe = popen("adb\\adb devices", "r");
	if (fpipe == NULL)
		std::cout << "\nadb is not available.\n";

	char adb_result_str[1024] = {};
	std::string adb_result;

	fread(adb_result_str, 1024, 1, fpipe);
	adb_result = adb_result_str;

	pclose(fpipe);

	if(adb_result.size() > 27) {
		m_isADB = true;
		std::cout << adb_result;
		std::cout << "[ADB가 사용 가능한 디바이스입니다.]\n";
		m_client = UNKOWN;
	}

	//nox 플레이어 확인 시작
	gameHwnd = FindWindowA("Qt5QWindowIcon", "녹스 플레이어");
	if (gameHwnd != nullptr) {
		m_client = NOX;
		m_isSupportExecOut = false;
		m_currentTouchEvent = 8;
		std::cout << "[NOX 플레이어를 인식하였습니다.]\n\n";
		return;
	}

	//블루스택 확인 시작
	gameHwnd = FindWindowA("BS2CHINAUI", nullptr);
	if (gameHwnd != nullptr) {
		m_client = BLUESTACK;
		m_isSupportExecOut = false;
		m_currentTouchEvent = -1;
		std::cout << "[블루스택을 인식하였습니다.]\n\n";
		return;
	}

}


void GameClientMgr::UpdateScreenImage() {


	//이미지 받아오기
	if (gameHwnd == nullptr && m_client == NONE) {

		if (m_screenImage != nullptr) {
			cvReleaseImage(&m_screenImage);
			SAFE_DELETE(m_screenImage);
		}

		m_screenImage = cvLoadImage("img/temp/temp.png");

		return;
	}

	Mat screenMat;
	
	if(!m_isADB) {
		screenMat = hwnd2Mat(gameHwnd);

		if (screenMat.data == nullptr) {
			if (m_screenImage != nullptr) {
				cvReleaseImage(&m_screenImage);
				SAFE_DELETE(m_screenImage);
			}
			return;
		}
	}else {
		GetHWNDSize();
		EditADBScreen();
		return;
	}


	switch (m_client) {
	case NOX:
		EditNoxScreen(screenMat);
		break;

	case NONE:
		EditNoneScreen(screenMat);
		break;
	case BLUESTACK:
		break;
	default:
		break;
	}

	screenMat.release();

}


Mat GameClientMgr::hwnd2Mat(HWND hwnd) {

	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	srcheight = windowsize.bottom;
	srcwidth = windowsize.right;
	height = m_hwndHeight = windowsize.bottom;  //change this to whatever size you want to resize to
	width = m_hwndWidth = windowsize.right;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	m_updatedTime = timeGetTime();
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

																									   // avoid memory leak
	DeleteObject(hbwindow); DeleteDC(hwindowCompatibleDC); ReleaseDC(hwnd, hwindowDC);

	Mat src_noAlpha;

	cvtColor(src, src_noAlpha, CV_BGRA2BGR);

	src.release();

	return src_noAlpha;

}


void GameClientMgr::GetHWNDSize() {

	if (gameHwnd == nullptr) return;

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(gameHwnd, &windowsize);


	m_hwndHeight = windowsize.bottom;  //change this to whatever size you want to resize to
	m_hwndWidth = windowsize.right;

}


void GameClientMgr::SetMouseMove(int x, int y) {

	if (m_isADB) {

		if(m_currentTouchEvent < 0) {
			return;
		}

		int final_x = ((float)m_ADBWidth / IMG_WIDTH)  *(float)x;
		int final_y = ((float)m_ADBHeight / IMG_HEIGHT)*(float)y;

		std::string event_index = "adb\\adb shell sendevent /dev/input/event" + std::to_string(m_currentTouchEvent) + " ";

		system(std::string(event_index + "3 53 " + std::to_string(final_x)).c_str());
		system(std::string(event_index + "3 54 " + std::to_string(final_y)).c_str());
		system(std::string(event_index + "0 0 0").c_str());
		return;
	}

	if (gameHwnd == nullptr) return;

	int final_x = ((float)m_hwndWidth / IMG_WIDTH)*x;
	int final_y = ((float)m_hwndHeight / IMG_HEIGHT)*y;

	PostMessageA(gameHwnd, WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(final_x, final_y));
}


void GameClientMgr::SetMouseUp(int x, int y) {

	if (m_isADB) {

		//if (m_currentTouchEvent < 0) {
		//	return;
		//}

		//std::string event_index = "adb\\adb shell sendevent /dev/input/event" + std::to_string(m_currentTouchEvent) + " ";

		//system(std::string(event_index + "3 57 -1").c_str());
		//system(std::string(event_index + "1 330 0").c_str());
		//system(std::string(event_index + "0 0 0").c_str());
		return;
	}

	if (gameHwnd == nullptr) return;

	int final_x = ((float)m_hwndWidth / IMG_WIDTH)*x;
	int final_y = ((float)m_hwndHeight / IMG_HEIGHT)*y;


	PostMessageA(gameHwnd, WM_LBUTTONUP, 0, MAKELPARAM(final_x + m_startX, final_y + m_startY));

}


void GameClientMgr::SetMouseDown(int x, int y, int delay) {

	if (m_isADB) {

		//int final_x = ((float)m_ADBWidth / IMG_WIDTH)*(float)(m_isVertical ? IMG_HEIGHT - y : x);
		//int final_y = ((float)m_ADBHeight / IMG_HEIGHT)*(float)(m_isVertical ? x : y);

		//if (m_currentTouchEvent < 0) {
		//	std::string command = "adb\\adb shell input swipe " + std::to_string(final_x) + " " + std::to_string(final_y) + " " + 
		//							std::to_string(final_x) + " " + std::to_string(final_y) + " " +
		//							std::to_string(delay);

		//	system(command.c_str());
		//	return;
		//}
		//std::string event_index = "adb\\adb shell sendevent /dev/input/event" + std::to_string(m_currentTouchEvent) + " ";

		//system(std::string(event_index + "3 57 0").c_str());
		//system(std::string(event_index + "3 53 " + std::to_string(final_x)).c_str());
		//system(std::string(event_index + "3 54 " + std::to_string(final_y)).c_str());
		//system(std::string(event_index + "3 58 21").c_str());
		//system(std::string(event_index + "1 330 1").c_str());
		//system(std::string(event_index + "0 0 0").c_str());
		//return;

		int final_x = ((float)m_ADBWidth / IMG_WIDTH)  *(float)x;
		int final_y = ((float)m_ADBHeight / IMG_HEIGHT)*(float)y;

		std::string command = "adb\\adb shell input swipe " + std::to_string(final_x) + " " + std::to_string(final_y) + " " +
			std::to_string(final_x) + " " + std::to_string(final_y) + " " +
			std::to_string(delay);

		system(command.c_str());
		return;
	}

	if (gameHwnd == nullptr) return;

	int final_x = ((float)m_hwndWidth / IMG_WIDTH)*x;
	int final_y = ((float)m_hwndHeight / IMG_HEIGHT)*y;

	PostMessageA(gameHwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(final_x + m_startX, final_y + m_startY));

	Sleep(delay);
}


void GameClientMgr::SetMouseClick(int x, int y) {

	if (m_isADB) {

		int final_x = ((float)m_ADBWidth / IMG_WIDTH)  *(float)x;
		int final_y = ((float)m_ADBHeight / IMG_HEIGHT)*(float)y;

		std::string command = "adb\\adb shell input tap " + std::to_string(final_x) + " " + std::to_string(final_y);

		system(command.c_str());

		return;
	}

	if (gameHwnd == nullptr) return;

	int final_x = ((float)m_hwndWidth / IMG_WIDTH)  *(float)x;
	int final_y = ((float)m_hwndHeight / IMG_HEIGHT)*(float)y;

	PostMessageA(gameHwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(final_x + m_startX, final_y + m_startY));
	PostMessageA(gameHwnd, WM_LBUTTONUP, 0, MAKELPARAM(final_x + m_startX, final_y + m_startY));



}


void GameClientMgr::SendAdbCommand(std::string command) {
	command = "adb\\" + command;
	system(command.c_str());
}


long GameClientMgr::GetUpdatedTime() const {
	return m_updatedTime;
}


void GameClientMgr::EditNoneScreen(cv::Mat img) {

	IplImage* noneImg = &IplImage(img);
	IplImage* img_temp = (IplImage*)cvClone(noneImg);

	if (m_screenImage != nullptr) {
		cvReleaseImage(&m_screenImage);
		SAFE_DELETE(m_screenImage);
	}

	m_screenImage = img_temp;

}


void GameClientMgr::EditNoxScreen(cv::Mat img) {

	m_startX = 2;
	m_startY = 30;

	IplImage* noxImg = &IplImage(img);
	IplImage* img_temp = (IplImage*)cvClone(noxImg);

	cvSetImageROI(img_temp, cvRect(2, 30, img_temp->width - 2, img_temp->height - 2));

	CvSize size = cvGetSize(img_temp);
	IplImage* _img = cvCreateImage(size, IPL_DEPTH_8U, 3);

	cvCopy(img_temp, _img);
	cvReleaseImage(&img_temp);

	if (m_screenImage != nullptr) {
		cvReleaseImage(&m_screenImage);
		SAFE_DELETE(m_screenImage);
	}

	RESIZE_IMAGE(_img, cvSize(1920, 1080));

	m_screenImage = _img;

}


void GameClientMgr::EditADBScreen() {

	m_updatedTime = timeGetTime();
	FILE* fp = nullptr;

	if(m_isSupportExecOut)
		fp = popen("adb\\adb exec-out screencap", "rb");
	else
		fp = popen("adb\\adb shell screencap", "rb");


	if (fp == nullptr) {
		std::cout << "Failed to load adb.\n";
		return;
	}

	const int buf_size = 1920 * 1080 * 4;

	unsigned char *buf = nullptr;
	unsigned char *buf_img = nullptr;

	buf = static_cast<unsigned char*>(calloc(sizeof(unsigned char), buf_size + 12));
	fread(buf, buf_size + 12, 1, fp);

	pclose(fp);

	//가로세로 설정
	const int width = buf[0] + buf[1] * 0x100 + buf[2] * 0x10000 + buf[3] * 0x1000000;
	const int height = buf[4] + buf[5] * 0x100 + buf[6] * 0x10000 + buf[7] * 0x1000000;

	if (width == 0 || height == 0) {
		free(buf);
		return;
	}



	if(!m_isSupportExecOut) {

		buf_img = static_cast<unsigned char*>(calloc(sizeof(unsigned char), width * height * 4));


		int index = 0;
		int bak_index = -1;
		int bak_i = -1;

		//보정 작업
		for (int i = 12; i < width * height * 4; i++) {

			int alpha_index = i + 4 - ((index + 1) % 4);
			unsigned char alpha = buf[alpha_index];


			if (alpha != 0xff) {

				if (bak_i != -1) {
					i = bak_i;
					index = bak_index;

					bak_i = bak_index = -1;
					continue;
				}

				if (buf[i] == 0x0d) {
					continue;
				}

			} else if (buf[alpha_index + 1] == 0xff || buf[alpha_index - 1] == 0xff) {
				if (buf[i] == 0x0d) {
					bak_i = i;
					bak_index = index;
				}
			} else {
				bak_i = bak_index = -1;
			}

			buf_img[index] = buf[i];
			index++;

		}

	}
	


	//가로세로에 따라 새로 할당

	IplImage* screenImage;

	if(width < height) {
		screenImage = cvCreateImage(cvSize(height, width), IPL_DEPTH_8U, 3);
		m_isVertical = true;
	}else {
		screenImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
		m_isVertical = false;
	}

	//픽셀값 할당 작업
	for (int y = 0; y < height; y++) {

		for (int x = 0; x < width; x++) {

			int src_index = y * width * 3 + x * 3;
			int index = y * width * 4 + x * 4;

			//가로본능에 맞추어 인덱스 값을 수정
			if (width < height) {
				src_index = (x)* height * 3 + y * 3;
				index = y * width * 4 + (width - x) * 4;
			}

			if(m_isSupportExecOut) {
				screenImage->imageData[src_index + 0] = buf[index + 2 + 12];
				screenImage->imageData[src_index + 1] = buf[index + 1 + 12];
				screenImage->imageData[src_index + 2] = buf[index + 0 + 12];
				continue;
			}

			screenImage->imageData[src_index + 0] = buf_img[index + 2];
			screenImage->imageData[src_index + 1] = buf_img[index + 1];
			screenImage->imageData[src_index + 2] = buf_img[index + 0];


		}

	}

	if (m_isVertical) {
		m_ADBWidth = height;
		m_ADBHeight = width;
	}else {
		m_ADBWidth = width;
		m_ADBHeight = height;
	}

	if(width != 1920 && height != 1080)
		RESIZE_IMAGE(screenImage, cvSize(1920, 1080));


	free(buf);
	if(!m_isSupportExecOut)
		free(buf_img);

	if (m_screenImage != nullptr) {
		cvReleaseImage(&m_screenImage);
		SAFE_DELETE(m_screenImage);
	}


	m_screenImage = screenImage;


}
