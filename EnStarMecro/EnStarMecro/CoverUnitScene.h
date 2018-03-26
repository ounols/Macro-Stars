#pragma once
#include "Scene.h"
#include <opencv2/opencv.hpp>

class CoverUnitScene : public Scene {
private:
	enum STATE { NONE, COVER_AP, COVER_LP, USE_ITEM, BUY_ITEM, LIMITED_ITEM };
public:
	CoverUnitScene();
	~CoverUnitScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

private:
	void ReadCoverAP();
	void ReadCoverLP();
	void ReadUseItem();
	void ReadBuyItem();
	void ReadLimitedItem();

	std::string GetNumber(int x, int y, int width, int height);

private:
	STATE m_state = NONE;
	CvPoint m_pos;
};

