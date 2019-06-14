#include "ConcertUnitScene.h"
#include "ResMgr.h"
#include "ProducerAI.h"
#include "GameClientMgr.h"
#include "SceneMgr.h"


ConcertUnitScene::ConcertUnitScene() {
	
	m_name = "ConcertUnitScene";


	RESMGR->RegisterImage("img/ConcertUnitScene/dance_bt.jpg", "concert_unit_dance");
	RESMGR->RegisterImage("img/ConcertUnitScene/total.jpg", "concert_unit_total");

}


ConcertUnitScene::~ConcertUnitScene() {}


bool ConcertUnitScene::CheckFirst() {


	bool isScene = true;

	isScene = isScene && RESMGR->CheckRGB(nullptr, 31, 65, 255, 197, 0, 5);
	isScene = isScene && (RESMGR->CheckRGB(nullptr, 1860, 420, 18, 173, 219, 5) ||
		RESMGR->CheckRGB(nullptr, 1860, 570, 212, 181, 54, 5));

	return isScene;

}


bool ConcertUnitScene::CheckScene() {
	auto points = RESMGR->FindImages(nullptr, "concert_unit_dance", 0.97, 1, true, cvRect(1705, 150, 195, 150));
	if (points.empty()) return false;

	Sleep(2000);


	return true;

}


bool ConcertUnitScene::ReadData() {

	isReadyToRead = false;

	{
		auto points = RESMGR->FindImages(nullptr, "concert_unit_total", 0.97, 1, true, cvRect(463, 235, 117, 103));
		if(points.empty()) {
			return false;
		}

		isReadyToRead = true;
	}

	current_page = UNKOWN;
	SetCurrentAttribute(240, 187);

	EnsembleConcertUnit unit;
	std::string attribute_str = "UNKOWN";

	std::string teamStr[3];


	//GetNumber(0, 0, 1920, 1080);

	teamStr[0] = GetNumber(418, 473, 164, 39);
	unit.team[0] = std::stoi(teamStr[0]);

	teamStr[1] = GetNumber(418, 686, 164, 39);
	unit.team[1] = std::stoi(teamStr[1]);

	teamStr[2] = GetNumber(418, 895, 164, 39);
	unit.team[2] = std::stoi(teamStr[2]);

	unit.type = current_page;

	//������ �ִ��� Ȯ��
	if(unit.team[0] < unit.team[1] || unit.team[1] < unit.team[2] || unit.team[0] < unit.team[2]) {
		std::cout << "\n\n****�ν� ����!****\nȭ�� ũ�⸦ �� Ű�켼��.\n\n";
		return false;
	}

	switch (current_page) {
	case DANCE:
		PRODUCER->SetConcertDance(unit);
		attribute_str = "��";
		break;
	case PERFORMANCE:
		PRODUCER->SetConcertPerformance(unit);
		attribute_str = "�����ս�";
		break;
	case VOCAL:
		PRODUCER->SetConcertVocal(unit);
		attribute_str = "����";
		break;
	default:
		return false;
	}
	

	std::cout << "\n[" << attribute_str << " �� ����]" << "\n���� �� : " << teamStr[0] << "\n���� ��1 : " << teamStr[1] << "\n���� ��2 : " << teamStr[2] << "\n\n";
	
	
	return true;

}


void ConcertUnitScene::ActionDecision() {

	if (!isReadyToRead) return;

	if(PRODUCER->GetConcertDance().team[0] <= 0) {
		GAME->SetMouseClick(1800, 216);
		Sleep(1000);
		return;
	}

	if (PRODUCER->GetConcertVocal().team[0] <= 0) {
		GAME->SetMouseClick(1800, 365);
		Sleep(1000);
		return;
	}


	if (PRODUCER->GetConcertPerformance().team[0] <= 0) {
		GAME->SetMouseClick(1800, 524);
		Sleep(1000);
		return;
	}

	GAME->SetMouseClick(90, 77);
	SCENE->LockScene();

}


void ConcertUnitScene::SetCurrentAttribute(int x, int y) {

	//�Ӽ� ����
	//vocal
	if (RESMGR->CheckRGB(nullptr, x, y, 75, 190, 221, 4)) {
		current_page = VOCAL;
	}
	//dance
	else if (RESMGR->CheckRGB(nullptr, x, y, 234, 58, 71, 4)) {
		current_page = DANCE;
	}
	//performance
	else if (RESMGR->CheckRGB(nullptr, x, y, 239, 182, 49, 4)) {
		current_page = PERFORMANCE;
	}

}



std::string ConcertUnitScene::GetNumber(int x, int y, int width, int height) {

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



		//dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		//cvCvtColor(img, dst, CV_RGB2GRAY);
		cvSplit(img, dst, 0, 0, 0);
		REALLOC(img, dst);

		cv::Mat leveled = cv::cvarrToMat(img);
		leveled = RESMGR->gammaCorrection(leveled, 4.5);


		//cvShowImage("sample", img);
		//cvWaitKey();

		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		IplImage dump_img = leveled;
		cvCopy(&dump_img, dst);
		// dst = (IplImage*)cvClone(&IplImage(leveled));
		REALLOC(img, dst);

		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		cvDilate(img, dst, nullptr, 3);
		REALLOC(img, dst);

		leveled = cv::cvarrToMat(img);
		//leveled = RESMGR->gammaCorrection(leveled, 2);
		leveled = RESMGR->basicLinearTransform(leveled, 1.8, 0);
		// dst = (IplImage*)cvClone(&IplImage(leveled));
		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		dump_img = leveled;
		cvCopy(&dump_img, dst);
		REALLOC(img, dst);


		//cvShowImage("sample", img);
		//cvWaitKey();



		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		int color = 230;
		CvScalar scalar_min = cvScalar(0, 0, 0);
		CvScalar scalar_max = cvScalar(color, color, color);

		cvInRangeS(img, scalar_min, scalar_max, dst);
		REALLOC(img, dst);

		dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

		cvErode(img, dst, nullptr, 2);
		REALLOC(img, dst);

	}
	//cvShowImage("sample", img);
	//cvWaitKey();
	std::string str = RESMGR->Image2String(img, 20, 51);
	//cvWaitKey();

	cvReleaseImage(&img);

	return str;

}