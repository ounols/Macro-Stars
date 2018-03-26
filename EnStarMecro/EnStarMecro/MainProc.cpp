#include "MainProc.h"
#include "ProducerAI.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "NomalProduceScene.h"
#include "SceneMgr.h"
#include "ProduceTodo.h"
#include "ConcertTodo.h"
#include "UnkownScene.h"


MainProc::MainProc() {
	GAME->GetGameHWND();
	RESMGR->TrainKNN("img/KNN/classifications.xml", "img/KNN/images.xml");


	//ProduceTodo todo = ProduceTodo();
	//todo.type = ProduceTodo::DAILY;

	//PRODUCER->AddTodo(static_cast<Todo>(todo));

	//�ӽ�
	//namedWindow("sample", cv::WINDOW_GUI_NORMAL);
	//namedWindow("����", cv::WINDOW_AUTOSIZE);
	PRODUCER->SetStatus(ProducerAI::NOMAL);

	//ProduceTodo* todo = new ProduceTodo();
	//todo->type = ProduceTodo::LIMIT;

	//PRODUCER->AddTodo(todo);

	//ConcertTodo* todo = new ConcertTodo();
	//todo->totalAudience = 500000;
	//todo->achieveTime = timeGetTime() + ProducerAI::GetMillisecond(36, 13);
	//todo->type = ConcertTodo::BIG;

	//PRODUCER->AddTodo(todo);

	//EnsembleUnit lp;
	//lp.current = 11;
	//lp.max = 10;
	//lp.achieveTime = timeGetTime() + ProducerAI::GetMillisecond(0, 0);

	//PRODUCER->SetLP(lp);


	//EnsembleConcertUnit per;

	//per.type = DANCE;
	//per.team[0] = 104882;
	//per.team[1] = 58412;
	//per.team[2] = 50248;

	//PRODUCER->SetConcertDance(per);

	//EnsembleUnit ap;

	//ap.max = 100;
	//ap.current = 100;

	//PRODUCER->SetAP(ap);
}


MainProc::~MainProc() {
	ProducerAI::delInstance();
	ResMgr::delInstance();
	GameClientMgr::delInstance();
	SceneMgr::delInstance();

	cvDestroyAllWindows();
}


void MainProc::Update() {

	if(GetAsyncKeyState(VK_ESCAPE) && GetAsyncKeyState(VK_BACK)) {
		b_isQuit = true;
		return;
	}


	if(GetAsyncKeyState(VK_END)) {
		b_isPause = !b_isPause;
	}

	if (GetAsyncKeyState(VK_F11)) {
		SYSTEMTIME oTime;
		GetLocalTime(&oTime);
		std::stringstream str;
		str << "img/Saved/" << (int)oTime.wYear << (int)oTime.wMonth << (int)oTime.wDay << (int)oTime.wHour
		<< (int)oTime.wMinute << (int)oTime.wSecond << ".jpg";
		cvSaveImage(str.str().c_str(), GAME->GetScreenImage());
		std::cout << str.str() << " is " << "Saved." << std::endl;
	}

	//Sleep(100);

	GAME->UpdateScreenImage();
	if (GAME->GetScreenImage() == nullptr) {
		return;
	}

	PRODUCER->Update();

	if (b_isPause) {
		std::cout << "Pause Mecro...\n";
		//cvShowImage("sample", GAME->GetScreenImage());
		//cvWaitKey(1);
		return;
	}


	Scene* currentScene = nullptr;

	//cvShowImage("sample", GAME->GetScreenImage());
	//cvWaitKey(1);

	//�� Ȯ��
	for(auto scene : SCENE->GetScenes()) {
		if (!scene->CheckFirst()) continue;
		if(scene->CheckScene()) {
			currentScene = scene;
			break;
		}
	}

	//�� �� ���� ��Ȳ���� ���� ����Ʈ �ۼ�
	if (m_unkownCount > UNKOWN_COUNT_MAX) {
		GAME->SendAdbCommand("adb shell input keyevent KEYCODE_BACK");
		m_unkownCount = 0;
		MakeBugReport();
	}

	if (currentScene == nullptr) {
		//cvShowImage("sample", GAME->GetScreenImage());
		//cvWaitKey(10);
		m_unkownCount++;
		return;
	}

	if (!currentScene->GetIsIgnorePrevScene())
		SCENE->SetCurrentScene(currentScene);

	if(!Scene::isScene<UnkownScene>(currentScene)) {
		m_unkownCount = 0;
	}else {
		m_unkownCount++;
	}

	SCENE->UpdateLockState();
	if(SCENE->isLocked()) {
		return;
	}


	//���� �� ���
	std::cout << "\n\n=============================================================="
			<< "\n[���� ���] " << currentScene->GetName() << "\n\n";

	//�� �м�
	//if (!currentScene->CheckFirst()) return;
	currentScene->ReadData();

	//cvShowImage("sample", GAME->GetScreenImage());
	//cvWaitKey(1);

	//�ൿ ����
	currentScene->ActionDecision();

	if(!currentScene->GetIsIgnorePrevScene())
		SCENE->SetPrevScene(currentScene);



	//Sleep(500);
}


void MainProc::MakeBugReport() {
	SYSTEMTIME oTime;
	GetLocalTime(&oTime);
	std::stringstream name;
	name << (int)oTime.wYear << "_" << (int)oTime.wMonth << "_" << (int)oTime.wDay << "_" << (int)oTime.wHour << "_"
		<< (int)oTime.wMinute << "_" << (int)oTime.wSecond << "_";

	std::string savePath_jpg = "img/Bug Report/" + name.str() + ".jpg";
	std::string savePath_log = "img/Bug Report/" + name.str() + ".log";
	cvSaveImage(savePath_jpg.c_str(), GAME->GetScreenImage());

	Scene* prev_scene = SCENE->GetPrevScene();
	std::string prevSceneName = (prev_scene == nullptr) ? "NULLPTR" : prev_scene->GetName();

	std::ofstream file(savePath_log, std::ios::out);

	file << "[���� ����Ʈ]\n\n";
	file << "��¥ : " << name.str() << std::endl;
	file << "���� ȭ�� : " << prevSceneName << std::endl;

	auto ap = PRODUCER->GetAP();
	auto lp = PRODUCER->GetLP();
	long currentTime = timeGetTime();

	long ap_remain = ap.achieveTime - currentTime;
	long lp_remain = lp.achieveTime - currentTime;

	file << "\n���� : " << PRODUCER->GetRank()
		<< "\nAP : " << ap.current << " / " << ap.max << "\t" << ProducerAI::Millisecond2Min(ap_remain) << "�� " << ProducerAI::Millisecond2Second(ap_remain) << "�� �� ����"
		<< "\nLP : " << lp.current << " / " << lp.max << "\t" << ProducerAI::Millisecond2Min(lp_remain) << "�� " << ProducerAI::Millisecond2Second(lp_remain) << "�� �� ����"
		<< "\n\n";

	file << "���� �� : " << PRODUCER->GetTodoSize() << std::endl;

	int index = 0;
	for(auto todo : PRODUCER->GetTodoList()) {
		Scene* scene = todo->targetScene;
		file << "����[" << index << "]\n\t��ǥ ȭ�� : " << (scene != nullptr ? scene->GetName() : "Null") << std::endl;
		file << "\t���� ��ȿ ���� : " << (todo->isAvailable() ? "true" : "false") << std::endl;
		file << "\t���ڿ� �� : " << todo->todo_str << std::endl;
		file << "\t�߿䵵 : " << todo->important << std::endl;
		file << std::endl;
	}

	file.close();
}
