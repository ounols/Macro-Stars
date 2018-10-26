#pragma once
#include "MacroDef.h"
#ifdef WIN32
#include <windows.h>
#elif __linux__
#include "windows_for_linux.h"
#endif
#include <opencv2/core/core.hpp>
#include <thread>
#include <mutex>

#define GAME GameClientMgr::getInstance()




class GameClientMgr {
public:
	enum CLIENT { NONE, NOX, BLUESTACK, OTHER, UNKOWN };
public:
	DECLARE_SINGLETONE(GameClientMgr);
	~GameClientMgr();

	void GetGameHWND();
	void UpdateScreenImage();

	IplImage* GetScreenImage() const {
		return m_screenImage;
	}

#ifdef WIN32
	cv::Mat hwnd2Mat(HWND hwnd);
#endif
	void GetHWNDSize();

	bool IsEmpty() const {
		return m_isEmpty;
	}

	int GetWidth() const {
		return m_screenImage->width;
	}

	int GetHeight() const {
		return m_screenImage->height;
	}

	void SetMouseMove(int x, int y);
	void SetMouseUp(int x, int y);
	void SetMouseDown(int x, int y, int delay = 0);
	void SetMouseClick(int x, int y);
	void SendAdbCommand(std::string command);

	long GetUpdatedTime() const;


	bool GetIsGunstars() const;
	void SetIsGunstars(bool is_gunstars);
	void SetHttp(bool enable);


	bool GetIsAutoReboot() const;
	void SetIsAutoReboot(bool is_auto_reboot);
	bool GetIsVpn() const;
	void SetIsVpn(bool is_vpn);

	std::string GetGunStarsPath() const;
	void SetGunStarsPath(const std::string& gun_stars_path);
private:
	void EditNoneScreen(cv::Mat img);
	void EditNoxScreen(cv::Mat img);
	void EditADBScreen();

	void SetADB();
	void SetHWND();

public:
	bool isQuit = false;

private:
	CLIENT m_client = NONE;
	bool m_isADB = false;
	bool m_isSupportExecOut = false;
#ifdef WIN32
	HWND gameHwnd = nullptr;
#endif
	int m_hwndWidth;
	int m_hwndHeight;
	int m_ADBWidth = -1;
	int m_ADBHeight = -1;
	bool m_isVertical = false;
	int m_currentTouchEvent = -1;

	int m_startX = 0;
	int m_startY = 0;

	IplImage* m_screenImage = nullptr;

	bool m_isEmpty = true;

	long m_updatedTime = 0;

	//�ӽ�
	bool isGunstars = false;
	std::string GunStarsPath = "";
	bool isVPN = false;
	bool isAutoReboot = false;
	bool isHttp = false;

};

