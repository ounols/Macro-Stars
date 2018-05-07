#pragma once
#include "Scene.h"
class KakaoLoginScene : public Scene {
public:
	KakaoLoginScene();
	~KakaoLoginScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

private:
	bool isLogout = false;
};

