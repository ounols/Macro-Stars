#include "ConcertPrepareScene.h"
#include "ResMgr.h"
#include "ConcertTodo.h"
#include "GameClientMgr.h"
#include "ProducerAI.h"
#include "SceneMgr.h"
#include "MainScene.h"
#include "ProduceTodo.h"
#include "CoverTodo.h"
#include <vector>


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
	RESMGR->RegisterImage("img/ConcertScene/midnight_bae.jpg", "concert_midnight_bae");
	RESMGR->RegisterImage("img/ConcertScene/switch_appearance.jpg", "concert_switch_appearance");



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
		auto points = RESMGR->FindImages(nullptr, "concert_enter_now", 0.98, 1, true, cvRect(1100, 695, 567, 113));
		if (points.empty()) {

			points = RESMGR->FindImages(nullptr, "concert_enter_normal", 0.98, 1, true, cvRect(1100, 695, 567, 113));
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
	ReadLP();
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

	//임시======================================================
	//PRODUCER->SetStatus(ProducerAI::EVENT_IMPORTANT);
	//==========================================================

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

		if(!isMidnightScene) {
			auto points = RESMGR->FindImages(nullptr, "concert_midnight_bae", 0.99, 1, true, cvRect(723, 728, 65, 65));
			if (!points.empty()) {
				isMidnightScene = true;
			}
		}
	}

	
	ConcertTodo* todo = PRODUCER->GetFirstTodo<ConcertTodo>();


	if(todo == nullptr) {
		std::cout << "Nullptr Todo\n";
		// PRODUCER->RemoveAllTodo<ConcertTodo>();
		ConcertTodo* todo_p = new ConcertTodo();
		PRODUCER->AddTodo(todo_p);
		todo_p->type = type;
		todo_p->todo_str = "concert";
		todo_p->targetScene = SCENE->GetScene<ConcertResultScene>();

		
		int index = 0;
		for(auto todo : PRODUCER->GetTodoList()) {
			Scene* scene = todo->targetScene;
			std::cout << "todo[" << index << "]\n\ttarget scene : " << (scene != nullptr ? scene->GetName() : "Null") << std::endl;
			std::cout << "\tisActive : " << (todo->isAvailable() ? "true" : "false") << std::endl;
			std::cout << "\ttodo_str : " << todo->todo_str << std::endl;
			std::cout << "\timportant : " << todo->important << std::endl;
			std::cout << std::endl;

			index++;
		}
		
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
		std::cout << "Need Total Audience\n";
		return;
	}

	if(!todo->isMidnight && isMidnightScene) {
		m_decision = CHANGE_MID;
		todo->totalAudience = -1;
		std::cout << "Need Total Audience(No Midnight)\n";
		return;
	}

	std::cout << "attri : " << todo->attribute << "\n";
	std::cout << "totalAudience : " << todo->totalAudience << "\n";
	std::cout << "type : " << todo->type << "\n";

	//필요한 LP 등 계산
	if (todo->attribute != UNKOWN && todo->totalAudience > -1 && todo->type != ConcertTodo::NODATA) {
		SetConcertTodo(todo);
	}

	//타입이 설정되지 않은거라면 뒤늦게 알아챈 콘서트이므로 새로 추가
	if(todo->type == ConcertTodo::NODATA) {

		long currentTime = GAME->GetUpdatedTime();

		auto points = RESMGR->FindImages(nullptr, "concert_scale_big_s", 0.99, 1, true, cvRect(137, 672, 193, 150));
		if (!points.empty()) {
			std::cout << "reconized BIG Concert\n";
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

		std::cout << "GiveUp\n";
		todo->isGiveUp = true;
	}

}


void ConcertPrepareScene::ReadLP() {

	std::string lp_str = GetNumber(1064, 73, 96, 28);

	int lp_slash;



	if ((lp_slash = lp_str.find("/")) != std::string::npos) {
		EnsembleUnit lp;

		lp.max = std::stoi(lp_str.substr(lp_slash + 1));
		lp.current = std::stoi(lp_str.substr(0, lp_slash));
		lp.achieveTime = PRODUCER->GetLP().achieveTime;

		if (lp.max - lp.current <= 0) {
			lp.achieveTime = GAME->GetUpdatedTime();
		}

		PRODUCER->SetLP(lp);
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

			////임시 추가
			//if (!todo->isMidnight && isMidnightAvailable) {
			//	if(todo->type == ConcertTodo::GUERRILLA) {
			//		//심야를 진행하겠다면
			//		m_decision = CHANGE_MID;
			//		todo->isMidnight = true;
			//		todo->totalAudience = -1;
			//		todo->isWait = false;
			//		isNeedDiamond = false;
			//		int needLP_mid = needLP * 3;
			//		todo->needLPCount = needLP_mid;
			//		break;
			//	}
			//}

			m_decision = dec;
		}
		break;

		case NEED_DIA: {
			////임시 추가
			//if (!todo->isMidnight && isMidnightAvailable) {
			//	if (todo->type == ConcertTodo::GUERRILLA) {
			//		//심야를 진행하겠다면
			//		m_decision = CHANGE_MID;
			//		todo->isMidnight = true;
			//		todo->totalAudience = -1;
			//		todo->isWait = false;
			//		isNeedDiamond = false;
			//		int needLP_mid = needLP * 3;
			//		todo->needLPCount = needLP_mid;
			//		break;
			//	}
			//}
			m_decision = dec;
		}
		break;

		case LEVEL_UP: 
		{
			{
				auto todo = PRODUCER->GetTodo<ProduceTodo>();

				//프로듀싱 할일 삭제
				if (todo != nullptr) {
					PRODUCER->RemoveTodo(todo);
				}

			}

			ProduceTodo* todo_new = new ProduceTodo();
			todo_new->isForLevelUp = true;
			todo_new->type = ProduceTodo::DAILY;
			PRODUCER->AddTodo(todo_new);

			if(PRODUCER->GetTodo<CoverTodo>() == nullptr) {
				CoverTodo* todo_cover = new CoverTodo();
				todo_cover->targetScene = SCENE->GetScene<ResultProduceScene>();
				PRODUCER->AddTodo(todo_cover);
			}

			todo->SetWait();
			isQuitConcert = true;
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
		std::cout << "It needs infomation\n";
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
			std::cout << "It needs infomation(while cur LP)\n";

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
	int waitCount = 0;
	bool isUsingDiaUntilFullLv = (PRODUCER->GetStatus() == ProducerAI::EVENT_IMPORTANT);
	int limitedMinutes = (PRODUCER->GetStatus() == ProducerAI::EVENT_IMPORTANT) ? 30 : 0; //limiting time when concert process is slow
	limitedMinutes -= 10;
	
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
		long deltaTime = LPTime - timeGetTime();

		std::cout << "시간 내로 불가능. 매우 촉박함" << std::endl;
		std::cout << "콘서트 남은 시간 : " << ProducerAI::Millisecond2Min(deltaTime) << std::endl;
		scarceWaitLP = scarceLP;

	} else {

		long deltaAchieveTime = todo->achieveTime - LPTime;

		for (int i = 1; i <= scarceLP; i++) {
			scarceWaitLP = scarceLP - i;
			waitCount = i;

			//콘서트가 LP i개를 마저 못채우고 끝날 경우
			if (deltaAchieveTime <= i * ProducerAI::GetMillisecond(30, 00)) {
				long deltaTime = (i - 1) * ProducerAI::GetMillisecond(30, 00) + (LPTime - timeGetTime());
				std::cout << "LP " << i << "개를 기간 내로 획득 가능. 최대한 모으는 시간은 " << ProducerAI::Millisecond2Min(deltaTime) << "분 " << ProducerAI::Millisecond2Second(deltaTime) << "초." << std::endl;
				break;
			}
		}


	}

	//If using dia until concert level will reach fully
	if(isUsingDiaUntilFullLv && todo->totalAudience < 500000){
		return NEED_DIA;
	}

	//만약 시간 내로 콘서트를 끝낼 수 있다면
	if (scarceWaitLP <= 0) {

		//if(PRODUCER->GetLP().current >= 1) {
		//	scarceWaitLP = scarceLP;
		//}else {
			//기다령
			//todo->SetWait();
			//isQuitConcert = true;
			std::cout << "시간 내로 가능. 잠시 기다림." << std::endl;
			if (isLPOver) {
				std::cout << "그러나 이미 LP가 최대치를 넘었기에 바로 진행" << std::endl;
				return OK;
			}

			if(limitedMinutes <= 0)	return WAIT;

			// limiting about timeout
			if(limitedMinutes >= ProducerAI::Millisecond2Min(concertTime)){
				std::cout << "LIMITING TIME!\n";
				return NEED_DIA;
			}else
				return WAIT;
		//}





	}

	std::cout << "시간이 최대한 지나고 난 뒤 필요한 LP는 " << scarceWaitLP << "개가 된다." << std::endl;


	//어...시간내로 못끝내나..?
	//ㄱ...그럼 레벨 업으로 LP 꽁으로 채울 수 있지 않을까...? (차후 추가)
	int currentEXP = PRODUCER->GetCurrentEXP();
	int totalEXP = PRODUCER->GetTotalEXP();

	std::cout << "현재 경험치 : " << currentEXP << " / " << totalEXP << std::endl;

	//현존하는 프로듀스 중 한번 당 가장 많이 경험치를 주는 프로듀스를 기준으로
	float produceAPCount = 60;

	int needProduceCount = ceil((totalEXP - currentEXP) / (produceAPCount * 10.f));
	int currentProduceCount = PRODUCER->GetAP().current / produceAPCount;

	std::cout << "필요한 프로듀스 횟수 : " << needProduceCount << ", 현재 AP로 획득 가능한 프로듀스 수 : " << currentProduceCount << "\n";

	//현재 AP로 레벨업이 가능하다면
	if(currentProduceCount - needProduceCount >= 0) {
		long concertTime = todo->achieveTime - timeGetTime();

		std::cout << "AP 충전하는데 걸리는 시간 : " << needProduceCount * 5 << "분, 콘서트 남은 기간 : " << ProducerAI::Millisecond2Min(concertTime) << "분\n";

		//한 프로듀스를 진행하는데 걸리는 시간이 성능에도 큰 영향을 받으므로
		//평균 5분으로 지정
		//프로듀스를 진행하는 시간보다 콘서트 기간이 더 긴 경우
		if(needProduceCount * 5 <= ProducerAI::Millisecond2Min(concertTime)) {
			//레벨업으로 LP 충전 가능
			std::cout << "레벨업으로 커버 가능\n";
			return LEVEL_UP;
		}

	}

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



	{
		int needDiaForAP = 0;
		int needDiaForLP = scarceWaitLP * 10;

		//한 프로듀스를 진행하는데 걸리는 시간이 성능에도 큰 영향을 받으므로
		//평균 5분으로 지정
		//프로듀스를 진행하는 시간보다 콘서트 기간이 더 긴 경우
		if (needProduceCount * 5 <= ProducerAI::Millisecond2Min(concertTime)) {
			//레벨업으로 LP 충전 가능
			std::cout << "레벨업으로 커버 가능한 AP를 소지함.\n";

			needDiaForAP = (needProduceCount - currentProduceCount) * (produceAPCount / 2.f);
			std::cout << needDiaForAP << "개의 다이아를 사용하면 레벨업 가능\n";

			//AP로 레벨업 하는게 더 효율적이라면
			if (needDiaForLP > needDiaForAP) {
				std::cout << "레벨업이 다이아 " << needDiaForLP - needDiaForAP << "개 차이로 저렴함\n";
				return LEVEL_UP;
			}
			
		}
	}


	// 다이아를 쓰기로 결정하고 시간 쿨타임 보정이 아직 먹히는 경우
	if (scarceLP - scarceWaitLP > 0) {

		//if (PRODUCER->GetLP().current >= 1) {
		//	scarceWaitLP = scarceLP;
		//} else {
			//기다령
			//todo->SetWait();
			//isQuitConcert = true;
			std::cout << "최대한 LP를 아끼면 " << scarceWaitLP << "개가 남고 나머지는 다이아로 매꿈. 따라서 잠시 기다림." << std::endl;
			if (isLPOver) {
				std::cout << "그러나 이미 LP가 최대치를 넘었기에 바로 진행" << std::endl;
				return OK;
			}
			return WAIT;
		//}


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
			SCENE->LockScene();
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
			SCENE->LockScene();
			break;
		}
		default: break;
	}

}


void ConcertPrepareScene::ActionPrepare() {

	ConcertTodo* todo = PRODUCER->GetFirstTodo<ConcertTodo>();


	if (todo != nullptr && todo->isGiveUp) {
		GAME->SetMouseClick(1100, 988);
		if(todo->isGiveUp)
			PRODUCER->RemoveTodo(todo);
		std::cout << "GiveUp (ActionPre)\n";
		Sleep(1000);
		return;
	}

	if (todo == nullptr || m_decision == WAIT || isQuitConcert) {

		if(m_decision == WAIT) {
			todo->SetWait();
		}
		GAME->SetMouseClick(102, 77);
		return;
	}


	if(isNeedInfomation) {
		GAME->SetMouseClick(1712, 225);
		Sleep(1000);
		SCENE->LockScene();
		return;
	}

	switch (m_decision) {
	case NEED_DIA: {
		if (PRODUCER->GetTodo<CoverTodo>() != nullptr) break;
		CoverTodo* todo_cover = new CoverTodo();
		todo_cover->targetScene = SCENE->GetScene<ResultProduceScene>();
		PRODUCER->AddTodo(todo_cover);
		break;
	}


	case CHANGE_MID: {
		GAME->SetMouseClick(795, 74);
	}
	return;
	case NONE:
	return;

	default: break;
	}

	bool isSwitchActive[2] = { false };

	{
		auto points = RESMGR->FindImages(nullptr, "concert_switch_appearance", 0.98, 1, true, cvRect(1558, 602, 235, 89));
		if (!points.empty()) {
			isSwitchActive[0] = true;
		}

		points = RESMGR->FindImages(nullptr, "concert_switch_appearance", 0.98, 1, true, cvRect(1558, 750, 235, 89));
		if (!points.empty()) {
			isSwitchActive[1] = true;
		}
	}

	int needLP = todo->needLPCount;
	todo->usedLP = (todo->isMidnight ? 3 : 1);

	if(needLP >= 3) {
		if(!isSwitchActive[1]) {
			GAME->SetMouseClick(1724, 764);
		}
		todo->usedLP = (todo->isMidnight ? 9 : 3);
	}else if(needLP >= 2) {
		if (isSwitchActive[1]) {
			GAME->SetMouseClick(1724, 764);
		}
		if (!isSwitchActive[0]) {
			GAME->SetMouseClick(1724, 620);
		}
		todo->usedLP = (todo->isMidnight ? 6 : 2);
	}else if(needLP <= 0) {
		PRODUCER->RemoveTodo(todo);
		GAME->SetMouseClick(102, 77);
		return;
	}else {
		if (isSwitchActive[0]) {
			GAME->SetMouseClick(1724, 620);
		}
	}
	
	Sleep(500);

	GAME->SetMouseClick(1595, 988);
	SCENE->LockScene();

}


std::string ConcertPrepareScene::GetNumber(int x, int y, int width, int height) {

	IplImage* img = (IplImage*)cvClone(GAME->GetScreenImage());

	CUT_IMAGE(img, cvRect(x, y, width, height));

	RESIZE_IMAGE(img, cvSize(img->width * 2, img->height * 2));
	IplImage* img_over = (IplImage*)cvClone(img);

	//cvShowImage("이진", img);
	//cvWaitKey();

	{
		IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		cvCvtColor(img, dst, CV_RGB2GRAY);
		REALLOC(img, dst);

		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		CvScalar scalar_min = cvScalar(225, 225, 225);
		CvScalar scalar_max = cvScalar(255, 255, 255);

		cvInRangeS(img, scalar_min, scalar_max, dst);
		REALLOC(img, dst);

		ResMgr::__MaskImage(img_over, cvScalar(250, 230, 96), 35, img);
	}

	//cvShowImage("이진", img);
	//cvWaitKey();

	std::string str = RESMGR->Image2String(img);
	//cvWaitKey();

	cvReleaseImage(&img);
	cvReleaseImage(&img_over);

	return str;

}
