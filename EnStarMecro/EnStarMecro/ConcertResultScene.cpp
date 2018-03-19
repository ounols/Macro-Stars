#include "ConcertResultScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "ProducerAI.h"
#include "ConcertTodo.h"


ConcertResultScene::ConcertResultScene() {
	m_name = "ConcertResultScene";

	RESMGR->RegisterImage("img/ConcertScene/success.jpg", "concert_success");
}


ConcertResultScene::~ConcertResultScene() {}


bool ConcertResultScene::CheckFirst() {
	bool isScene = true;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 31, 65, 255, 197, 0, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1451, 407, 7, 73, 134, 5);

	return isScene;
}


bool ConcertResultScene::CheckScene() {

	auto points = RESMGR->FindImages(nullptr, "concert_success", 0.99, 1, true, cvRect(1498, 147, 404, 190));
	if (points.empty()) return false;

	return true;

}


bool ConcertResultScene::ReadData() {

	auto todo = PRODUCER->GetTodo<ConcertTodo>();

	if(todo == nullptr) {
		return true;
	}

	PRODUCER->SetCurrentLP(PRODUCER->GetLP().current - todo->usedLP);
	PRODUCER->RemoveTodo(todo);
	
	
	return true;
}


void ConcertResultScene::ActionDecision() {

	GAME->SetMouseClick(1480, 930);

}
