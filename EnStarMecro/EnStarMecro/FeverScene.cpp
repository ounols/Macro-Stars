#include "FeverScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"

void Print2(std::vector<CvPoint> points, const char* say, CvScalar color = CV_RGB(255, 0, 0)) {

	IplImage* screen = GAME->GetScreenImage();
	CvFont font;

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 1.0, 0, 2, CV_AA);


	for (auto point : points) {

		cvPutText(screen, say, cvPoint(point.x, point.y - CONVERT_Y(-9, screen)), &font, color);
		cvRectangle(screen, point, cvPoint(point.x + CONVERT_X(30, screen), point.y + 5 + CONVERT_Y(30, screen)), color, 2);

	}

}


FeverScene::FeverScene() {

	m_name = "FeverScene";


	RESMGR->RegisterImage("img/FeverScene/skip.jpg", "skip");
	RESMGR->RegisterImage("img/FeverScene/skip_01.jpg", "fever_skip");
	RESMGR->RegisterImage("img/FeverScene/music_gauge_icon.jpg", "fever_gauge_icon");
	RESMGR->RegisterImage("img/FeverScene/button_01.jpg", "fever_bt_left");

}


FeverScene::~FeverScene() {
	
}


bool FeverScene::CheckFirst() {

	bool isScene = true;
	isSkiped = false;
	isSkipPopUp = false;
	isCleared = false;


	isScene = isScene && RESMGR->CheckRGB(nullptr, 1836, 1020, 34, 153, 187, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 40, 904, 34, 170, 220, 5);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 1850, 970, 12, 58, 71, 5) || RESMGR->CheckRGB(nullptr, 1850, 970, 18, 85, 102, 5);
	isScene = isScene && (RESMGR->CheckRGB(nullptr, 40, 904, 12, 59, 77, 5) || RESMGR->CheckRGB(nullptr, 40, 904, 18, 85, 111, 5));

	if (isScene) isSkiped = true;

	return isScene;
}


bool FeverScene::CheckScene() {

	pos_x = 970;
	pos_y = 909;

	auto points = RESMGR->FindImages(nullptr, "fever_gauge_icon", 0.99, 1, true, cvRect(492, 30, 125, 85));
	if (points.empty() || points.size() > 1) return false;

	return true;
}


bool FeverScene::ReadData() {

	//어떠한 팝업이 존재할 경우
	if(isSkiped) {

		//스킵하시겠습니까? 팝업이 떴을 시
		auto skip_bt = RESMGR->FindImages(nullptr, "fever_skip", 0.99, 1, true, cvRect(974, 648, 390, 135));

		if (!skip_bt.empty()) {
			isSkipPopUp = true;

			CvPoint point = skip_bt[0];

			pos_x = point.x + 974;
			pos_y = point.y + 708;

			//Print2(skip_bt, "Skip", CV_RGB(0, 20, 250));

			return true;

		}

		//아니면 일시정지
		pos_x = 1031;
		pos_y = 696;

		return true;
	}

	//특훈이 모두 진행된 상태인지 확인
	auto left_bt = RESMGR->FindImages(nullptr, "fever_bt_left", 0.99, 1, true, cvRect(200, 830, 450, 250));

	//특훈이 모두 진행된 상태라면
	if(left_bt.empty()) {
		isCleared = true;
		return true;
	}

	//스킵이 불가능한 경우
	if (RESMGR->CheckRGB(nullptr, 1835, 856, 174, 174, 174, 10)) {
		GAME->SetMouseClick(1830, 80);
		pos_x = 1031;
		pos_y = 696;
		return true;
	}

	//아니면 작은 스킵버튼만 계속 누르도록
	pos_x = 1842;
	pos_y = 881;

	Print2(left_bt, "Left_Button", CV_RGB(0, 250, 250));


	return true;
}


void FeverScene::ActionDecision() {

	if(isCleared) {
		GAME->SetMouseDown(300, 300, 2000);
		GAME->SetMouseUp(300, 300);
		return;
	}

	GAME->SetMouseClick(pos_x, pos_y);

}
