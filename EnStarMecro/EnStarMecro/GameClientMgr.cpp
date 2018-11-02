#include "GameClientMgr.h"
#include <opencv2/opencv.hpp>
#include "ResMgr.h"

#include <stdio.h>
#include <errno.h>   /* errno */

using namespace cv;

#ifdef WIN32
#define popen _popen
#define pclose _pclose
#elif __linux__

#endif
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

	SendAdbCommand("adb kill-server");
	SendAdbCommand("adb start-server");
#ifdef WIN32
	SendAdbCommand("adb connect 127.0.0.1:62001");	//�콺 ����
#endif
	//ADB ����
	SetADB();
#ifdef WIN32

	//�ڵ� ����
	SetHWND();
#endif
}


void GameClientMgr::UpdateScreenImage() {

#ifdef WIN32
	//�̹��� �޾ƿ���
	if (gameHwnd == nullptr && m_client == NONE) {

		if (m_screenImage != nullptr) {
			cvReleaseImage(&m_screenImage);
			SAFE_DELETE(m_screenImage);
		}

		m_screenImage = cvLoadImage("img/temp/temp.png");

		SetADB();
		SetHWND();

		return;
	}

	Mat screenMat;
	
	if(!m_isADB) {
		if(m_client != BLUESTACK)
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
		SetADB();
		break;
	default:
		break;
	}

	screenMat.release();
#elif __linux__
	EditADBScreen();
	// cvShowImage("sample", m_screenImage);
	// cvWaitKey();
#endif
}

#ifdef WIN32
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
#endif

void GameClientMgr::GetHWNDSize() {
#ifdef WIN32

	if (gameHwnd == nullptr) return;

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(gameHwnd, &windowsize);


	m_hwndHeight = windowsize.bottom;  //change this to whatever size you want to resize to
	m_hwndWidth = windowsize.right;
#endif
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
#ifdef WIN32

	if (gameHwnd == nullptr) return;

	int final_x = ((float)m_hwndWidth / IMG_WIDTH)*x;
	int final_y = ((float)m_hwndHeight / IMG_HEIGHT)*y;

	PostMessageA(gameHwnd, WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(final_x, final_y));
#endif
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
#ifdef WIN32

	if (gameHwnd == nullptr) return;

	int final_x = ((float)m_hwndWidth / IMG_WIDTH)*x;
	int final_y = ((float)m_hwndHeight / IMG_HEIGHT)*y;


	PostMessageA(gameHwnd, WM_LBUTTONUP, 0, MAKELPARAM(final_x + m_startX, final_y + m_startY));
#endif
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

		std::string command =
#ifdef WIN32
		 "adb\\adb shell input swipe " + 
#elif __linux__
		 "adb shell input swipe " + 
#endif
		 	std::to_string(final_x) + " " + std::to_string(final_y) + " " +
			std::to_string(final_x) + " " + std::to_string(final_y) + " " +
			std::to_string(delay);

		system(command.c_str());
#ifdef __linux__
		// Sleep(500);
#endif
		return;
	}
#ifdef WIN32

	if (gameHwnd == nullptr) return;

	int final_x = ((float)m_hwndWidth / IMG_WIDTH)*x;
	int final_y = ((float)m_hwndHeight / IMG_HEIGHT)*y;

	PostMessageA(gameHwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(final_x + m_startX, final_y + m_startY));

	Sleep(delay);
#endif
}


void GameClientMgr::SetMouseClick(int x, int y) {

	if (m_isADB) {

		int final_x = ((float)m_ADBWidth / IMG_WIDTH)  *(float)x;
		int final_y = ((float)m_ADBHeight / IMG_HEIGHT)*(float)y;

		std::string command =
#ifdef WIN32		
		"adb\\adb shell input tap "
#elif __linux__
		"sudo adb shell input tap "
#endif
		 + std::to_string(final_x) + " " + std::to_string(final_y);

		system(command.c_str());
#ifdef __linux__
		Sleep(100);
#endif
		return;
	}
#ifdef WIN32
	if (gameHwnd == nullptr) return;

	int final_x = ((float)m_hwndWidth / IMG_WIDTH)  *(float)x;
	int final_y = ((float)m_hwndHeight / IMG_HEIGHT)*(float)y;

	PostMessageA(gameHwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(final_x + m_startX, final_y + m_startY));
	PostMessageA(gameHwnd, WM_LBUTTONUP, 0, MAKELPARAM(final_x + m_startX, final_y + m_startY));


#endif
}


void GameClientMgr::SendAdbCommand(std::string command) {
#ifdef WIN32
	command = "adb\\" + command;
#endif
	system(command.c_str());
}


long GameClientMgr::GetUpdatedTime() const {
	return m_updatedTime;
}


bool GameClientMgr::GetIsGunstars() const {
	return isGunstars;
}


void GameClientMgr::SetIsGunstars(bool is_gunstars) {
	isGunstars = is_gunstars;
}


void GameClientMgr::SetHttp(bool enable) {
	isHttp = enable;
}


bool GameClientMgr::GetIsAutoReboot() const {
	return isAutoReboot;
}


void GameClientMgr::SetIsAutoReboot(bool is_auto_reboot) {
	isAutoReboot = is_auto_reboot;
}


bool GameClientMgr::GetIsVpn() const {
	return isVPN;
}


void GameClientMgr::SetIsVpn(bool is_vpn) {
	isVPN = is_vpn;
}


std::string GameClientMgr::GetGunStarsPath() const {
	return GunStarsPath;
}


void GameClientMgr::SetGunStarsPath(const std::string& gun_stars_path) {
	GunStarsPath = gun_stars_path;
}


void GameClientMgr::EditNoneScreen(cv::Mat img) {
#ifdef WIN32
	IplImage* noneImg = &IplImage(img);
	IplImage* img_temp = (IplImage*)cvClone(noneImg);

	if (m_screenImage != nullptr) {
		cvReleaseImage(&m_screenImage);
		SAFE_DELETE(m_screenImage);
	}

	m_screenImage = img_temp;
#endif
}


void GameClientMgr::EditNoxScreen(cv::Mat img) {
#ifdef WIN32
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
#endif
}


void GameClientMgr::EditADBScreen() {

	m_updatedTime = timeGetTime();
	FILE* fp = nullptr;

	if(m_isSupportExecOut)
#ifdef WIN32
		fp = popen("adb\\adb exec-out screencap", "rb");
#elif __linux
		fp = popen("adb exec-out screencap", "r");
#endif
	else
#ifdef WIN32
		fp = popen("adb\\adb shell screencap", "rb");
#elif __linux
		fp = popen("adb shell screencap", "r");
#endif


	if (fp == nullptr) {
		std::cout << "Failed to load adb.(screencap)\n";
		return;
	}

	const int buf_size = 1920 * 1080 * 4;

	unsigned char *buf = nullptr;
	unsigned char *buf_img = nullptr;

	buf = static_cast<unsigned char*>(calloc(sizeof(unsigned char), buf_size + 12));
	fread(buf, buf_size + 12, 1, fp);

	pclose(fp);

	//���μ��� ����
	const int width = buf[0] + buf[1] * 0x100 + buf[2] * 0x10000 + buf[3] * 0x1000000;
	const int height = buf[4] + buf[5] * 0x100 + buf[6] * 0x10000 + buf[7] * 0x1000000;

	if (width == 0 || height == 0) {
		free(buf);
		SetADB();
		return;
	}



	if(!m_isSupportExecOut) {

		buf_img = static_cast<unsigned char*>(calloc(sizeof(unsigned char), width * height * 4));


		int index = 0;
		int bak_index = -1;
		int bak_i = -1;

		//���� �۾�
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
	


	//���μ��ο� ���� ���� �Ҵ�

	IplImage* screenImage;

	if(width < height) {
		screenImage = cvCreateImage(cvSize(height, width), IPL_DEPTH_8U, 3);
		m_isVertical = true;
	}else {
		screenImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
		m_isVertical = false;
	}

	//�ȼ��� �Ҵ� �۾�
	for (int y = 0; y < height; y++) {

		for (int x = 0; x < width; x++) {

			int src_index = y * width * 3 + x * 3;
			int index = y * width * 4 + x * 4;

			//���κ��ɿ� ���߾� �ε��� ���� ����
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

	if (width != 1920 && height != 1080) {
		if(isHttp) {
			int param[3] = { CV_IMWRITE_JPEG_QUALITY , 10, 0};
			cvSaveImage("C:\\http\\report\\current.jpg", screenImage, param);
		}
		RESIZE_IMAGE(screenImage, cvSize(1920, 1080));
	}


	free(buf);
	if(!m_isSupportExecOut)
		free(buf_img);

	if (m_screenImage != nullptr) {
		cvReleaseImage(&m_screenImage);
		SAFE_DELETE(m_screenImage);
	}


	m_screenImage = screenImage;


}


void GameClientMgr::SetADB() {

	//ADB ����
#ifdef WIN32
	FILE* fpipe = popen("adb\\adb devices", "r");
#elif __linux__
	FILE* fpipe = popen("adb devices", "r");
#endif
	
	if (fpipe == NULL)
		std::cout << "\nadb is not available.(Set ADB)\n";

	char adb_result_str[1024] = {};
	std::string adb_result;

	fread(adb_result_str, 1024, 1, fpipe);
	adb_result = adb_result_str;

	pclose(fpipe);

	if (adb_result.size() > 27) {
		m_isADB = true;
		std::cout << adb_result;
		std::cout << "[ADB�� ��� ������ ����̽��Դϴ�.]\n";
		m_client = UNKOWN;
	} else {
#ifdef __linux__
		std::cout << "Emulator is not working...\n";
		system("shutdown -r now");
		return;
#endif
	}

}


void GameClientMgr::SetHWND() {
#ifdef WIN32
	//nox �÷��̾� Ȯ�� ����
	gameHwnd = FindWindowA("Qt5QWindowIcon", "�콺 �÷��̾�");
	if (gameHwnd != nullptr) {
		m_client = NOX;
		m_isSupportExecOut = false;
		m_currentTouchEvent = 8;
		std::cout << "[NOX �÷��̾ �ν��Ͽ����ϴ�.]\n\n";
		return;
	}

	//��罺�� Ȯ�� ����
	gameHwnd = FindWindowA("BS2CHINAUI", nullptr);
	if (gameHwnd != nullptr) {
		m_client = BLUESTACK;
		m_isSupportExecOut = false;
		m_currentTouchEvent = -1;
		std::cout << "[��罺���� �ν��Ͽ����ϴ�.]\n\n";
		return;
	}
#endif
}
