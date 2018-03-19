#pragma once
#include "Todo.h"
#include "Unit.h"
#include "ProducerAI.h"
#include <windows.h>
#include "SceneMgr.h"
#include "ConcertResultScene.h"


class ConcertTodo : public Todo {

public:
	enum TYPE { NODATA, NONE, BIG, GUERRILLA };

	//이 콘서트를 진행할 때 필요한 LP
	int needLPCount = -1;

	//이 콘서트로 사용한 LP
	int usedLP = 0;

	//이 콘서트를 진행하기 위해 필요한 최선의 캐시템으로 얻을 수 있는 LP
	int paidLPCount = 0;

	//이 콘서트에서 달성해야할 관객 수
	int totalAudience = -1;

	//콘서트 타입
	TYPE type = NODATA;

	//속성
	ATTRIBUTE_UNIT attribute = UNKOWN;

	//최종적으로 끝나는 시간
	long achieveTime = 0;

	//기다릴 필요가 있는지의 여부
	bool isWait = false;

	//기다리라는 호출을 받은 당시의 LP
	int waitLP = -1;

	//포기 유무
	bool isGiveUp = false;

	//심야 콘서트 유무
	bool isMidnight = false;

	bool isAvailable() override {

		//콘서트에 대한 아무런 정보가 없기에 할일 수락
		if (needLPCount == -1) return true;

		if (isWait) return false;

		return true;

	}

	void Update() override {

		if (targetScene == nullptr) targetScene = SCENE->GetScene<ConcertResultScene>();

		if(achieveTime - timeGetTime() <= 0 || needLPCount == 0) {
			PRODUCER->RemoveTodo(this);
			return;
		}

		if(isWait && PRODUCER->GetLP().current != waitLP) {
			isWait = false;
		}

	}

	void SetWait() {
		isWait = true;
		waitLP = PRODUCER->GetLP().current;
	}


	~ConcertTodo() override {}
};
