#pragma once
#include "Scene.h"
#include "ConcertTodo.h"

class ConcertPrepareScene : public Scene {
private:
	enum DECISION { NONE, OK, NO, WAIT, NEED_DIA, CHANGE_MID, LEVEL_UP };
public:
	ConcertPrepareScene();
	~ConcertPrepareScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	bool ReadData() override;
	void ActionDecision() override;

private:
	void ReadIntro();
	void ReadPrepare();
	void ReadLP();

	void SetConcertTodo(ConcertTodo* todo);
	int CheckNeedLP(int totalAudience, ATTRIBUTE_UNIT unit, bool isMidnight);
	DECISION DecisionConcert(ConcertTodo* todo, int needLPCount, bool isTodoSave = true);

	void ActionIntro();
	void ActionPrepare();

	std::string GetNumber(int x, int y, int width, int height);

private:
	bool isIntro = false;
	bool isGiveUp = false;
	bool isNeedInfomation = false;
	bool isQuitConcert = false;
	bool isNeedDiamond = false;
	bool isMidnightAvailable = false;
	bool isMidnightScene = false;

	ConcertTodo::TYPE type = ConcertTodo::NONE;
	DECISION m_decision = NONE;
};

