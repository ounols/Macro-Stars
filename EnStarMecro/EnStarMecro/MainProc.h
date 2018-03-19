#pragma once
#include <vector>
#include "Scene.h"

class MainProc {
public:
	MainProc();
	~MainProc();

	void Update();


private:
	void MakeBugReport();

public:
	bool b_isQuit = false;
	bool b_isPause = false;

private:
	const int UNKOWN_COUNT_MAX = 30;
	int m_unkownCount = 0;
};

