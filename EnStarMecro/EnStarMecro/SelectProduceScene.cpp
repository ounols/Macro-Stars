#include "SelectProduceScene.h"
#include "ResMgr.h"
#include "ProducerAI.h"
#include "ResultProduceScene.h"
#include "GameClientMgr.h"
#include "ProduceTodo.h"
#include "MainScene.h"


SelectProduceScene::SelectProduceScene() {
	m_name = "SelectProduceScene";

	RESMGR->RegisterImage("img/SelectProduceScene/ap_icon.jpg", "select_produce_ap");
	RESMGR->RegisterImage("img/SelectProduceScene/clear.jpg", "select_produce_clear");
	RESMGR->RegisterImage("img/SelectProduceScene/main_story_open.jpg", "select_main_story_open");

}


SelectProduceScene::~SelectProduceScene() {}


bool SelectProduceScene::CheckFirst() {

	bool isScene = true;
	m_state = NONE;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 31, 65, 255, 197, 0, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1028, 82, 2, 82, 134, 5);

	if (isScene) return isScene;

	isScene = CheckOpenMainStory();

	return isScene;

}


bool SelectProduceScene::CheckScene() {

	if(m_state == OpenMainStoryPop) {
		auto points = RESMGR->FindImages(nullptr, "select_main_story_open", 0.98, 1, true, cvRect(538, 57, 886, 412));
		if (points.empty() || points.size() > 1) return false;

		return true;

	}

	auto points = RESMGR->FindImages(nullptr, "select_produce_ap", 0.94, 1, true, cvRect(720, 40, 110, 100));
	if (points.empty() || points.size() > 1) return false;

	bool isScene = false;

	isScene = CheckSelectChapter() || isScene;
	isScene = CheckSelectProduce() || isScene;
	isScene = CheckProduceDetail() || isScene;

	return isScene;

}


void SelectProduceScene::ActionDecision() {

	if(m_state == OpenMainStoryPop) {
		ActionOpenMainStory();
		return;
	}

	Todo* todo = PRODUCER->GetTodo();

	if (todo == nullptr || todo->targetScene == nullptr) {
		GAME->SetMouseClick(1839, 1000);
		SCENE->LockScene();
		return;
	}

	if(isScene<ResultProduceScene>(todo->targetScene)) {
		switch (m_state) {


		case SelectChapter:
			ActionSelectChapter();
			break;
		case SelectProduce:
			ActionSelectProduce();
			break;
		case ProduceDetail:
			ActionProduceDetail();
			break;
		default: break;

		}

	}
	else {
		GAME->SetMouseClick(1839, 1000);
		SCENE->LockScene();
	}


}


int SelectProduceScene::GetUsedAP() {
	int value = m_usedAP;

	m_usedAP = 0;

	return value;
}


bool SelectProduceScene::CheckSelectChapter() {


	bool isScene = true;

	//isScene = isScene && RESMGR->CheckRGB(nullptr, 22, 44, 255, 196, 0, 5);
	//isScene = isScene && RESMGR->CheckRGB(nullptr, 685, 50, 2, 82, 134);

	m_state = SelectChapter;

	return isScene;
}


bool SelectProduceScene::CheckSelectProduce() {

	bool isScene = true;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 1030, 210, 7, 72, 134);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1566, 210, 6, 58, 110);

	if (isScene) m_state = SelectProduce;

	return isScene;

}


bool SelectProduceScene::CheckProduceDetail() {

	bool isScene = true;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 332, 209, 7, 72, 134);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1566, 210, 214, 163, 60);

	if (isScene) m_state = ProduceDetail;

	return isScene;

}


bool SelectProduceScene::CheckOpenMainStory() {

	bool isScene = true;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 900, 850, 246, 246, 246);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1000, 850, 228, 181, 43, 5);

	if (isScene) m_state = OpenMainStoryPop;

	return isScene;
}


void SelectProduceScene::ActionSelectChapter() {
		
	ProduceTodo* produce_todo = static_cast<ProduceTodo*>(PRODUCER->GetTodo());

	switch (produce_todo->type) {

		case ProduceTodo::DAILY:
			GAME->SetMouseClick(130, 513);
		break;
		case ProduceTodo::LIMIT: 
			GAME->SetMouseClick(130, 663);
		break;
		case ProduceTodo::TEST: 
			GAME->SetMouseClick(130, 364);
		break;
		default: break;
	}

	GAME->SetMouseClick(980, 592);

	Sleep(1000);

}


void SelectProduceScene::ActionSelectProduce() {

	//스크롤 내림
	GAME->SetMouseClick(1683, 865);

	//Sleep(1000);
	//GAME->UpdateScreenImage();

	auto points = RESMGR->FindImages(nullptr, "select_produce_clear", 0.969, 4, true, cvRect(900, 247, 190, 655));
	if (points.empty()) {
		std::cout << "Cannot Start Completed Produce.\n";
		GAME->SetMouseClick(1839, 1000);
		return;
	}

	int pos_y[3] = {0};
	int max = 0;

	for(auto point : points) {

		point.x += 900;
		point.y += 247;
		
		int index = (point.y - 249) / 204;

		if (index < 3) {
			pos_y[index] = point.y;

			if (index > max)	max = index;
		}

	}

	ProduceTodo* produce_todo = static_cast<ProduceTodo*>(PRODUCER->GetTodo());
	int currentAP = PRODUCER->GetAP().current;

	switch (produce_todo->type) {

	case ProduceTodo::DAILY:

		if (produce_todo->isForLevelUp) {
			GAME->SetMouseClick(1300, pos_y[max] + 110);
			m_usedAP = 60;
			break;
		}

		/*if(currentAP > 60) {
			GAME->SetMouseClick(1300, pos_y[max] + 110);
			m_usedAP = 60;
		} else */{
			GAME->SetMouseClick(1300, pos_y[0] + 110);
			m_usedAP = 30;
		}


		break;
	case ProduceTodo::LIMIT:
			if (max > 0) {
				GAME->SetMouseClick(1300, pos_y[1] + 110);
				m_usedAP = 8;
			} else {
				GAME->SetMouseClick(1683, 350);
				Sleep(1000);
				GAME->SetMouseClick(1300, pos_y[0] + 110);
				m_usedAP = 8;

			}
		break;
	case ProduceTodo::TEST:
		GAME->SetMouseClick(1300, pos_y[max] + 110);
		break;
	default: break;
	}


}


void SelectProduceScene::ActionProduceDetail() {

	GAME->SetMouseClick(1388, 836);


}


void SelectProduceScene::ActionOpenMainStory() {

	GAME->SetMouseClick(762, 886);

}
