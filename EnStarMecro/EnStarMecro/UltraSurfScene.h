#pragma once
#include "Scene.h"
class UltraSurfScene : public Scene {
public:
	UltraSurfScene();
	~UltraSurfScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	void ActionDecision() override;

private:
	bool needClick = false;
	bool isCleared = false;
	bool isCaution = false;
};

