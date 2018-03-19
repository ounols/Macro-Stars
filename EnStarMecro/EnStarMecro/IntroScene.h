#pragma once
#include "Scene.h"
class IntroScene : public Scene {
public:
	IntroScene();
	~IntroScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	void ActionDecision() override;

private:
	bool isPopUp = false;
};

