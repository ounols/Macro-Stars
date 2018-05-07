#pragma once
#include <vector>
#include "Scene.h"

class MainProc {
private:
	enum ACTIVITY { UNKOWN, ENSTARS, MARKET, EASYVPN, OPENVPN, ULTRA, HOLA };
public:
	MainProc();
	~MainProc();

	void Update();

	void SetGunStars(bool enable);
	void SetMarket(bool enable);

private:
	void MakeBugReport();

	void GetCurrentActivity();

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
};

