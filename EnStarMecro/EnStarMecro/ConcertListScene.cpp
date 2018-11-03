#include "ConcertListScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "ConcertTodo.h"


ConcertListScene::ConcertListScene() {
	m_name = "ConcertListScene";


	RESMGR->RegisterImage("img/ConcertScene/event_rule.jpg", "concert_event_rule");
	RESMGR->RegisterImage("img/ConcertScene/concert_list_yellow.jpg", "concert_list_yellow");
	RESMGR->RegisterImage("img/ConcertScene/get_all.jpg", "concert_list_get_all");
	RESMGR->RegisterImage("img/ConcertScene/sub_story_open.jpg", "concert_sub_story_open");
	RESMGR->RegisterImage("img/ConcertScene/rank_reward.jpg", "concert_rank_reward");
	RESMGR->RegisterImage("img/ConcertScene/bt_get.jpg", "concert_bt_get");
}


ConcertListScene::~ConcertListScene() {}


bool ConcertListScene::CheckFirst() {

	bool isScene = true;
	m_state = NONE;
	isWait = false;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 31, 65, 255, 197, 0, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 433, 958, 228, 180, 42, 5);

	if (isScene) {
		m_state = LIST;
		return isScene;
	}

	isScene = RESMGR->CheckRGB(nullptr, 31, 65, 255, 197, 0, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1035, 221, 222, 72, 73, 5);

	if (isScene) {
		m_state = INTRO;
		return isScene;
	}

	isScene = RESMGR->CheckRGB(nullptr, 368, 67, 228, 181, 43, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1740, 200, 224, 174, 41, 5);

	if (isScene) {
		m_state = POINT_REWARD;
		return isScene;
	}

	isScene = RESMGR->CheckRGB(nullptr, 760, 90, 248, 242, 229, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1740, 351, 224, 174, 41, 5);

	if (isScene) {
		m_state = RANK_REWARD;
		return isScene;
	}

	isScene = RESMGR->CheckRGB(nullptr, 900, 850, 246, 246, 246, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1000, 850, 228, 181, 43, 5);

	if (isScene) {
		m_state = SUB_STROY;
	}

	return isScene;

}


bool ConcertListScene::CheckScene() {

	if(m_state == INTRO) {
		auto points = RESMGR->FindImages(nullptr, "concert_event_rule", 0.99, 1, true, cvRect(1003, 23, 284, 119));
		if (points.empty() || points.size() > 1) return false;

		Sleep(1000);


		return true;
	}

	if(m_state == POINT_REWARD) {
		auto points = RESMGR->FindImages(nullptr, "concert_list_get_all", 0.99, 1, true, cvRect(350, 40, 190, 90));
		if (points.empty()) return false;

		return true;
	}

	if (m_state == RANK_REWARD) {
		auto points = RESMGR->FindImages(nullptr, "concert_rank_reward", 0.99, 1, true, cvRect(750, 180, 213, 70));
		if (points.empty()) return false;

		return true;
	}

	if(m_state == SUB_STROY) {
		auto points = RESMGR->FindImages(nullptr, "concert_sub_story_open", 0.98, 1, true, cvRect(538, 57, 886, 412));
		if (points.empty()) return false;

		return true;
	}

	auto points = RESMGR->FindImages(nullptr, "concert_list_yellow", 0.99, 1, true, cvRect(210, 932, 259, 87));
	if (points.empty() || points.size() > 1) return false;

	Sleep(1000);


	return true;

}


bool ConcertListScene::ReadData() {

	isQuit = false;

	if(m_state == INTRO) {
		AddPointRewardTodo();
		return true;
	}

	if(m_state == SUB_STROY || m_state == POINT_REWARD) {
		return true;
	}

	if(m_state == RANK_REWARD) {
		auto points = RESMGR->FindImages(nullptr, "concert_bt_get", 0.98, 1, true, cvRect(1098, 260, 252, 769));
		if (points.empty()) {
			isQuit = true;
			#ifdef WIN32
			{
				SYSTEMTIME oTime;
				GetLocalTime(&oTime);
				std::stringstream str;
				str << "img/Saved/EVENT_RESULT_" << (int)oTime.wYear << (int)oTime.wMonth << (int)oTime.wDay << ".jpg";
				cvSaveImage(str.str().c_str(), GAME->GetScreenImage());
				std::cout << str.str() << " is " << "Saved." << std::endl;
			}
			#endif
			return true;
		}

		m_point = cvPoint(points[0].x + 1098, points[0].y + 260);

		return true;
	}

	if (!RESMGR->CheckRGB(nullptr, 1150, 430, 6, 58, 110, 5)) {
		auto _todo = PRODUCER->GetTodo<ConcertTodo>();

		if(_todo != nullptr)
			PRODUCER->RemoveTodo(_todo);

		isQuit = true;
		return true;
	}

	if(PRODUCER->GetTodoSize() == 0) {
		isQuit = true;
		return true;
	}

	ConcertTodo* todo = PRODUCER->GetFirstTodo<ConcertTodo>();

	//������ ����
	if(PRODUCER->GetTodo<ConcertTodo>() == nullptr) {
		AddConcertTodo();
		std::cout << "add todo\n";
		todo = PRODUCER->GetFirstTodo<ConcertTodo>();
	}

	if(todo == nullptr) {
		isQuit = true;
		std::cout << "NULLPTR todo\n";
		return true;
	}

	if(!todo->isAvailable()){
		isQuit = true;
		return true;
	}
	if(todo->totalAudience == -1) {

		std::string totalAudience_str = GetNumber(1245, 305, 116, 33);
		todo->totalAudience = std::stoi(totalAudience_str);
		std::cout << "totalAudience : " << todo->totalAudience << std::endl;

		std::string concert_achieveTime_str = GetNumber(1306, 346, 87, 36);
		int achieve_second = std::stoi(concert_achieveTime_str.substr(concert_achieveTime_str.size() - 2));
		int achieve_minute = std::stoi(concert_achieveTime_str.substr(0, 2));


		if (achieve_second <= 59 && achieve_minute <= 59)
			todo->achieveTime = GAME->GetUpdatedTime() + ProducerAI::GetMillisecond(achieve_minute, achieve_second);

	}


	return true;
}


void ConcertListScene::ActionDecision() {


	if(isQuit) {
		GAME->SetMouseClick(80, 70);
		SCENE->LockScene();
		return;
	}

	if(m_state == INTRO) {

		if (PRODUCER->GetFirstTodo<ConcertTodo>() != nullptr) {
			GAME->SetMouseClick(690, 960);
			return;
		}

		Todo* todo = PRODUCER->GetTodo();
		if (todo != nullptr && todo->todo_str == "point_reward") {
			GAME->SetMouseClick(1628, 645);
			return;
		}

		if(RESMGR->CheckRGB(nullptr, 1712, 238, 222, 33, 53, 5)) {
			GAME->SetMouseClick(1630, 286);
			return;
		}

		GAME->SetMouseClick(80, 70);
		return;
	}

	if (m_state == POINT_REWARD) {
		Todo* todo = PRODUCER->GetTodo();
		if (todo != nullptr && todo->todo_str == "point_reward") {
			PRODUCER->RemoveTodo(todo);
			GAME->SetMouseClick(440, 84);
			return;
		}

		GAME->SetMouseClick(80, 70);
		Sleep(1000);

		return;
	}

	if(m_state == RANK_REWARD) {
		GAME->SetMouseClick(m_point.x, m_point.y);
		return;
	}

	if(m_state == SUB_STROY) {
		GAME->SetMouseClick(762, 886);
		return;
	}

	if (isWait) return;

	if (PRODUCER->GetFirstTodo<ConcertTodo>() != nullptr) {
		GAME->SetMouseClick(1500, 320);
		return;
	}

	GAME->SetMouseClick(80, 70);
	SCENE->LockScene();

}


std::string ConcertListScene::GetNumber(int x, int y, int width, int height) {

	IplImage* img = (IplImage*)cvClone(GAME->GetScreenImage());

	CUT_IMAGE(img, cvRect(x, y, width, height));

	//if (isDilate) {
	//	IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	//	cvDilate(img, dst);

	//	REALLOC(img, dst);
	//}

	//if (isErode) {
	//	IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
	//	cvErode(img, dst);

	//	REALLOC(img, dst);
	//}

	RESIZE_IMAGE(img, cvSize(img->width * 4, img->height * 4));

	{
		IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		cvCvtColor(img, dst, CV_RGB2GRAY);
		REALLOC(img, dst);

		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		int color = 130;
		CvScalar scalar_min = cvScalar(0, 0, 0);
		CvScalar scalar_max = cvScalar(color, color, color);

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


void ConcertListScene::AddConcertTodo() {

	if(!RESMGR->CheckRGB(nullptr, 1150, 430, 6, 58, 110, 5)) {
		return;
	}

	ConcertTodo* _todo = new ConcertTodo();
	PRODUCER->AddTodo(_todo);
	_todo->targetScene = SCENE->GetScene<ConcertResultScene>();
	_todo->todo_str = "concert";

	std::string concert_achieveTime_str = GetNumber(1306, 346, 87, 36);

	if (concert_achieveTime_str.size() >= 4) {

		int achieve_second = std::stoi(concert_achieveTime_str.substr(concert_achieveTime_str.size() - 2));
		int achieve_minute = std::stoi(concert_achieveTime_str.substr(0, 2));

		if (achieve_second <= 59 && achieve_minute <= 59)
			_todo->achieveTime = GAME->GetUpdatedTime() + ProducerAI::GetMillisecond(achieve_minute, achieve_second);
		else
			isWait = true;
	}

}


void ConcertListScene::AddPointRewardTodo() {

	if (!RESMGR->CheckRGB(nullptr, 1700, 570, 222, 33, 53, 5)) {
		return;
	}

	Todo* todo = new Todo();
	todo->targetScene = this;
	todo->todo_str = "point_reward";

	PRODUCER->AddTodo(todo);
}
