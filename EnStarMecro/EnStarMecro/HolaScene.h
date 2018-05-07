#pragma once
#include "Scene.h"
#include <opencv2/opencv.hpp>

class HolaScene : public Scene {
public:
	HolaScene();
	~HolaScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

private:
	bool isMain = false;
	CvPoint m_point;
};

