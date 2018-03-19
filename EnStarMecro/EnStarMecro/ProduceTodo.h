#pragma once
#include "Todo.h"
#include "ProducerAI.h"
#include "SceneMgr.h"
#include "ResultProduceScene.h"

class ProduceTodo : public Todo {

public:

	enum PRODUCE{ DAILY, LIMIT, TEST };

	PRODUCE type = DAILY;

	bool isStarted = false;
	int usedAP = 0;


	bool isAvailable() override {


		//�̹� ���� ���� ���εེ�� �״�� ������ ����
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

	}


	~ProduceTodo() override {
	}
};
