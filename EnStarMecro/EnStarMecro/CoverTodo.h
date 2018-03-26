#pragma once
#include "Todo.h"
#include "SceneMgr.h"
#include "ProducerAI.h"

class CoverTodo : public Todo {
	
public:

	int coveredAP = 0;
	int coveredLP = 0;

	~CoverTodo() override {}

	bool isAvailable() override {
		return false;
	}


	void Update() override {
		
		if (targetScene != nullptr && targetScene == SCENE->GetPrevScene()) {
			PRODUCER->RemoveTodo(this);
			return;
		}

	}

	void UpdateCovered(int count) {
		PRODUCER->SetCurrentAP(PRODUCER->GetAP().current + (coveredAP * count));
		PRODUCER->SetCurrentLP(PRODUCER->GetLP().current + (coveredLP * count));

		coveredAP = coveredLP = 0;
	}

	void Destroy() {
		PRODUCER->RemoveTodo(this);
	}

};
