#pragma once
#include "MacroDef.h"
#include <Windows.h>
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


	cv::Mat hwnd2Mat(HWND hwnd);
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

private:
	void EditNoneScreen(cv::Mat img);
	void EditNoxScreen(cv::Mat img);
	void EditADBScreen();

private:
	CLIENT m_client = NONE;
	bool m_isADB = false;
	bool m_isSupportExecOut = false;
	HWND gameHwnd = nullptr;
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

};

