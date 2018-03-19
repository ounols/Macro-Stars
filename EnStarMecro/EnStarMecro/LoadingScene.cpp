#include "LoadingScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "SceneMgr.h"
#include "SelectProduceScene.h"
#include "ProducerAI.h"
#include "ResultProduceScene.h"
#include "ProduceTodo.h"


LoadingScene::LoadingScene() {
	
	m_name = "LoadingScene";

	RESMGR->RegisterImage("img/LoadingScene/tips.jpg", "loading_tips");
	RESMGR->RegisterImage("img/LoadingScene/now.jpg", "loading_pop_now", CV_LOAD_IMAGE_GRAYSCALE);
	RESMGR->RegisterImage("img/LoadingScene/loading.jpg", "loading_pop_loading", CV_LOAD_IMAGE_GRAYSCALE);
	RESMGR->RegisterImage("img/LoadingScene/fever_result.jpg", "loading_fever_result");

}


LoadingScene::~LoadingScene() {
	
}


bool LoadingScene::CheckFirst() {

	bool isScene = true;
	m_isIgnorePrevScene = false;
	isDelayLoading = false;
	isFeverResult = false;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 517, 420, 253, 207, 60, 10);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1414, 658, 255, 255, 255, 15);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 960, 490, 255, 255, 255, 3);

	if (isScene) {
		isDelayLoading = true;
		return isScene;
	}

	isScene = RESMGR->CheckRGB(nullptr, 710, 222, 7, 73, 134);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1000, 790, 246, 246, 246);

	if (isScene) isFeverResult = true;

	return isScene;
}


bool LoadingScene::CheckScene() {

	if(isDelayLoading) {
		return CheckDelayLoading();
	}

	if(isFeverResult) {
		return CheckFeverResult();
	}

	auto points = RESMGR->FindImages(nullptr, "loading_tips", 0.99, 1);
	if (points.empty() || points.size() > 1) return false;

	return true;

}


void LoadingScene::ActionDecision() {

	if (isDelayLoading) {
		Sleep(500);
		return;
	}

	if(isFeverResult) {
		GAME->SetMouseClick(950, 830);
		return;
	}

	GAME->SetMouseClick(400, 400);

	if(isScene<SelectProduceScene>(SCENE->GetPrevScene())) {
		Todo* todo = PRODUCER->GetTodo();
		int usedAP = static_cast<SelectProduceScene*>(SCENE->GetPrevScene())->GetUsedAP();



		PRODUCER->SetCurrentAP(PRODUCER->GetAP().current - usedAP);

		//이미 진행이 시작되었음을 알림
		if(todo != nullptr) {
			auto produce_todo = static_cast<ProduceTodo*>(todo);
			
			produce_todo->usedAP = usedAP;

			if(isScene<ResultProduceScene>(todo->targetScene))
				produce_todo->isStarted = true;
		}
	}

}


bool LoadingScene::CheckDelayLoading() {

	m_isIgnorePrevScene = true;

	IplImage* img = (IplImage*)cvClone(GAME->GetScreenImage());

	CUT_IMAGE(img, cvRect(850, 568, 222, 45));
	RESIZE_IMAGE(img, cvSize(img->width * 4, img->height * 4));

	IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

	cvCvtColor(img, dst, CV_RGB2GRAY);
	REALLOC(img, dst);

	dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	int color = 200;
	CvScalar scalar_min = cvScalar(color, color, color);
	CvScalar scalar_max = cvScalar(255, 255, 255);

	cvInRangeS(img, scalar_min, scalar_max, dst);
	REALLOC(img, dst);

	//cvShowImage("sample", img);
	//cvWaitKey();


	auto points = RESMGR->FindImages(img, "loading_pop_now", 0.80, 1, true);
	if (!points.empty()) {
		cvReleaseImage(&img);
		return true;
	}

	points.clear();

	points = RESMGR->FindImages(img, "loading_pop_loading", 0.80, 1, true);
	if (!points.empty()) {
		cvReleaseImage(&img);
		return true;
	}

	m_isIgnorePrevScene = false;

	cvReleaseImage(&img);
	return false;

}


bool LoadingScene::CheckFeverResult() {

	auto points = RESMGR->FindImages(nullptr, "loading_fever_result", 0.98, 1, true);
	if (points.empty()) return false;

	return true;
}
