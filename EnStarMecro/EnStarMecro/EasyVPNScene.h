#pragma once
#include "Scene.h"
#include <opencv2/opencv.hpp>

class EasyVPNScene : public Scene {
public:
	EasyVPNScene();
	~EasyVPNScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

	void Reset();

private:
	int m_unkownCount = 0;
	bool isRefeshed = false;
	bool isCleared = false;
	CvPoint m_point;
};

