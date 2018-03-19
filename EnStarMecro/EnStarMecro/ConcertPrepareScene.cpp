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

	//속성 설정
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

	//필요한 LP 등 계산
	if (todo->attribute != UNKOWN && todo->totalAudience > -1) {
		SetConcertTodo(todo);
	}

	//타입이 설정되지 않은거라면 뒤늦게 알아챈 콘서트이므로 새로 추가
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

	//콘서트 진행 결정
	DECISION dec = DecisionConcert(todo, needLP);

	switch (dec) {
		case WAIT:
		case NEED_DIA:
		case OK: 
		{
			//바로 진행 가능
			//심야 콘서트로 설정되지 않았고 심야 콘서트가 가능한 경우
			if(!todo->isMidnight && isMidnightAvailable) {
				int needLP_mid = needLP * 3;
				if (isNeedInfomation) { return; }

				DECISION dec_mid = DecisionConcert(todo, needLP_mid, false);

				//만약 심야 콘서트를 포기한다면
				if(dec_mid == NO) {
					m_decision = dec;
					break;
				}

				//심야를 진행하겠다면
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

	//속성에 따른 콘서트 유닛 스펙 호출
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

	//필요한 LP 계산
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

			//시너지 효과를 추가

			//심야 콘서트일 경우
			if (isMidnight) {
				tempSum = pureSum * scale * 3;
				//필요한 LP추가
				needLPCount += 3;

			}
			//평범한 콘서트 경우
			else {
				tempSum = pureSum * scale;
				//필요한 LP추가
				needLPCount++;
			}


			//현재까지 계산된 관객 수와 필요한 관객 수를 확인
			if (audience + tempSum >= totalAudience) {
				audience += tempSum;
				break;
			}

		}

		audience += tempSum;
	}

	std::cout << "\n필요한 LP : " << needLPCount << std::endl;
	return needLPCount;

}


ConcertPrepareScene::DECISION ConcertPrepareScene::DecisionConcert(ConcertTodo* todo, int needLPCount, bool isTodoSave) {

	//int needLPCount = todo->needLPCount;
	int currentLP = PRODUCER->GetLP().current;
	int scarceLP = 0;	//부족한 LP
	int scarceWaitLP = 0;	//시간을 지내서 얻는 최선의 부족한 LP

	long concertTime = todo->achieveTime - timeGetTime();
	std::cout << "콘서트 남은 시간 : " << ProducerAI::Millisecond2Min(concertTime) << "분 " << ProducerAI::Millisecond2Second(concertTime) << "초\n";


	if (needLPCount - currentLP > 0) {
		scarceLP = needLPCount - currentLP;
	}


	//현재 LP로 필요한 LP를 매꿀 수 있다면
	else {
		std::cout << "현재 LP가 " << currentLP << "개 있으므로 바로 진행" << std::endl;
		return OK;
	}

	std::cout << "현재 LP가 " << currentLP << "개를 사용하면 더 필요한 LP는 " << scarceLP << "개." << std::endl;


	long LPTime = PRODUCER->GetLP().achieveTime;
	bool isLPOver = false;

	//이미 lp가 꽉 찼을 때
	if (PRODUCER->GetLP().current >= PRODUCER->GetLP().max) {
		LPTime = timeGetTime() + ProducerAI::GetMillisecond(30, 00);
		isLPOver = true;
	}

	//1차적으론 가장 먼저 올 LP 쿨타임 시간이 콘서트 마감 시간보다 늦을 경우
	if (todo->achieveTime <= LPTime) {
		long deltaTime = timeGetTime() - LPTime;

		std::cout << "시간 내로 불가능. 매우 촉박함" << std::endl;
		std::cout << "콘서트 남은 시간 : " << ProducerAI::Millisecond2Min(deltaTime) << std::endl;
		scarceWaitLP = scarceLP;

	} else {

		long deltaAchieveTime = todo->achieveTime - LPTime;

		for (int i = 1; i <= scarceLP; i++) {
			scarceWaitLP = scarceLP - i;

			//콘서트가 LP i개를 마저 못채우고 끝날 경우
			if (deltaAchieveTime <= i * ProducerAI::GetMillisecond(30, 00)) {
				long deltaTime = (i - 1) * ProducerAI::GetMillisecond(30, 00) + (LPTime - timeGetTime());
				std::cout << "LP " << i << "개를 기간 내로 획득 가능. 최대한 모으는 시간은 " << ProducerAI::Millisecond2Min(deltaTime) << "분 " << ProducerAI::Millisecond2Second(deltaTime) << "초." << std::endl;
				break;
			}
		}


	}

	//만약 시간 내로 콘서트를 끝낼 수 있다면
	if (scarceWaitLP <= 0) {
		//기다령
		//todo->SetWait();
		//isQuitConcert = true;
		std::cout << "시간 내로 가능. 잠시 기다림." << std::endl;
		if(isLPOver) {
			std::cout << "그러나 이미 LP가 최대치를 넘었기에 바로 진행" << std::endl;
			return OK;
		}
		return WAIT;
	}

	std::cout << "시간이 최대한 지나고 난 뒤 필요한 LP는 " << scarceWaitLP << "개가 된다." << std::endl;


	//어...시간내로 못끝내나..?
	//ㄱ...그럼 레벨 업으로 LP 꽁으로 채울 수 있지 않을까...? (차후 추가)
	//ㅇ..아니면 나의 의지도와 콘서트 규모에 따라 돈으로 쳐발쳐발할지 생각을 하자

	bool isNeedDia = false;

	switch (PRODUCER->GetStatus()) {
	case ProducerAI::EVENT_NOMAL: {
		//게릴라 좀 보고 돈 쓰자.....
		if (todo->type == ConcertTodo::GUERRILLA) {
			if (scarceWaitLP <= 3) {
				isNeedDia = true;
				break;
			}
		}
	}
		break;
	case ProducerAI::EVENT_IMPORTANT: {
		//게릴라는 무조건 돈을 부어야 해... 헤헷......
		if (todo->type == ConcertTodo::GUERRILLA) {
			isNeedDia = true;
			break;
		}

		//대규모 콘서트에서 부족한 LP를 확인 후 결정
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

	//다이아도 안쓰면 포기
	if (!isNeedDia) {
		std::cout << "돈 쓰기를 포기함" << std::endl;
		//todo->isGiveUp = true;
		//isQuitConcert = true;
		return NO;
	}

	// 다이아를 쓰기로 결정하고 시간 쿨타임 보정이 아직 먹히는 경우
	if (scarceLP - scarceWaitLP > 0) {
		//기다령
		//todo->SetWait();
		//isQuitConcert = true;
		std::cout << "최대한 LP를 아끼면 " << scarceWaitLP << "개가 남고 나머지는 다이아로 매꿈. 따라서 잠시 기다림." << std::endl;
		if (isLPOver) {
			std::cout << "그러나 이미 LP가 최대치를 넘었기에 바로 진행" << std::endl;
			return OK;
		}
		return WAIT;
	}

	std::cout << "남은 LP " << scarceWaitLP << "개를 다이아로 매꿈" << std::endl;


	//다이아를 쓸 시간이다.
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
			//씬 첫 부분이라면 할일 리스트에 추가
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
