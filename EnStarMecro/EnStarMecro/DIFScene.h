#pragma once
#include "Scene.h"
class DIFScene : public Scene {
public:
	DIFScene();
	~DIFScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	void ActionDecision() override;

private:
	bool CheckIntro();
	bool CheckPrepare();
	bool CheckBattle();
	bool CheckResult();
};

