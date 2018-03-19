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
	bool isPopUp = false;
};

