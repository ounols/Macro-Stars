#pragma once
#include "Scene.h"
class ConcertListScene : public Scene {
public:
	ConcertListScene();
	~ConcertListScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;


private:
	static std::string GetNumber(int x, int y, int width, int height);

	void AddConcertTodo();
private:
	bool isIntro = false;
	bool isQuit = false;
	bool isWait = false;
};

