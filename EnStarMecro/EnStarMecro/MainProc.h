#pragma once
#include <vector>
#include "Scene.h"


#ifdef __linux__
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>
#endif

class MainProc {
private:
	enum ACTIVITY { UNKOWN, ENSTARS, MARKET, EASYVPN, OPENVPN, ULTRA, HOLA };
public:
	MainProc();
	~MainProc();

	void Update();

	void SetGunStars(bool enable);
	void SetMarket(bool enable);
	void SetReport(bool enable);

private:
	void MakeBugReport();
	void MakeCurrentStateReport();
	void GetRemotedInfo();

	void GetCurrentActivity();
#ifdef __linux__
	bool LinuxKeyEvent();

#endif
public:
	bool b_isQuit = false;
	bool b_isPause = false;

private:
	const int UNKOWN_COUNT_MAX = 30;
	int m_unkownCount = 0;
	int m_unkownAccrue = 0;
	ACTIVITY m_currentActivity = UNKOWN;


	bool isLoaded = false;
	bool isUsedVPN = false;
	int loadingCount = 0;

	bool isMarket = false;
	bool isGunStars = false;
	bool isReport = false;
	int remoteDate = 0;

#ifdef __linux__
	struct input_event ev;
	int fd = -1;
#endif
};

