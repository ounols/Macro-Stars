#include "ConcertPrepareScene.h"
#include "ResMgr.h"
#include "ConcertTodo.h"
#include "GameClientMgr.h"
#include "ProducerAI.h"
#include "SceneMgr.h"
#include "MainScene.h"


ConcertPrepareScene::ConcertPrepareScene() {
	
	m_name = "ConcertPrepareScene";


	RESMGR->RegisterImage("img/ConcertScene/enter_now.jpg", "concert_enter_now");
	RESMGR->RegisterImage("img/ConcertScene/enter_normal.jpg", "concert_enter_normal");

	RESMGR->RegisterImage("img/ConcertScene/giveup_live.jpg", "concert_giveup_live");
	RESMGR->RegisterImage("img/ConcertScene/scale_big.jpg", "concert_scale_big");
	RESMGR->RegisterImage("img/ConcertScene/scale_guerrilla.jpg", "concert_scale_guerrilla");
	RESMGR->RegisterImage("img/ConcertScene/scale_big_s.jpg", "concert_scale_big_s");
	RESMGR->RegisterImage("img/ConcertScene/scale_guerrilla_s.jpg", "concert_scale_guerrilla_s");

	RESMGR->RegisterImage("img/ConcertScene/set_midnight.jpg", "concert_set_midnight");
	RESMGR->RegisterImage("img/ConcertScene/set_normal.jpg", "concert_set_normal");



}


ConcertPrepareScene::~ConcertPrepareScene() {}


bool ConcertPrepareScene::CheckFirst() {

	bool isScene = true;
	isIntro = true;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 1400, 920, 246, 246, 246, 3);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1200, 70, 0, 0, 0);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 1660, 200, 224, 172, 34, 8);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1300, 957, 203, 42, 60, 5);

	if (isScene) isIntro = false;

	return isScene;
}


bool ConcertPrepareScene::CheckScene() {

	isGiveUp = false;

	if(isIntro) {
		auto points = RESMGR->FindImages(nullptr, "concert_enter_now", 0.99, 1, true, cvRect(1100, 695, 567, 113));
		if (points.empty()) {

			points = RESMGR->FindImages(nullptr, "concert_enter_normal", 0.99, 1, true, cvRect(1100, 695, 567, 113));
			if(points.empty()) {
				return false;
			}

		}

		return true;
	}

	auto points = RESMGR->FindImages(nullptr, "concert_giveup_live", 0.98, 1, true, cvRect(1010, 926, 347, 115));
	if (points.empty()) return false;

	Sleep(1000);

	return true;

}


bool ConcertPrepareScene::ReadData() {

	if(isIntro) {
		ReadIntro();
		return true;
	}

	ReadPrepare();
	return true;

}

void ConcertPrepareScene::ActionDecision() {

	if(isIntro) {
		ActionIntro();
		return;
	}

	ActionPrepare();

	
}


void ConcertPrepareScene::ReadIntro() {

	type = ConcertTodo::NONE;

	auto points = RESMGR->FindImages(nullptr, "concert_scale_big", 0.99, 1, true, cvRect(820, 200, 193, 94));
	if (!points.empty()) {
		type = ConcertTodo::BIG;
		return;
	}

	points.clear();

	points = RESMGR->FindImages(nullptr, "concert_scale_guerrilla", 0.99, 1, true, cvRect(820, 200, 193, 94));
	if (!points.empty()) {
		type = ConcertTodo::GUERRILLA;
		return;
	}


}


void ConcertPrepareScene::ReadPrepare() {

	isNeedInfomation = false;
	isQuitConcert = false;
	isNeedDiamond = false;
	isMidnightAvailable = false;
	isMidnightScene = false;
	m_decision = NONE;

	{
		auto points = RESMGR->FindImages(nullptr, "concert_set_midnight", 0.99, 1, true, cvRect(610, 30, 327, 110));
		if (!points.empty()) {
			isMidnightAvailable = true;
		}

		points = RESMGR->FindImages(nullptr, "concert_set_normal", 0.99, 1, true, cvRect(610, 30, 327, 110));
		if (!points.empty()) {
			isMidnightScene = true;
		}
	}

	
	ConcertTodo* todo = PRODUCER->GetFirstTodo<ConcertTodo>();


	if(todo == nullptr) {
		return;
	}


	if(PRODUCER->GetConcertDance().team[0] <= 0) {
		isNeedInfomation = true;
	}

	//�Ӽ� ����
	//vocal
	if (RESMGR->CheckRGB(nullptr, 990, 170, 75, 190, 221, 4)) {
		todo->attribute = VOCAL;
	}
	//dance
	else if (RESMGR->CheckRGB(nullptr, 990, 170, 234, 58, 71, 4)) {
		todo->attribute = DANCE;
	}
	//performance
	else if (RESMGR->CheckRGB(nullptr, 990, 170, 239, 182, 49, 4)) {
		todo->attribute = PERFORMANCE;
	}

	if(todo->totalAudience == -1) {
		isQuitConcert = true;
		return;
	}

	if(!todo->isMidnight && isMidnightScene) {
		m_decision = CHANGE_MID;
		todo->totalAudience = -1;
		return;
	}

	//�ʿ��� LP �� ���
	if (todo->attribute != UNKOWN && todo->totalAudience > -1) {
		SetConcertTodo(todo);
	}

	//Ÿ���� �������� �����Ŷ�� �ڴʰ� �˾�æ �ܼ�Ʈ�̹Ƿ� ���� �߰�
	if(todo->type == ConcertTodo::NODATA) {

		long currentTime = GAME->GetUpdatedTime();

		auto points = RESMGR->FindImages(nullptr, "concert_scale_big_s", 0.99, 1, true, cvRect(137, 672, 193, 150));
		if (!points.empty()) {
			todo->type = ConcertTodo::BIG;
			if(todo->achieveTime <= (long)0)
				todo->achieveTime = GAME->GetUpdatedTime(); + ProducerAI::GetMillisecond(60, 00);
			return;
		}

		points.clear();

		points = RESMGR->FindImages(nullptr, "concert_scale_guerrilla_s", 0.99, 1, true, cvRect(137, 672, 193, 150));
		if (!points.empty()) {
			todo->type = ConcertTodo::GUERRILLA;
			if (todo->achieveTime <= (long)0)
				todo->achieveTime = GAME->GetUpdatedTime(); + ProducerAI::GetMillisecond(10, 00);
			return;
		}

		todo->isGiveUp = true;
	}

}


void ConcertPrepareScene::SetConcertTodo(ConcertTodo* todo) {

	int needLP = CheckNeedLP(todo->totalAudience, todo->attribute, todo->isMidnight);

	if(isNeedInfomation) {
		return;
	}

	todo->needLPCount = needLP;

	//�ܼ�Ʈ ���� ����
	DECISION dec = DecisionConcert(todo, needLP);

	switch (dec) {
		case WAIT:
		case NEED_DIA:
		case OK: 
		{
			//�ٷ� ���� ����
			//�ɾ� �ܼ�Ʈ�� �������� �ʾҰ� �ɾ� �ܼ�Ʈ�� ������ ���
			if(!todo->isMidnight && isMidnightAvailable) {
				int needLP_mid = needLP * 3;
				if (isNeedInfomation) { return; }

				DECISION dec_mid = DecisionConcert(todo, needLP_mid, false);

				//���� �ɾ� �ܼ�Ʈ�� �����Ѵٸ�
				if(dec_mid == NO) {
					m_decision = dec;
					break;
				}

				//�ɾ߸� �����ϰڴٸ�
				m_decision = CHANGE_MID;
				todo->isMidnight = true;
				todo->totalAudience = -1;
				todo->isWait = false;
				isNeedDiamond = false;
				todo->needLPCount = needLP_mid;
				break;

			}

			m_decision = dec;
		}
		break;

		case NONE:
		case NO: 
		{
			if(todo->isMidnight && isMidnightScene) {
				todo->isMidnight = false;
				todo->totalAudience = -1;
				m_decision = CHANGE_MID;
				break;
			}
			todo->isGiveUp = true;
			m_decision = NO;
		}
		break;

		default: break;
	}
}


int ConcertPrepareScene::CheckNeedLP(int totalAudience, ATTRIBUTE_UNIT unit, bool isMidnight) {

	totalAudience = totalAudience * 0.99f;
	int needLPCount = 0;

	int* units;

	//�Ӽ��� ���� �ܼ�Ʈ ���� ���� ȣ��
	switch (unit) {
		case DANCE:
			units = PRODUCER->GetConcertDance().team;
			break;
		case PERFORMANCE:
			units = PRODUCER->GetConcertPerformance().team;
			break;
		case VOCAL:
			units = PRODUCER->GetConcertVocal().team;
			break;
		default:
			units = nullptr;
			break;
	}

	if (units[0] <= 0) {
		isNeedInfomation = true;
		return 0;
	}

	//�ʿ��� LP ���
	for (int audience = 0; audience < totalAudience; ) {

		int pureSum = 0;
		int tempSum = 0;

		float scale = 1;
		for (int team = 0; team < 3; team++, scale += 0.5f) {

			pureSum += units[team];
			if (pureSum <= 0) {
				isNeedInfomation = true;
				return 0;
			}

			//�ó��� ȿ���� �߰�

			//�ɾ� �ܼ�Ʈ�� ���
			if (isMidnight) {
				tempSum = pureSum * scale * 3;
				//�ʿ��� LP�߰�
				needLPCount += 3;

			}
			//����� �ܼ�Ʈ ���
			else {
				tempSum = pureSum * scale;
				//�ʿ��� LP�߰�
				needLPCount++;
			}


			//������� ���� ���� ���� �ʿ��� ���� ���� Ȯ��
			if (audience + tempSum >= totalAudience) {
				audience += tempSum;
				break;
			}

		}

		audience += tempSum;
	}

	std::cout << "\n�ʿ��� LP : " << needLPCount << std::endl;
	return needLPCount;

}


ConcertPrepareScene::DECISION ConcertPrepareScene::DecisionConcert(ConcertTodo* todo, int needLPCount, bool isTodoSave) {

	//int needLPCount = todo->needLPCount;
	int currentLP = PRODUCER->GetLP().current;
	int scarceLP = 0;	//������ LP
	int scarceWaitLP = 0;	//�ð��� ������ ��� �ּ��� ������ LP

	long concertTime = todo->achieveTime - timeGetTime();
	std::cout << "�ܼ�Ʈ ���� �ð� : " << ProducerAI::Millisecond2Min(concertTime) << "�� " << ProducerAI::Millisecond2Second(concertTime) << "��\n";


	if (needLPCount - currentLP > 0) {
		scarceLP = needLPCount - currentLP;
	}


	//���� LP�� �ʿ��� LP�� �Ų� �� �ִٸ�
	else {
		std::cout << "���� LP�� " << currentLP << "�� �����Ƿ� �ٷ� ����" << std::endl;
		return OK;
	}

	std::cout << "���� LP�� " << currentLP << "���� ����ϸ� �� �ʿ��� LP�� " << scarceLP << "��." << std::endl;


	long LPTime = PRODUCER->GetLP().achieveTime;
	bool isLPOver = false;

	//�̹� lp�� �� á�� ��
	if (PRODUCER->GetLP().current >= PRODUCER->GetLP().max) {
		LPTime = timeGetTime() + ProducerAI::GetMillisecond(30, 00);
		isLPOver = true;
	}

	//1�������� ���� ���� �� LP ��Ÿ�� �ð��� �ܼ�Ʈ ���� �ð����� ���� ���
	if (todo->achieveTime <= LPTime) {
		long deltaTime = timeGetTime() - LPTime;

		std::cout << "�ð� ���� �Ұ���. �ſ� �˹���" << std::endl;
		std::cout << "�ܼ�Ʈ ���� �ð� : " << ProducerAI::Millisecond2Min(deltaTime) << std::endl;
		scarceWaitLP = scarceLP;

	} else {

		long deltaAchieveTime = todo->achieveTime - LPTime;

		for (int i = 1; i <= scarceLP; i++) {
			scarceWaitLP = scarceLP - i;

			//�ܼ�Ʈ�� LP i���� ���� ��ä��� ���� ���
			if (deltaAchieveTime <= i * ProducerAI::GetMillisecond(30, 00)) {
				long deltaTime = (i - 1) * ProducerAI::GetMillisecond(30, 00) + (LPTime - timeGetTime());
				std::cout << "LP " << i << "���� �Ⱓ ���� ȹ�� ����. �ִ��� ������ �ð��� " << ProducerAI::Millisecond2Min(deltaTime) << "�� " << ProducerAI::Millisecond2Second(deltaTime) << "��." << std::endl;
				break;
			}
		}


	}

	//���� �ð� ���� �ܼ�Ʈ�� ���� �� �ִٸ�
	if (scarceWaitLP <= 0) {
		//��ٷ�
		//todo->SetWait();
		//isQuitConcert = true;
		std::cout << "�ð� ���� ����. ��� ��ٸ�." << std::endl;
		if(isLPOver) {
			std::cout << "�׷��� �̹� LP�� �ִ�ġ�� �Ѿ��⿡ �ٷ� ����" << std::endl;
			return OK;
		}
		return WAIT;
	}

	std::cout << "�ð��� �ִ��� ������ �� �� �ʿ��� LP�� " << scarceWaitLP << "���� �ȴ�." << std::endl;


	//��...�ð����� ��������..?
	//��...�׷� ���� ������ LP ������ ä�� �� ���� ������...? (���� �߰�)
	//��..�ƴϸ� ���� �������� �ܼ�Ʈ �Ը� ���� ������ �Ĺ��Ĺ����� ������ ����

	bool isNeedDia = false;

	switch (PRODUCER->GetStatus()) {
	case ProducerAI::EVENT_NOMAL: {
		//�Ը��� �� ���� �� ����.....
		if (todo->type == ConcertTodo::GUERRILLA) {
			if (scarceWaitLP <= 3) {
				isNeedDia = true;
				break;
			}
		}
	}
		break;
	case ProducerAI::EVENT_IMPORTANT: {
		//�Ը���� ������ ���� �ξ�� ��... ����......
		if (todo->type == ConcertTodo::GUERRILLA) {
			isNeedDia = true;
			break;
		}

		//��Ը� �ܼ�Ʈ���� ������ LP�� Ȯ�� �� ����
		if (todo->type == ConcertTodo::BIG) {
			if (scarceWaitLP <= 3) {
				isNeedDia = true;
				break;
			}
		}
	}
		break;
	default: break;
	}

	if(isTodoSave) {
		todo->paidLPCount = scarceWaitLP;
		todo->needLPCount = needLPCount;
	}

	//���̾Ƶ� �Ⱦ��� ����
	if (!isNeedDia) {
		std::cout << "�� ���⸦ ������" << std::endl;
		//todo->isGiveUp = true;
		//isQuitConcert = true;
		return NO;
	}

	// ���̾Ƹ� ����� �����ϰ� �ð� ��Ÿ�� ������ ���� ������ ���
	if (scarceLP - scarceWaitLP > 0) {
		//��ٷ�
		//todo->SetWait();
		//isQuitConcert = true;
		std::cout << "�ִ��� LP�� �Ƴ��� " << scarceWaitLP << "���� ���� �������� ���̾Ʒ� �Ų�. ���� ��� ��ٸ�." << std::endl;
		if (isLPOver) {
			std::cout << "�׷��� �̹� LP�� �ִ�ġ�� �Ѿ��⿡ �ٷ� ����" << std::endl;
			return OK;
		}
		return WAIT;
	}

	std::cout << "���� LP " << scarceWaitLP << "���� ���̾Ʒ� �Ų�" << std::endl;


	//���̾Ƹ� �� �ð��̴�.
	//isNeedDiamond = true;
	return NEED_DIA;

}


void ConcertPrepareScene::ActionIntro() {

	long deltaTime = 0;

	switch (type) {
		case ConcertTodo::NONE: 
			GAME->SetMouseClick(1156, 900);
		break;
		case ConcertTodo::BIG:
			deltaTime = ProducerAI::GetMillisecond(60, 00);
		case ConcertTodo::GUERRILLA: 
		{
			//�� ù �κ��̶�� ���� ����Ʈ�� �߰�
			if(!isScene<ConcertPrepareScene>(SCENE->GetPrevScene())) {
				ConcertTodo* _todo = new ConcertTodo();
				PRODUCER->AddTodo(_todo);
				if (deltaTime <= 0) deltaTime = ProducerAI::GetMillisecond(10, 00);
				_todo->type = type;
				_todo->todo_str = "concert";
				_todo->achieveTime = timeGetTime() + deltaTime;
				_todo->targetScene = SCENE->GetScene<ConcertResultScene>();

			}
			else {
				Todo* todo = PRODUCER->GetTodo();
				if (todo != nullptr && todo->todo_str == "concert") {
					static_cast<ConcertTodo*>(todo)->type = type;
				}
			}


			GAME->SetMouseClick(1497, 900);
			break;
		}
		default: break;
	}

}


void ConcertPrepareScene::ActionPrepare() {

	ConcertTodo* todo = PRODUCER->GetFirstTodo<ConcertTodo>();



	if (todo != nullptr && todo->isGiveUp) {
		GAME->SetMouseClick(1100, 988);
		PRODUCER->RemoveTodo(todo);
		Sleep(1000);
		return;
	}

	if (todo == nullptr || m_decision == WAIT || isQuitConcert) {
		GAME->SetMouseClick(102, 77);
		return;
	}


	if(isNeedInfomation) {
		GAME->SetMouseClick(1712, 225);
		return;
	}

	switch (m_decision) {
	case NEED_DIA: {

	}
	return;


	case CHANGE_MID: {
		GAME->SetMouseClick(795, 74);
	}
	return;
	case NONE:
	return;

	default: break;
	}

	int needLP = todo->needLPCount;
	todo->usedLP = 1;

	if(needLP >= 3) {
		GAME->SetMouseClick(1724, 764);
		todo->usedLP = 3;
	}else if(needLP >= 2) {
		GAME->SetMouseClick(1724, 620);
		todo->usedLP = 2;
	}else if(needLP <= 0) {
		PRODUCER->RemoveTodo(todo);
		GAME->SetMouseClick(102, 77);
		return;
	}
	
	Sleep(500);

	GAME->SetMouseClick(1595, 988);


}
