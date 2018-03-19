#include "ConcertListScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "ConcertTodo.h"


ConcertListScene::ConcertListScene() {
	m_name = "ConcertListScene";


	RESMGR->RegisterImage("img/ConcertScene/enter.jpg", "concert_enter");
	RESMGR->RegisterImage("img/ConcertScene/concert_list_yellow.jpg", "concert_list_yellow");
}


ConcertListScene::~ConcertListScene() {}


bool ConcertListScene::CheckFirst() {

	bool isScene = true;
	isIntro = false;
	isWait = false;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 31, 65, 255, 197, 0, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 433, 958, 228, 180, 42, 5);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 31, 65, 255, 197, 0, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1035, 221, 222, 72, 73, 5);

	if (isScene) isIntro = true;

	return isScene;

}


bool ConcertListScene::CheckScene() {

	if(isIntro) {
		auto points = RESMGR->FindImages(nullptr, "concert_enter", 0.99, 1, true, cvRect(1094, 886, 473, 114));
		if (points.empty() || points.size() > 1) return false;

		Sleep(1000);


		return true;
	}

	auto points = RESMGR->FindImages(nullptr, "concert_list_yellow", 0.99, 1, true, cvRect(210, 932, 259, 87));
	if (points.empty() || points.size() > 1) return false;

	Sleep(1000);


	return true;

}


bool ConcertListScene::ReadData() {

	isQuit = false;

	if(isIntro) {
		return true;
	}

	if (!RESMGR->CheckRGB(nullptr, 1150, 430, 6, 58, 110, 5)) {
		auto _todo = PRODUCER->GetTodo<ConcertTodo>();

		if(_todo != nullptr)
			PRODUCER->RemoveTodo(_todo);

		isQuit = true;
		return true;
	}

	if(PRODUCER->GetTodoSize() == 0) {
		isQuit = true;
		return true;
	}

	ConcertTodo* todo = PRODUCER->GetFirstTodo<ConcertTodo>();

	//새로이 설정
	if(PRODUCER->GetTodo<ConcertTodo>() == nullptr) {
		AddConcertTodo();
		todo = PRODUCER->GetFirstTodo<ConcertTodo>();
	}

	if(todo == nullptr) {
		isQuit = true;
		return true;
	}

	if(todo->totalAudience == -1) {

		std::string totalAudience_str = GetNumber(1245, 305, 116, 33);
		todo->totalAudience = std::stoi(totalAudience_str);

		std::string concert_achieveTime_str = GetNumber(1306, 346, 87, 36);
		int achieve_second = std::stoi(concert_achieveTime_str.substr(concert_achieveTime_str.size() - 2));
		int achieve_minute = std::stoi(concert_achieveTime_str.substr(0, 2));


		if (achieve_second <= 59 && achieve_minute <= 59)
			todo->achieveTime = GAME->GetUpdatedTime() + ProducerAI::GetMillisecond(achieve_minute, achieve_second);
		else
			isWait = true;

	}


	return true;
}


void ConcertListScene::ActionDecision() {


	if(isQuit) {
		GAME->SetMouseClick(80, 70);
		Sleep(2000);
		return;
	}

	if(isIntro) {

		if (PRODUCER->GetFirstTodo<ConcertTodo>() != nullptr) {
			GAME->SetMouseClick(690, 960);
			return;
		}

		GAME->SetMouseClick(80, 70);
		return;
	}

	if (isWait) return;

	if (PRODUCER->GetFirstTodo<ConcertTodo>() != nullptr) {
		GAME->SetMouseClick(1500, 320);
		return;
	}

	GAME->SetMouseClick(80, 70);


}


std::string ConcertListScene::GetNumber(int x, int y, int width, int height) {

	IplImage* img = (IplImage*)cvClone(GAME->GetScreenImage());

	CUT_IMAGE(img, cvRect(x, y, width, height));

	//if (isDilate) {
	//	IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	//	cvDilate(img, dst);

	//	REALLOC(img, dst);
	//}

	//if (isErode) {
	//	IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	//	cvErode(img, dst);

	//	REALLOC(img, dst);
	//}

	RESIZE_IMAGE(img, cvSize(img->width * 4, img->height * 4));

	{
		IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		cvCvtColor(img, dst, CV_RGB2GRAY);
		REALLOC(img, dst);

		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		int color = 130;
		CvScalar scalar_min = cvScalar(0, 0, 0);
		CvScalar scalar_max = cvScalar(color, color, color);

		cvInRangeS(img, scalar_min, scalar_max, dst);
		REALLOC(img, dst);

		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		cvErode(img, dst, nullptr, 1);
		REALLOC(img, dst);

	}
	//cvShowImage("sample", img);
	//cvWaitKey();
	std::string str = RESMGR->Image2String(img);
	//cvWaitKey();

	cvReleaseImage(&img);

	return str;

}


void ConcertListScene::AddConcertTodo() {

	if(!RESMGR->CheckRGB(nullptr, 1150, 430, 6, 58, 110, 5)) {
		return;
	}

	ConcertTodo* _todo = new ConcertTodo();
	PRODUCER->AddTodo(_todo);
	_todo->targetScene = SCENE->GetScene<ConcertResultScene>();
	_todo->todo_str = "concert";

	std::string concert_achieveTime_str = GetNumber(1306, 346, 87, 36);

	if (concert_achieveTime_str.size() >= 4) {

		int achieve_second = std::stoi(concert_achieveTime_str.substr(concert_achieveTime_str.size() - 2));
		int achieve_minute = std::stoi(concert_achieveTime_str.substr(0, 2));

		if (achieve_second <= 59 && achieve_minute <= 59)
			_todo->achieveTime = GAME->GetUpdatedTime() + ProducerAI::GetMillisecond(achieve_minute, achieve_second);
		else
			isWait = true;
	}

}
