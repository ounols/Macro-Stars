#pragma once
#include "Scene.h"
#include <opencv2/opencv.hpp>

class NomalProduceScene : public Scene {
public:
	NomalProduceScene();
	~NomalProduceScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

private:

	void ReadPopUp();

private:
	bool isSkiped = false;
	bool isPopUpActived = false;
	int m_choose;
	bool isEnterConcert = false;
	bool isWait = false;
	bool isQuit = false;

	IplImage* m_prevImage = nullptr;
	CvPoint m_popPos;
};

