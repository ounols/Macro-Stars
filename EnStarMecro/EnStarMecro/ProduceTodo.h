#pragma once
#include "Todo.h"
#include "ProducerAI.h"
#include "SceneMgr.h"
#include "ResultProduceScene.h"
#include "CoverTodo.h"

class ProduceTodo : public Todo {

public:

	enum PRODUCE{ DAILY, LIMIT, TEST };

	PRODUCE type = DAILY;

	bool isStarted = false;
	int usedAP = 0;
	bool isForLevelUp = false;


	bool isAvailable() override {

		if (isForLevelUp) {
			return true;
		}

		//이미 진행 중인 프로듀스면 그대로 진행이 가능
		if (isStarted) return true;
		
		int currentAP = PRODUCER->GetAP().current;
		ProducerAI::STATUS status = PRODUCER->GetStatus();

		switch (type) {
		case DAILY:
			if (currentAP + usedAP >= 30) return true;
			break;

		case LIMIT:
			if (currentAP + usedAP >= 8) return true;
			break;

		case TEST:
			return true;
		}


		return false;
		
	}


	void Update() override {

		if (targetScene == nullptr) targetScene = SCENE->GetScene<ResultProduceScene>();

		if(isForLevelUp) {
			auto todo = PRODUCER->GetTodo<CoverTodo>();
			if (todo == nullptr) {
				todo = new CoverTodo();
				todo->targetScene = SCENE->GetScene<ResultProduceScene>();
				PRODUCER->AddTodo(todo);
			}
		}

	}


	~ProduceTodo() override {
	}
};
