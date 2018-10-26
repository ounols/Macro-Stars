#pragma once
#include "Scene.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>


class ConcertListScene : public Scene {
private:
	enum STATE { NONE, INTRO, LIST, SUB_STROY, POINT_REWARD, RANK_REWARD };
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
	void AddPointRewardTodo();
private:
	STATE m_state = NONE;
	bool isQuit = false;
	bool isWait = false;
	CvPoint m_point;
};

