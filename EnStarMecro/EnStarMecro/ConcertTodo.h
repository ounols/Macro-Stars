#pragma once
#include "Todo.h"
#include "Unit.h"
#include "ProducerAI.h"
#ifdef WIN32
#include <windows.h>
#elif __linux__
#include "windows_for_linux.h"
#endif
#include "SceneMgr.h"
#include "ConcertResultScene.h"


class ConcertTodo : public Todo {

public:
	enum TYPE { NODATA, NONE, BIG, GUERRILLA };

	//�� �ܼ�Ʈ�� ������ �� �ʿ��� LP
	int needLPCount = -1;

	//�� �ܼ�Ʈ�� ����� LP
	int usedLP = 0;

	//�� �ܼ�Ʈ�� �����ϱ� ���� �ʿ��� �ּ��� ĳ�������� ���� �� �ִ� LP
	int paidLPCount = 0;

	//�� �ܼ�Ʈ���� �޼��ؾ��� ���� ��
	int totalAudience = -1;

	//�ܼ�Ʈ Ÿ��
	TYPE type = NODATA;

	//�Ӽ�
	ATTRIBUTE_UNIT attribute = UNKOWN;

	//���������� ������ �ð�
	long achieveTime = 0;

	int limitedTime = -1;

	//��ٸ� �ʿ䰡 �ִ����� ����
	bool isWait = false;

	//��ٸ���� ȣ���� ���� ����� LP
	int waitLP = -1;

	//���� ����
	bool isGiveUp = false;

	//�ɾ� �ܼ�Ʈ ����
	bool isMidnight = false;

	bool isAvailable() override {

		//�ܼ�Ʈ�� ���� �ƹ��� ������ ���⿡ ���� ����
		if (needLPCount == -1) return true;

		if (isWait) return false;

		return true;

	}

	void Update() override {

		if (targetScene == nullptr) targetScene = SCENE->GetScene<ConcertResultScene>();

		long concertTime = achieveTime - timeGetTime();
		if(concertTime <= 0 || needLPCount == 0) {
			PRODUCER->RemoveTodo(this);
			return;
		}

		if(isWait && PRODUCER->GetLP().current != waitLP) {
			isWait = false;
		}

		if(limitedTime > 0 && limitedTime >= ProducerAI::Millisecond2Min(concertTime)){
			isWait = false;
		}

	}

	void SetWait() {
		isWait = true;
		waitLP = PRODUCER->GetLP().current;
	}


	~ConcertTodo() override {}
};
