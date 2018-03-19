#pragma once
#include "Scene.h"
class ConcertResultScene : public Scene {
public:
	ConcertResultScene();
	~ConcertResultScene();

	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

};

