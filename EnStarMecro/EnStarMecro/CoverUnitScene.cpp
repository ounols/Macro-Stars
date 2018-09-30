#include "CoverUnitScene.h"
#include "ResMgr.h"
#include "ProducerAI.h"
#include "CoverTodo.h"
#include "GameClientMgr.h"
#include "MainScene.h"
#include "ConcertTodo.h"


CoverUnitScene::CoverUnitScene() {
	m_name = "CoverUnitScene";

	RESMGR->RegisterImage("img/CoverUnitScene/pop_ap.jpg", "cover_pop_ap");
	RESMGR->RegisterImage("img/CoverUnitScene/pop_lp.jpg", "cover_pop_lp");
	RESMGR->RegisterImage("img/CoverUnitScene/pop_use_item.jpg", "cover_pop_use_item");
	RESMGR->RegisterImage("img/CoverUnitScene/pop_charge_lp.jpg", "cover_pop_charge_lp");
	RESMGR->RegisterImage("img/CoverUnitScene/pop_buy_item.jpg", "cover_pop_buy_item");
	RESMGR->RegisterImage("img/CoverUnitScene/pop_limited_item.jpg", "cover_pop_limited_item");

	RESMGR->RegisterImage("img/CoverUnitScene/item_bread.jpg", "cover_item_bread");
	RESMGR->RegisterImage("img/CoverUnitScene/item_dia.jpg", "cover_item_dia");
	RESMGR->RegisterImage("img/CoverUnitScene/item_sportdrink.jpg", "cover_item_sportdrink");
	RESMGR->RegisterImage("img/CoverUnitScene/item_yogurt.jpg", "cover_item_yogurt");
	RESMGR->RegisterImage("img/CoverUnitScene/item_limit.jpg", "cover_item_limit");

	RESMGR->RegisterImage("img/CoverUnitScene/pop_lp_sale.jpg", "cover_pop_lp_sale");
	RESMGR->RegisterImage("img/CoverUnitScene/pop_lp_sale_buy.jpg", "cover_pop_lp_sale_buy");

	RESMGR->RegisterImage("img/CoverUnitScene/bt_use_yellow.jpg", "cover_bt_use_yellow");



}


CoverUnitScene::~CoverUnitScene() {
	
}


bool CoverUnitScene::CheckFirst() {

	bool isScene = true;
	m_state = NONE;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 750, 210, 7, 72, 134, 3);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1160, 210, 6, 58, 110, 3);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 680, 300, 221, 178, 75, 3);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1225, 300, 214, 163, 60, 3);

	if (isScene) m_state = LIMITED_ITEM;

	return isScene;
}


bool CoverUnitScene::CheckScene() {

	CvRect rect = cvRect(734, 165, 450, 180);

	//이벤트 기간 - 전용 한정 아이템
	if(m_state == LIMITED_ITEM) {
		auto points = RESMGR->FindImages(nullptr, "cover_pop_limited_item", 0.98, 1, true, rect);
		if (points.empty()) return false;
		std::cout << "이벤트 기간 - 전용 한정 아이템\n";
		return true;
	}

	//ap회복
	auto points = RESMGR->FindImages(nullptr, "cover_pop_ap", 0.98, 1, true, rect);
	if (!points.empty()) {
		m_state = COVER_AP;
		std::cout << "ap회복\n";
		return true;
	}

	//lp회복
	points = RESMGR->FindImages(nullptr, "cover_pop_lp", 0.98, 1, true, rect);
	if (!points.empty()) {
		m_state = COVER_LP;
		std::cout << "lp회복\n";
		return true;
	}

	//아이템 사용
	points = RESMGR->FindImages(nullptr, "cover_pop_use_item", 0.98, 1, true, rect);
	if (!points.empty()) {
		m_state = USE_ITEM;
		std::cout << "아이템 사용\n";
		return true;
	}

	//LP 충전
	points = RESMGR->FindImages(nullptr, "cover_pop_charge_lp", 0.98, 1, true, rect);
	if (!points.empty()) {
		m_state = USE_ITEM;
		std::cout << "LP 충전 (아이템 사용)\n";
		return true;
	}

	//아이템 구매
	points = RESMGR->FindImages(nullptr, "cover_pop_buy_item", 0.98, 1, true, rect);
	if (!points.empty()) {
		m_state = BUY_ITEM;
		std::cout << "아이템 구매\n";
		return true;
	}

	return false;

}


bool CoverUnitScene::ReadData() {

	m_pos = cvPoint(0, 0);

	switch (m_state) {

		case COVER_AP: {
			ReadCoverAP();
		} break;

		case COVER_LP: {
			ReadCoverLP();
		} break;

		case USE_ITEM: {
			ReadUseItem();
		} break;

		case BUY_ITEM: {
			ReadBuyItem();
		} break;

		case LIMITED_ITEM: {
			ReadLimitedItem();
		} break;

		default: break;
	}

	return true;
}


void CoverUnitScene::ActionDecision() {

	GAME->SetMouseClick(m_pos.x, m_pos.y);

}


void CoverUnitScene::ReadCoverAP() {

	auto todo = PRODUCER->GetTodo<CoverTodo>();
	m_pos = cvPoint(958, 850);

	if(todo == nullptr) {
		Todo* todo = new Todo();
		todo->targetScene = SCENE->GetScene<MainScene>();
		PRODUCER->AddTodo(todo);
		return;
	}

	CvRect rect = cvRect(430, 290, 234, 194);

	//사용버튼이 존재하는지 확인
	{
		auto points = RESMGR->FindImages(nullptr, "cover_bt_use_yellow", 0.98, 1, true, cvRect(1300, 319, 194, 151));
		if (!points.empty()) {
			std::cout << "사용 버튼이 있음\n";
			m_pos = cvPoint(1400, 390);
		}
	}

	//메인화면에서 지원받는거라면 
	if (isScene<MainScene>(todo->targetScene)) {

		//기간이 남은 아이템인가?
		auto points = RESMGR->FindImages(nullptr, "cover_item_limit", 0.99, 1, true, cvRect(663, 443, 146, 50));
		if (points.empty()) {
			std::cout << "중요 이벤트에만 사용함. 고로 넘김\n";
			todo->Destroy();
			m_pos = cvPoint(958, 850);
			return;
		}

	}
	

	auto points = RESMGR->FindImages(nullptr, "cover_item_dia", 0.98, 1, true, rect);
	if (!points.empty()) {
		std::cout << "다이아 사용 + 1 ap\n";
		todo->coveredAP = 1;
		return;
	}

	points = RESMGR->FindImages(nullptr, "item_bread", 0.98, 1, true, rect);
	if (!points.empty()) {
		std::cout << "빵 사용 + max ap\n";
		todo->coveredAP = PRODUCER->GetAP().max;
		return;
	}

}


void CoverUnitScene::ReadCoverLP() {

	auto todo = PRODUCER->GetTodo<CoverTodo>();
	m_pos = cvPoint(700, 850);

	if (todo == nullptr) {
		return;
	}

	CvRect rect = cvRect(430, 290, 234, 194);

	//사용버튼이 존재하는지 확인
	{
		auto points = RESMGR->FindImages(nullptr, "cover_bt_use_yellow", 0.98, 1, true, cvRect(1300, 319, 194, 151));
		if (!points.empty()) {
			std::cout << "사용 버튼이 있음\n";
			m_pos = cvPoint(1400, 390);
		}
	}


	//중요 이벤트가 아니라면 그리고 메인화면에서 지원받는거라면 
	if((PRODUCER->GetStatus() != ProducerAI::EVENT_IMPORTANT && PRODUCER->GetStatus() != ProducerAI::EVENT_NOMAL) ||
		isScene<MainScene>(todo->targetScene)) {
		
		//기간이 남은 아이템인가?
		auto points = RESMGR->FindImages(nullptr, "cover_item_limit", 0.99, 1, true, cvRect(663, 443, 146, 50));
		if (points.empty()) {
			std::cout << "중요 이벤트에만 사용함. 고로 넘김\n";
			auto concert_todo = PRODUCER->GetTodo<ConcertTodo>();

			if(concert_todo != nullptr)
				concert_todo->isGiveUp = true;

			todo->Destroy();
			m_pos = cvPoint(700, 850);

			return;
		}

	}


	auto points = RESMGR->FindImages(nullptr, "cover_item_dia", 0.98, 1, true, rect);
	if (!points.empty()) {

		//기간 한정 세일이 진행 중인지 확인
		auto point_limited = RESMGR->FindImages(nullptr, "cover_pop_lp_sale", 0.98, 1, true, cvRect(390, 150, 310, 130));
		if(!point_limited.empty() && PRODUCER->GetStatus() == ProducerAI::EVENT_IMPORTANT) {
			std::cout << "한정 이벤트 물품 세일 중이기에 사러 감.\n";
			m_pos = cvPoint(456, 198);
			return;
		}

		std::cout << "다이아 사용 + 1 lp\n";
		todo->coveredLP = 1;
		return;
	}

	points = RESMGR->FindImages(nullptr, "cover_item_sportdrink", 0.98, 1, true, rect);
	if (!points.empty()) {
		std::cout << "스포츠 음료 사용 + 5 lp\n";
		todo->coveredLP = 5;
		return;
	}

	points = RESMGR->FindImages(nullptr, "cover_item_yogurt", 0.98, 1, true, rect);
	if (!points.empty()) {
		std::cout << "요구르트 사용 + 1 lp\n";
		todo->coveredLP = 1;
		return;
	}

}


void CoverUnitScene::ReadUseItem() {

	auto todo = PRODUCER->GetTodo<CoverTodo>();
	m_pos = cvPoint(1440, 170);

	if (todo == nullptr) {
		return;
	}

	std::string count_str = GetNumber(794, 512, 320, 55);
	int count = std::stoi(count_str);

	std::cout << count << "개 사용함\n";

	todo->UpdateCovered(count);
	todo->Destroy();

	m_pos = cvPoint(960, 830);
}


void CoverUnitScene::ReadBuyItem() {

	auto todo = PRODUCER->GetTodo<CoverTodo>();
	m_pos = cvPoint(1440, 170);

	if (todo == nullptr) {
		return;
	}

	m_pos = cvPoint(960, 830);

}


void CoverUnitScene::ReadLimitedItem() {

	auto todo = PRODUCER->GetTodo<CoverTodo>();
	m_pos = cvPoint(1415, 270);

	if (todo == nullptr) {
		return;
	}

	auto points = RESMGR->FindImages(nullptr, "cover_pop_lp_sale_buy", 0.98, 2, true, cvRect(566, 612, 612, 123));
	if (!points.empty()) {

		CvPoint point_result = cvPoint(9999, 9999);
		for(auto point : points) {
			
			if(point_result.x > point.x) {
				point_result = point;
			}

		}

		m_pos = cvPoint(566 + 140 + point_result.x, 612 + 50 + point_result.y);

		return;
	}

}


std::string CoverUnitScene::GetNumber(int x, int y, int width, int height) {

	IplImage* img = (IplImage*)cvClone(GAME->GetScreenImage());

	CUT_IMAGE(img, cvRect(x, y, width, height));


	RESIZE_IMAGE(img, cvSize(img->width * 3, img->height * 3));

	{
		IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		cvCvtColor(img, dst, CV_RGB2GRAY);
		REALLOC(img, dst);

		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		int color = 170;
		CvScalar scalar_min = cvScalar(color, color, color);
		CvScalar scalar_max = cvScalar(255, 255, 255);

		cvInRangeS(img, scalar_min, scalar_max, dst);
		REALLOC(img, dst);

		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		cvErode(img, dst, nullptr, 1);
		REALLOC(img, dst);

	}
	//cvShowImage("sample", img);
	//cvWaitKey();
	std::string str = RESMGR->Image2String(img);
	//cvWaitKey();

	cvReleaseImage(&img);

	return str;

}
