#include "MainScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "ProducerAI.h"
#include "ProduceTodo.h"
#include "ConcertTodo.h"

#ifdef WIN32
#define popen _popen
#define pclose _pclose
#endif

MainScene::MainScene() {

	m_name = "MainScene";


	RESMGR->RegisterImage("img/MainScene/bt_menu.jpg", "main_bt_menu");
	RESMGR->RegisterImage("img/MainScene/pop_menu.jpg", "main_pop_menu");
	RESMGR->RegisterImage("img/MainScene/pop_lp.jpg", "main_pop_lp");
	RESMGR->RegisterImage("img/MainScene/pop_info.jpg", "main_pop_info");
	RESMGR->RegisterImage("img/MainScene/pop_contact.jpg", "main_pop_contact");
	RESMGR->RegisterImage("img/MainScene/pop_giftbox.jpg", "main_pop_giftbox");
	RESMGR->RegisterImage("img/MainScene/pop_check_item.jpg", "main_pop_check_item");
	RESMGR->RegisterImage("img/MainScene/pop_close.jpg", "main_pop_close");
	RESMGR->RegisterImage("img/MainScene/pop_menu_concert.jpg", "main_pop_menu_concert");
	RESMGR->RegisterImage("img/MainScene/pop_menu_storage.jpg", "main_pop_menu_storage");
	RESMGR->RegisterImage("img/MainScene/pop_reward.jpg", "main_pop_reward");
	RESMGR->RegisterImage("img/MainScene/pop_login_gift.jpg", "main_pop_login_gift");
	RESMGR->RegisterImage("img/MainScene/reward_check.jpg", "main_pop_reward_check");
	RESMGR->RegisterImage("img/MainScene/quest_script.jpg", "main_quest_script");
}


MainScene::~MainScene() {
	
}


bool MainScene::CheckFirst() {

	bool isScene = true;
	isPopUp = false;
	isHide = false;


	isScene = isScene && RESMGR->CheckRGB(nullptr, 65, 411, 206, 65, 107, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 80, 973, 220, 220, 220, 5);

	if (isScene) return isScene;

	isScene = RESMGR->CheckRGB(nullptr, 1778, 67, 65, 158, 217, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 1471, 603, 248, 218, 24, 5);


	if (isScene) { isHide = true; return isScene; }

	isScene = RESMGR->CheckRGB(nullptr, 65, 411, 105, 33, 53, 5);
	isScene = isScene && RESMGR->CheckRGB(nullptr, 80, 973, 110, 110, 110, 5);

	if (isScene) isPopUp = true;

	return isScene;

}


bool MainScene::CheckScene() {

	auto points = RESMGR->FindImages(nullptr, "main_bt_menu", 0.92, 1, true, cvRect(1710, 900, 192, 189));
	if (points.empty() || points.size() > 1) return false;

	return true;

}


bool MainScene::ReadData() {

	m_pos = cvPoint(0, 0);

//======================================================================================
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
//======================================================================================

	if (isPopUp) {
		ReadPopUp();
		return true;
	}

	long currentTime = timeGetTime();

	{
		Todo* todo = PRODUCER->GetTodo();
		if(todo != nullptr && isScene<MainScene>(todo->targetScene)) {
			PRODUCER->RemoveTodo(todo);
		}
	}


	//rank
	//std::string rank_str = GetNumber(995, 58, 102, 65, true, false);
	//int rank = std::stoi(rank_str);
	int rank = 99;
	PRODUCER->SetRank(rank);

	//ap
	std::string ap_str = GetNumber(1254, 30, 152, 38);
	int ap_slash;

	std::string ap_cooltime_str = GetNumber(1425, 20, 88, 30, false, false, 190, 255, 3);

	//46랭크 이후로 ap최대치가 3자리수
	//랭크가 46 미만이라면
	if(rank < 46) {
		ap_slash = ap_str.size() - 3;
	}else {
		ap_slash = ap_str.size() - 4;
	}


	if((ap_slash = ap_str.find("/")) != std::string::npos) {

		EnsembleUnit ap;

		ap.max = std::stoi(ap_str.substr(ap_slash + 1));
		ap.current = std::stoi(ap_str.substr(0, ap_slash));
		ap.achieveTime = PRODUCER->GetAP().achieveTime;

		if (ap.max - ap.current <= 0) {

			ap.achieveTime = GAME->GetUpdatedTime();

		} else if (ap_cooltime_str.size() >= 4) {

			int ap_cool_second = std::stoi(ap_cooltime_str.substr(ap_cooltime_str.size() - 2));
			int ap_cool_minute = std::stoi(ap_cooltime_str.substr(0, 2));

			if (ap_cool_minute <= 3 && ap_cool_second <= 59)
				ap.achieveTime = GAME->GetUpdatedTime() + ProducerAI::GetMillisecond(ap_cool_minute, ap_cool_second);
		}

		PRODUCER->SetAP(ap);
	}


	//lp
	std::string lp_str = GetNumber(1660, 30, 103, 38);
	int lp_slash;
	bool isLPOverSet = false;


	std::string lp_cooltime_str = GetNumber(1785, 20, 88, 30, false, false, 190, 255, 3);



	if((lp_slash = lp_str.find("/")) != std::string::npos) {
		EnsembleUnit lp;

		lp.max = std::stoi(lp_str.substr(lp_slash + 1));
		lp.current = std::stoi(lp_str.substr(0, lp_slash));
		lp.achieveTime = PRODUCER->GetLP().achieveTime;

		if (lp.max - lp.current <= 0 || isLPOverSet) {

			lp.achieveTime = GAME->GetUpdatedTime();

			if(isLPOverSet) {
				lp.current = lp.max + 1;
			}

		} else if (lp_cooltime_str.size() >= 4) {

			int lp_cool_second = std::stoi(lp_cooltime_str.substr(lp_cooltime_str.size() - 2));
			int lp_cool_minute = std::stoi(lp_cooltime_str.substr(0, 2));

			if (lp_cool_minute <= 30 && lp_cool_second <= 59)
				lp.achieveTime = GAME->GetUpdatedTime() + ProducerAI::GetMillisecond(lp_cool_minute, lp_cool_second);
		}


		PRODUCER->SetLP(lp);
	}

	auto ap = PRODUCER->GetAP();
	auto lp = PRODUCER->GetLP();

	long ap_remain = ap.achieveTime - currentTime;
	long lp_remain = lp.achieveTime - currentTime;

	std::cout << "===============\n[Rank] " << PRODUCER->GetRank()
		<< "\n[AP] " << ap.current << " / " << ap.max << "\t" << ProducerAI::Millisecond2Min(ap_remain) << "분 " << ProducerAI::Millisecond2Second(ap_remain) << "초 후 갱신"
		<< "\n[LP] " << lp.current << " / " << lp.max << "\t" << ProducerAI::Millisecond2Min(lp_remain) << "분 " << ProducerAI::Millisecond2Second(lp_remain) << "초 후 갱신"
		<< "\n===============\n";


	//======================================================
	//아이템 및 일부 갱신사항 갱신

	//경험치 갱신 요망
	if (PRODUCER->GetTotalEXP() <= 0 || PRODUCER->GetIsChacked() != 198294) {
		m_pos = cvPoint(1050, 90);
		return true;
	}

	//콘서트 보상 갱신
	if (RESMGR->CheckRGB(nullptr, 1806, 276, 221, 34, 53, 7)) {
		m_pos = cvPoint(1482, 385);
		return true;
	}

	//선물함 갱신 요망
	if (RESMGR->CheckRGB(nullptr, 145, 340, 223, 31, 52, 7)) {
		m_pos = cvPoint(83, 393);
		return true;
	}

	//미션 갱신 요망
	if (RESMGR->CheckRGB(nullptr, 1570, 608, 223, 33, 53, 7)) {

		auto points = RESMGR->FindImages(nullptr, "main_quest_script", 0.97, 1, true, cvRect(450, 744, 270, 124));
		if (!points.empty()) {
			m_pos = cvPoint(800, 775);
			SCENE->LockScene();
			return true;
		}
	}

	//�޴� ���� ���



	AddTodo();


	//�ܼ�Ʈ�� �ؾ��Ѵٸ�
	if (PRODUCER->GetFirstTodo<ConcertTodo>() != nullptr) {
		m_pos = cvPoint(1807, 967);
		return true;

	}

	Todo* todo = PRODUCER->GetTodo();

	if(todo != nullptr) {
		Scene* target = todo->targetScene;

		if (target == nullptr) return true;
		// ���εེ�� �ؾ��Ѵٸ�
		if (target->GetName().find("Produce") != std::string::npos) {
			m_pos = cvPoint(PRODUCE_POS.x, PRODUCE_POS.y);
			SCENE->LockScene();
			return true;
		}
	}

	//���� �̺�Ʈ ������ ��� - lp
	if (PRODUCER->GetStatus() != ProducerAI::NOMAL && lp.current <= lp.max / 2) {
		if (PRODUCER->GetTodo<CoverTodo>() == nullptr) {
			CoverTodo* todo_cover = new CoverTodo();
			todo_cover->targetScene = SCENE->GetScene<MainScene>();
			PRODUCER->AddTodo(todo_cover);
			m_pos = cvPoint(1700, 41);
			SCENE->LockScene();
			return true;
		}
	}

	//���� �̺�Ʈ ������ ��� - ap
	if (ap.current < 30) {
		if (PRODUCER->GetTodo<CoverTodo>() == nullptr) {
			CoverTodo* todo_cover = new CoverTodo();
			todo_cover->targetScene = SCENE->GetScene<MainScene>();
			PRODUCER->AddTodo(todo_cover);
			m_pos = cvPoint(1500, 41);
			SCENE->LockScene();
			return true;
		}
	}



	return true;
}


void MainScene::ActionDecision() {

	if(isPopUp) {
		ActionPopUp();
		return;
	}

	if(m_pos.x > 0 && m_pos.y > 0) {
		GAME->SetMouseClick(m_pos.x, m_pos.y);
		return;
	}

	if (PRODUCER->GetIsChacked() != 198294) return;

	//AddTodo();

	Todo* todo = PRODUCER->GetTodo();

	if (todo == nullptr) {
		//��Ÿ�ӿ� ���� ��ũ�� ����� �Ͻ� �ߴ���
		auto ap = PRODUCER->GetAP();
		auto lp = PRODUCER->GetLP();

		long ap_remain = ap.achieveTime - static_cast<long>(timeGetTime());
		long lp_remain = lp.achieveTime - static_cast<long>(timeGetTime());

		unsigned long remain_second;

		if(ap_remain >= 0 && ProducerAI::Millisecond2Second(ap_remain) < ProducerAI::Millisecond2Second(lp_remain)) {
			remain_second = ProducerAI::GetMillisecond(0, ProducerAI::Millisecond2Second(ap_remain));
		}else if(lp_remain >= 0){
			remain_second = ProducerAI::GetMillisecond(0, ProducerAI::Millisecond2Second(lp_remain));
		}else {
			remain_second = 0;
		}

		GAME->SetMouseClick(595, 210);
		Sleep(remain_second);

		return;
	}

	////�ܼ�Ʈ�� �ؾ��Ѵٸ�
	//if (PRODUCER->GetFirstTodo<ConcertTodo>() != nullptr) {
	//	GAME->SetMouseClick(1807, 967);
	//	return;

	//}

	//Scene* target = todo->targetScene;

	//if (target == nullptr) return;
	//// ���εེ�� �ؾ��Ѵٸ�
	//if(target->GetName().find("Produce") != std::string::npos) {
	//	GAME->SetMouseClick(PRODUCE_POS.x, PRODUCE_POS.y);
	//	SCENE->LockScene();
	//}

	


	//Sleep(1000);

}


void MainScene::ReadPopUp() {

	std::cout << "PopUp!\n";

	CvPoint cancel_pos = cvPoint(0, 0);
	Todo* todo = PRODUCER->GetTodo();

	auto points = RESMGR->FindImages(nullptr, "main_pop_close", 0.98, 1, true, cvRect(1465, 45, 334, 206));
	//cancel button
	if (!points.empty()) {
		std::cout << "닫기 버튼이 존재함.\n";
		cancel_pos = cvPoint(1465 + points[0].x + 40, 45 + points[0].y + 40);

	}

	points.clear();
	points = RESMGR->FindImages(nullptr, "main_pop_menu", 0.98, 1, true, cvRect(756, 92, 411, 165));
	//menu
	if (!points.empty()) {

		std::cout << "[메뉴]\n";

		if(todo == nullptr) {
			m_pos = cancel_pos;
			return;
		}

		if (isScene<ConcertResultScene>(todo->targetScene)) {
			//콘서트 확인

			auto concert_icon = RESMGR->FindImages(nullptr, "main_pop_menu_concert", 0.95, 1, true, cvRect(455, 276, 706, 652));
			//concert icon
			if (concert_icon.empty()) {
				std::cout << "콘서트 버튼이 보이지 않음 스크롤 수정.\n";
				m_pos = cvPoint(1525, 610);
				return;
			}
			std::cout << "콘서트 버튼 찾음.\n";
			m_pos = cvPoint(455 + concert_icon[0].x + 50, 276 + concert_icon[0].y + 50);
			SCENE->LockScene();
			return;
		}

		//스토리지 관련도 추가 예정

		m_pos = cancel_pos;
		return;
	}

	points = RESMGR->FindImages(nullptr, "main_pop_info", 0.98, 1, true, cvRect(756, 92, 411, 165));
	//infomation
	if (!points.empty()) {

		std::cout << "[전학생 서류]\n";

		//현재 액티비티 확인
		//EasyVPN : com.easyovpn.easyovpn
		//OpenVPN : net.openvpn.openvpn
		//마켓 : com.android.vending
#ifdef WIN32
		FILE* fpipe = popen("adb\\adb shell \"dumpsys window windows | grep -E \'mCurrentFocus|mFocusedApp\'\"", "r");
#elif __linux__
		FILE* fpipe = popen("adb shell \"dumpsys window windows | grep -E \'mCurrentFocus|mFocusedApp\'\"", "r");
#endif
		if (fpipe == NULL)
			std::cout << "\nadb is not available.\n";

		char adb_result_str[1024] = {};
		std::string adb_result;

		fread(adb_result_str, 1024, 1, fpipe);
		adb_result = adb_result_str;

		pclose(fpipe);

		//앙스타 앱인지 확인
		if(adb_result.find("com.kakaogames.estarskr/com.happyelements.kirara.KakaoActivity") == std::string::npos) {
			std::cout << "\n[앙스타가 감지가 되지 않음]\n\n";
			system("adb\\adb shell am start -n com.kakaogames.estarskr/com.happyelements.kirara.KakaoActivity");
			return;
		}

		//유저 확인
		std::string uid_str = GetNumber(436, 760, 250, 45, false, false, 0, 180, 4);
		std::string userNumber_str = GetNumber(1039, 760, 250, 45, false, false, 0, 180, 4);


		if(uid_str != "1000065003" || userNumber_str != "748421995247") {
			std::cout << "\n\n[본인 확인 실패]\n\n본인의 계정인지 확인하여 주십시오.\n\n";
			return;
		}

		std::cout << "\n[본인 확인 완료]\n\n";
		PRODUCER->SetIsChacked(198294);

		//인포 확인
		std::string exp_str = GetNumber(968, 520, 142, 40, false, false, 0, 90, 4);
		int exp_slash = -1;
		if ((exp_slash = exp_str.find("/")) != std::string::npos) {

			int exp_max = std::stoi(exp_str.substr(exp_slash + 1));
			int exp_current = std::stoi(exp_str.substr(0, exp_slash));

			PRODUCER->SetRank(-1, exp_current, exp_max);
		}


		m_pos = cancel_pos;
		return;
	}

	points = RESMGR->FindImages(nullptr, "main_pop_contact", 0.98, 1, true, cvRect(756, 92, 411, 165));
	//contact
	if (!points.empty()) {

		std::cout << "[contact]\n";

		//차후 추가

		m_pos = cancel_pos;
		return;
	}

	points = RESMGR->FindImages(nullptr, "main_pop_giftbox", 0.98, 1, true, cvRect(756, 92, 411, 165));
	//giftbox
	if (!points.empty()) {

		std::cout << "[선물함]\n";

		m_pos = cvPoint(1194, 920);
		return;
	}

	points = RESMGR->FindImages(nullptr, "main_pop_check_item", 0.98, 1, true, cvRect(756, 92, 411, 165));
	//check_item
	if (!points.empty()) {

		std::cout << "[아이템 확인]\n";

		m_pos = cvPoint(957, 904);
		return;
	}

	points = RESMGR->FindImages(nullptr, "main_pop_reward", 0.98, 1, true, cvRect(617, 0, 700, 192));
	//reward
	if (!points.empty()) {

		std::cout << "[유메노사키 학원 출석표]\n";

		if(PRODUCER->b_isGetGift) {
			GAME->SetMouseClick(cancel_pos.x, cancel_pos.y);
		}

		points = RESMGR->FindImages(nullptr, "main_pop_reward_check", 0.99, 18, true, cvRect(330, 288, 1130, 627));
		if(points.empty()) {
			//m_pos = cvPoint(420, 376);
			GAME->SetMouseClick(420, 376);
			Sleep(1000);
			// GAME->SendAdbCommand("adb shell input keyevent KEYCODE_BACK");
			Sleep(1000);

			PRODUCER->b_isGetGift = true;

			return;

			//CvPoint point = cvPoint(420, 376);
			//cvRectangle(GAME->GetScreenImage(), point, cvPoint(point.x + 20, point.y + 20), CV_RGB(255, 0, 0), 4);
			//cvShowImage("sample", GAME->GetScreenImage());
			//cvWaitKey();
			//return;
		}

		//첫번째 체크표시
		CvPoint point_first = cvPoint(0, 0);
		int min_x = 999;
		int min_y = 999;
		for(auto point : points) {
			if(point.x + point.y <= min_x + min_y) {
				point_first = cvPoint(330 + points[0].x + 76, 288 + points[0].y + 32);
			}
		}
		int y = points.size() / 6;
		int x = points.size() - y * 6;



		GAME->SetMouseClick(point_first.x + x * 210, point_first.y + y * 210);
		Sleep(1000);
		GAME->SendAdbCommand("adb shell input keyevent KEYCODE_BACK");

		PRODUCER->b_isGetGift = true;


		//m_pos = cvPoint(420, 376);

		//CvPoint point = cvPoint(point_first.x + x * 210, point_first.y + y * 210);
		//cvRectangle(GAME->GetScreenImage(), point, cvPoint(point.x + 20, point.y + 20), CV_RGB(255, 0, 0), 4);
		//cvShowImage("sample", GAME->GetScreenImage());
		//cvWaitKey();
		return;
	}

	points = RESMGR->FindImages(nullptr, "main_pop_login_gift", 0.98, 1, true, cvRect(763, 219, 411, 165));
	//login gift
	if (!points.empty()) {

		std::cout << "[로그인 보상]\n";

		m_pos = cvPoint(959, 740);
		return;
	}


	std::cout << "[unkown]\n";
	m_pos = cancel_pos;

	if(cancel_pos.x + cancel_pos.y <= 0) {
		SetSkiped(true);
	}

	GAME->SendAdbCommand("adb shell input keyevent KEYCODE_BACK");

	return;
}


void MainScene::ActionPopUp() {

	GAME->SetMouseClick(m_pos.x, m_pos.y);
	Sleep(500);
}


std::string MainScene::GetNumber(int x, int y, int width, int height, bool isDilate, bool isErode, int minScalar, int maxScalar, int scale) {

	IplImage* img = (IplImage*)cvClone(GAME->GetScreenImage());

	CUT_IMAGE(img, cvRect(x, y, width, height));

	if(isDilate) {
		IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
		cvDilate(img, dst);

		REALLOC(img, dst);
	}

	if (isErode) {
		IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
		cvErode(img, dst);

		REALLOC(img, dst);
	}

	RESIZE_IMAGE(img, cvSize(img->width * scale, img->height * scale));

	{
		IplImage* dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		cvCvtColor(img, dst, CV_RGB2GRAY);
		REALLOC(img, dst);

		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		CvScalar scalar_min = cvScalar(minScalar, minScalar, minScalar);
		CvScalar scalar_max = cvScalar(maxScalar, maxScalar, maxScalar);

		cvInRangeS(img, scalar_min, scalar_max, dst);
		REALLOC(img, dst);
	}

	//cvShowImage("����", img);
	//cvWaitKey();

	std::string str = RESMGR->Image2String(img, 20 * scale);
	cvReleaseImage(&img);

	return str;

}


std::string MainScene::GetNumber(int x, int y, int width, int height) {

	IplImage* img = (IplImage*)cvClone(GAME->GetScreenImage());

	CUT_IMAGE(img, cvRect(x, y, width, height));

	RESIZE_IMAGE(img, cvSize(img->width * 2, img->height * 2));
	IplImage* img_over = (IplImage*)cvClone(img);

	//cvShowImage("����", img);
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

	//cvShowImage("����", img);
	//cvWaitKey();

	std::string str = RESMGR->Image2String(img);
	//cvWaitKey();

	cvReleaseImage(&img);
	cvReleaseImage(&img_over);

	return str;

}


void MainScene::AddTodo() const {

	if (PRODUCER->GetTodo() == nullptr && PRODUCER->GetTodo<ProduceTodo>() == nullptr) {

		int remainBlankAP = PRODUCER->GetAP().max - PRODUCER->GetAP().current;
		float AP_persent = (float) PRODUCER->GetAP().max / PRODUCER->GetAP().current;

		if(PRODUCER->GetTodo<ConcertTodo>() != nullptr && AP_persent < 0.5f) {
			return;
		}

		switch (PRODUCER->GetStatus()) {

		case ProducerAI::NOMAL:
		{
			ProduceTodo* todo = new ProduceTodo();
			todo->type = ProduceTodo::DAILY;

			PRODUCER->PushBackTodo(todo);
		}
		break;
		case ProducerAI::EVENT_LIGHT:
		case ProducerAI::EVENT_NOMAL:
		case ProducerAI::EVENT_IMPORTANT:
		{
			ProduceTodo* todo = new ProduceTodo();
			todo->type = ProduceTodo::LIMIT;

			PRODUCER->PushBackTodo(todo);
		}
		break;
		//case ProducerAI::EVENT_IMPORTANT: 
		//{
		//
		//	ProduceTodo todo = ProduceTodo();

		//	int currentLP = PRODUCER->GetLP().current;
		//	
		//}
		//
		//break;
		default: break;
		}
	}

}
