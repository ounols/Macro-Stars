#include "DIFScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "ConcertTodo.h"
#include "ConcertPrepareScene.h"
#include "SceneMgr.h"


DIFScene::DIFScene() {

	m_name = "DIFScene";

	
	RESMGR->RegisterImage("img/DIFScene/vs.jpg", "DIF_vs");
	//RESMGR->RegisterImage("img/DIFScene/start_battle.jpg", "DIF_start_battle");


}


DIFScene::~DIFScene() {}


bool DIFScene::CheckFirst() {

	if (CheckIntro()) return false;
	if (CheckBattle()) return false;
	if (CheckResult()) return false;
	
	
	return CheckPrepare();

}


bool DIFScene::CheckScene() {

	auto points = RESMGR->FindImages(nullptr, "DIF_vs", 0.99, 1);
	if (points.empty() || points.size() > 1) return false;

	return true;

}


void DIFScene::ActionDecision() {

	GAME->SetMouseClick(960, 860);

}


bool DIFScene::CheckIntro() {

	bool isIntro = RESMGR->CheckRGB(nullptr, 90, 220, 24, 166, 216)
			&& RESMGR->CheckRGB(nullptr, 320, 735, 2, 29, 48);

	if(isIntro) {
		GAME->SetMouseClick(300, 300);
		Sleep(1000);
	}

	return isIntro;
}


bool DIFScene::CheckPrepare() {

	bool isScene = true;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 68, 47, 44, 169, 225, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 73, 1012, 224, 170, 35, 5);

	return isScene;

}


bool DIFScene::CheckBattle() {

	bool isScene = RESMGR->CheckRGB(nullptr, 1830, 70, 250, 250, 250, 5)
		&& RESMGR->CheckRGB(nullptr, 1801, 77, 23, 173, 212, 5);

	if (isScene) {
		GAME->SetMouseClick(1800, 110);
		Sleep(1000);
		return isScene;
	}

	isScene = RESMGR->CheckRGB(nullptr, 1805, 84, 250, 250, 250, 5)
		&& RESMGR->CheckRGB(nullptr, 1780, 95, 23, 173, 212, 5);

	if(isScene) {

		GAME->SetMouseClick(1800, 110);

		if(this->isScene<ConcertPrepareScene>(SCENE->GetPrevScene())) {
			ConcertTodo* todo = PRODUCER->GetFirstTodo<ConcertTodo>();
			if (todo != nullptr) {
				PRODUCER->SetCurrentLP(PRODUCER->GetLP().current - todo->usedLP);
				PRODUCER->RemoveTodo(todo);
			}


		}

		SCENE->SetCurrentScene(this);
		SCENE->SetPrevScene(this);

		Sleep(5000);
		GAME->SetMouseClick(1800, 110);
	}

	return isScene;
}


bool DIFScene::CheckResult() {

	bool isScene = RESMGR->CheckRGB(nullptr, 1062, 512, 71, 186, 251, 5)
		&& RESMGR->CheckRGB(nullptr, 1057, 603, 252, 122, 98, 5);

	if (isScene) {
		GAME->SetMouseClick(300, 300);
		Sleep(1000);
	}

	return isScene;

}
