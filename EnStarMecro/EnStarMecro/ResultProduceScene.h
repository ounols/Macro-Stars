#pragma once
#include "Scene.h"
class ResultProduceScene : public Scene {
public:
	ResultProduceScene();
	~ResultProduceScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

private:
	std::string GetNumber(int x, int y, int width, int height);

private:
	bool isLevelUp = false;
	bool isPopUp = false;
};

