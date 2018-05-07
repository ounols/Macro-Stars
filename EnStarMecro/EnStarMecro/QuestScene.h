#pragma once
#include "Scene.h"
#include <opencv2/opencv.hpp>

class QuestScene : public Scene {
public:
	QuestScene();
	~QuestScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

private:
	bool isQuit = false;
	CvPoint m_point;
};

