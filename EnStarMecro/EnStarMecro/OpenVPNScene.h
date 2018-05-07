#pragma once
#include "Scene.h"
#include <opencv2/opencv.hpp>

class OpenVPNScene : public Scene {
private:
	enum STATE { IMPORT, OVPN, CAUTION };
public:
	OpenVPNScene();
	~OpenVPNScene();

	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

	void Reset();
private:
	int m_unkownCount = 0;

	bool isPopUp = false;
	CvPoint m_point;
	STATE m_state;

	bool isCleared = false;
};

