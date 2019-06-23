#pragma once
#include "Scene.h"
class LoadingScene : public Scene {
public:
	LoadingScene();
	~LoadingScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	void ActionDecision() override;

private:
	bool CheckDelayLoading();
	bool CheckFeverResult();

private:
	bool isDelayLoading = false;
	bool isFeverResult = false;
	bool isQuit = false;
};

