#pragma once
#include "Scene.h"
class TalkProduceScene : public Scene {
public:
	TalkProduceScene();
	~TalkProduceScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	void ActionDecision() override;
};

