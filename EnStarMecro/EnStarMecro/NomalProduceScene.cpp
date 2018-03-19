#include "NomalProduceScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "ProducerAI.h"
#include "ResultProduceScene.h"
#include "ConcertTodo.h"
#include "ProduceTodo.h"
#include "MainScene.h"


static int GetChooseIndex(CvPoint pos) {
	
	int x = pos.x;
	x -= 244;
	x /= 288;

	return x;


}

static CvPoint GetChoosePos(int index) {

	int x = 132;

	x += 288 * (index + 1);

	return cvPoint(x, 930);


}


//void Print(std::vector<CvPoint> points, const char* say, CvScalar color = CV_RGB(255, 0, 0)) {
//	
//	IplImage* screen = GAME->GetScreenImage();
//	CvFont font;
//
//	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 1.0, 0, 2, CV_AA);
//
//
//	for(auto point : points) {
//		
//		cvPutText(screen, say, cvPoint(point.x, point.y - CONVERT_Y(-9, screen)), &font, color);
//		cvRectangle(screen, point, cvPoint(point.x + CONVERT_X(27, screen), point.y + 5 + CONVERT_Y(58, screen)), color, 2);
//
//	}
//
//}



NomalProduceScene::NomalProduceScene() {

	m_name = "NomalProduceScene";

	RESMGR->RegisterImage("img/ProduceScene/dance.jpg", "produce_dance");
	RESMGR->RegisterImage("img/ProduceScene/performance.jpg", "produce_performance");
	RESMGR->RegisterImage("img/ProduceScene/vocal.jpg", "produce_vocal");
	RESMGR->RegisterImage("img/ProduceScene/lesson.jpg", "produce_lesson");
	RESMGR->RegisterImage("img/ProduceScene/choose.jpg", "produce_choose");
	RESMGR->RegisterImage("img/ProduceScene/get_concert.jpg", "produce_get_concert");
	RESMGR->RegisterImage("img/ProduceScene/option_on.jpg", "produce_option_on");
	RESMGR->RegisterImage("img/ProduceScene/pause.jpg", "produce_pause");
}


NomalProduceScene::~NomalProduceScene() {
	if(m_prevImage != nullptr) {
		cvReleaseImage(&m_prevImage);
		SAFE_DELETE(m_prevImage);
	}
}


bool NomalProduceScene::CheckFirst() {

	if (m_prevImage != nullptr) return true;

	bool isScene = true;
	isSkiped = false;
	isPopUpActived = false;
	m_choose = 0;


	isScene = isScene && RESMGR->CheckRGB(nullptr, 1870, 920, 34, 170, 204);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1600, 200, 7, 72, 134);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 1870, 920, 18, 85, 102, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 40, 904, 18, 85, 111, 5);

	if (isScene) {
		isPopUpActived = true;
		return isScene;
	}

	isScene = RESMGR->CheckRGB(nullptr, 1570, 223, 6, 57, 110, 3);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1590, 550, 242, 238, 229, 5);

	return isScene;
}


bool NomalProduceScene::CheckScene() {

	if (m_prevImage != nullptr) {
		cvReleaseImage(&m_prevImage);
		SAFE_DELETE(m_prevImage);
	}

	auto points = RESMGR->FindImages(nullptr, "produce_lesson", 0.99, 1, true, cvRect(0, 0, 230, 200));
	if (points.empty() || points.size() > 1) return false;

	m_prevImage = (IplImage*)cvClone(GAME->GetScreenImage());

	return true;

}


bool NomalProduceScene::ReadData() {

	isEnterConcert = false;
	isWait = false;

	if(isPopUpActived) {
		ReadPopUp();
		return true;
	}

	//진행 중인 콘서트 확인
	auto concert = RESMGR->FindImages(nullptr, "produce_get_concert", 0.99, 1, true, cvRect(0, 205, 260, 240));
	if (!concert.empty() || RESMGR->CheckRGB(nullptr, 172, 286, 215, 7, 41, 15)) {
		
		//콘서트 정보가 없거나 당장 할일이 콘서트일 때
		if(PRODUCER->GetTodo<ConcertTodo>() == nullptr || PRODUCER->GetFirstTodo<ConcertTodo>() != nullptr) {
			isEnterConcert = true;
			return true;
		}

	}


	auto dance = RESMGR->FindImages(m_prevImage, "produce_dance", 0.99, 1, true, cvRect(0, 615, 1360, 80));
	auto performance = RESMGR->FindImages(m_prevImage, "produce_performance", 0.99, 1, true, cvRect(0, 615, 1360, 80));
	auto vocal = RESMGR->FindImages(m_prevImage, "produce_vocal", 0.99, 1, true, cvRect(0, 615, 1360, 80));
	bool isRoomEmpty = false;
	bool isChooseEmpty = false;

	auto choose = RESMGR->FindImages(m_prevImage, "produce_choose", 0.99, 1, true, cvRect(0, 945, 1676, 131));

	//선택이 없으면 스킵가능
	if (choose.empty()) {
		isChooseEmpty = true;
		isSkiped = true;
	}else {
		//선택이 있으면 인덱스를 찾아옴
		m_choose = GetChooseIndex(choose[0]);
	}


	//그 장소에 사람이 있으면
	if(!dance.empty() || !performance.empty() || !vocal.empty()) {
		

	} else {
		//사람이 없으면 다음 장소로 이동
		isRoomEmpty = true;
		if(m_choose >= 3) {
			m_choose = 0;
		}else {
			m_choose++;
		}
	}

	if(isRoomEmpty && isChooseEmpty) {
		isWait = true;
		return true;
	}

	//Print(dance, "dance");
	//Print(performance, "performance", CV_RGB(200, 200, 0));
	//Print(vocal, "vocal", CV_RGB(0, 0, 255));
	//Print(choose, "choose", CV_RGB(0, 0, 0));

	//cvShowImage("sample", GAME->GetScreenImage());
	//cvWaitKey(10);

	return true;

}


void NomalProduceScene::ActionDecision() {

	if (m_prevImage != nullptr) {
		cvReleaseImage(&m_prevImage);
		SAFE_DELETE(m_prevImage);
	}

	if (isWait) return;

	if(isPopUpActived) {
		GAME->SetMouseClick(m_popPos.x, m_popPos.y);
		return;
	}

	{
		Todo* todo = PRODUCER->GetTodo();

		if(todo == nullptr || isScene<MainScene>(todo->targetScene)) {
			if(PRODUCER->GetTodo<ProduceTodo>() == nullptr) {

				ProduceTodo* produce_todo = new ProduceTodo();
				produce_todo->targetScene = SCENE->GetScene<ResultProduceScene>();
				produce_todo->isStarted = true;
				PRODUCER->AddTodo(produce_todo);

				Todo* main_todo = new Todo();
				main_todo->targetScene = SCENE->GetScene<MainScene>();
				PRODUCER->AddTodo(main_todo);
			}

			GAME->SetMouseClick(1828, 78);
			return;
		}
	}

	if(isEnterConcert) {
		GAME->SetMouseClick(126, 306);
		return;
	}

	if(isSkiped) {
		GAME->SetMouseDown(10, 950, 1000);
		GAME->SetMouseUp(10, 950);
		GAME->SetMouseClick(10, 950);
		return;
	}

	CvPoint pos = GetChoosePos(m_choose);

	std::cout << "Try sending...\n";
	GAME->SetMouseClick(pos.x, pos.y);

}


void NomalProduceScene::ReadPopUp() {

	m_popPos = cvPoint(150, 920);

	//옵션 ON 스위치 확인 후 해제
	auto points = RESMGR->FindImages(nullptr, "produce_option_on", 0.99, 1, true, cvRect(1234, 770, 300, 220));
	if (!points.empty()) {
		m_popPos = cvPoint(1234 + points[0].x + 140, 770 + points[0].y + 60);
		return;
	}

	//현재 상태에 따라 프로듀스 일시정지 할지 확인
	points = RESMGR->FindImages(nullptr, "produce_pause", 0.99, 1, true, cvRect(733, 634, 530, 110));
	if (!points.empty()) {
		Todo* todo = PRODUCER->GetTodo();

		if (todo == nullptr || isScene<MainScene>(todo->targetScene)) {
			m_popPos = cvPoint(733 + points[0].x + 250, 634 + points[0].y + 60);
			return;
		}

		//프로듀스 메뉴에서 나가기
		m_popPos = cvPoint(1530, 310);
		return;
	}


}
