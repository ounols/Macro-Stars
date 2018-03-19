#pragma once
#include "Scene.h"
#include "Unit.h"

class ConcertUnitScene : public Scene {
public:
	ConcertUnitScene();
	~ConcertUnitScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

private:
	void SetCurrentAttribute(int x, int y);
	static std::string GetNumber(int x, int y, int width, int height);

private:
	ATTRIBUTE_UNIT current_page = UNKOWN;
	bool isReadyToRead = false;
};

