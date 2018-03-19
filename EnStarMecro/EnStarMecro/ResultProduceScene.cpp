#include "ResultProduceScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "Todo.h"
#include "ProducerAI.h"


ResultProduceScene::ResultProduceScene() {

	m_name = "ResultProduceScene";


	RESMGR->RegisterImage("img/ResultProduceScene/form_side_01.jpg", "result_produce_form_side_01");
	RESMGR->RegisterImage("img/ResultProduceScene/level_up_ok.jpg", "result_produce_ok");

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


	return true;
}


void ResultProduceScene::ActionDecision() {

	GAME->SetMouseClick(963, 873);
	GAME->SetMouseClick(963, 873);

	Todo* todo = PRODUCER->GetTodo();

	if(todo == nullptr || todo->targetScene == nullptr) {
		return;
	}

	if(isScene<ResultProduceScene>(todo->targetScene)) {
		PRODUCER->RemoveTodo();
	}

}
