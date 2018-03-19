#pragma once
#include "Scene.h"
class KakaoPopUpScene : public Scene {
public:
	KakaoPopUpScene();
	~KakaoPopUpScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

private:
	bool isNoticePopUp = false;
};

