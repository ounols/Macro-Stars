#pragma once
#include "Scene.h"
#include <string>
#include <opencv2/core/cuda.inl.hpp>

class MainScene : public Scene {
public:
	MainScene();
	~MainScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;


private:

	void ReadPopUp();
	void ActionPopUp();

	static std::string GetNumber(int x, int y, int width, int height, bool isDilate, bool isErode, int minScalar = 230, int maxScalar = 255, int scale = 2);
	static std::string GetNumber(int x, int y, int width, int height);

	void AddTodo() const;

private:
	bool isPopUp;
	bool isHide;

	const CvPoint PRODUCE_POS = cvPoint(1200, 700);
	CvPoint m_pos;
};

