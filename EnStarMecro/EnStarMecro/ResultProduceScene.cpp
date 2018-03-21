#include "ResultProduceScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "Todo.h"
#include "ProducerAI.h"
#include "ProduceTodo.h"
#include "MainScene.h"


ResultProduceScene::ResultProduceScene() {

	m_name = "ResultProduceScene";


	RESMGR->RegisterImage("img/ResultProduceScene/form_side_01.jpg", "result_produce_form_side_01");
	RESMGR->RegisterImage("img/ResultProduceScene/level_up_ok.jpg", "result_produce_ok");
	RESMGR->RegisterImage("img/ResultProduceScene/convert_to_point_arrow.jpg", "result_produce_convert_to_point_arrow");

}


ResultProduceScene::~ResultProduceScene() {
	
}


bool ResultProduceScene::CheckFirst() {

	bool isScene = true;
	isPopUp = false;


	isScene = isScene && (RESMGR->CheckRGB(nullptr, 450, 213, 7, 73, 134, 3)
		|| RESMGR->CheckRGB(nullptr, 450, 213, 221, 178, 74, 3));
	isScene = isScene && (RESMGR->CheckRGB(nullptr, 1474, 210, 6, 57, 110, 3)
		|| RESMGR->CheckRGB(nullptr, 1474, 210, 215, 163, 61, 3));

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 450, 213, 1, 7, 13);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1474, 210, 1, 6, 11);


	if (isScene) isPopUp = true;

	return isScene;

}


bool ResultProduceScene::CheckScene() {

	std::vector<CvPoint> points;


	if(isPopUp) {
		points = RESMGR->FindImages(nullptr, "result_produce_ok", 0.99, 1, true ,cvRect(815, 850, 300, 100));
	} else {
		points = RESMGR->FindImages(nullptr, "result_produce_form_side_01", 0.99, 1, true, cvRect(532, 30, 122, 132));
	}

	if (points.empty() || points.size() > 1) return false;

	return true;
}


bool ResultProduceScene::ReadData() {

	if (isPopUp) {
		auto todo = PRODUCER->GetTodo<ProduceTodo>();
		if(todo != nullptr) {
			PRODUCER->RemoveTodo(todo);
		}
		return true;
	}

	auto points = RESMGR->FindImages(nullptr, "result_produce_convert_to_point_arrow", 0.99, 1, true, cvRect(1003, 770, 130, 100));
	if (points.empty()) return true;


	std::string exp_str = GetNumber(1018, 449, 122, 37);
	int exp_slash = -1;
	if ((exp_slash = exp_str.find("/")) != std::string::npos) {

		int exp_max = std::stoi(exp_str.substr(exp_slash + 1));
		int exp_current = std::stoi(exp_str.substr(0, exp_slash));

		PRODUCER->SetRank(-1, exp_current, exp_max);
	}

	return true;
}


void ResultProduceScene::ActionDecision() {

	GAME->SetMouseClick(963, 873);

	Todo* todo = PRODUCER->GetTodo();

	if(todo == nullptr || todo->targetScene == nullptr) {
		return;
	}

	if(isScene<ResultProduceScene>(todo->targetScene)) {
		if (!static_cast<ProduceTodo*>(todo)->isForLevelUp) {
			PRODUCER->RemoveTodo();
		}else {
			Todo* todo_mainScene = new Todo();
			todo_mainScene->targetScene = SCENE->GetScene<MainScene>();
			PRODUCER->AddTodo(todo_mainScene);
		}
	}

}


std::string ResultProduceScene::GetNumber(int x, int y, int width, int height) {

	IplImage* img = (IplImage*)cvClone(GAME->GetScreenImage());

	CUT_IMAGE(img, cvRect(x, y, width, height));

	RESIZE_IMAGE(img, cvSize(img->width * 3, img->height * 3));

	//cvShowImage("이진", img);
	//cvWaitKey();

	{
		IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		cvCvtColor(img, dst, CV_RGB2GRAY);
		REALLOC(img, dst);

		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		CvScalar scalar_min = cvScalar(0, 0, 0);
		CvScalar scalar_max = cvScalar(100, 100, 100);

		cvInRangeS(img, scalar_min, scalar_max, dst);
		REALLOC(img, dst);

	}

	//cvShowImage("이진", img);
	//cvWaitKey();

	std::string str = RESMGR->Image2String(img, 30);
	//cvWaitKey();

	cvReleaseImage(&img);

	return str;

}
