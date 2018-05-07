#pragma once
#include "Scene.h"
class PlayStoreScene : public Scene {
public:
	PlayStoreScene();
	~PlayStoreScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

private:
	int m_unkownCount = 0;
	bool isClear = false;
	bool isNeedUpdate = false;
};

