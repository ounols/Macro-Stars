#pragma once
#include "Scene.h"
class FeverScene : public Scene {
public:
	FeverScene();
	~FeverScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;


private:
	bool isSkiped;
	bool isSkipPopUp;
	bool isCleared;

	int pos_x;
	int pos_y;
};

