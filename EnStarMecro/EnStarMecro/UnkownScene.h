#pragma once
#include "Scene.h"
class UnkownScene : public Scene {
public:
	UnkownScene();
	~UnkownScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	void ActionDecision() override;
};

