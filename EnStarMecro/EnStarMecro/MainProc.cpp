#include "MainProc.h"
#include "ProducerAI.h"
#include "ResMgr.h"
#include "GameClientMgr.h"
#include "NomalProduceScene.h"
#include "SceneMgr.h"
#include "ProduceTodo.h"
#include "ConcertTodo.h"
#include "UnkownScene.h"
#include "PlayStoreScene.h"
#include "EasyVPNScene.h"
#include "OpenVPNScene.h"
#include "ReconnectScene.h"
#include "UltraSurfScene.h"
#include "HolaScene.h"


#ifdef WIN32
#define popen _popen
#define pclose _pclose
#endif

MainProc::MainProc() {
	GAME->GetGameHWND();
	RESMGR->TrainKNN("img/KNN/classifications.xml", "img/KNN/images.xml");


	//ProduceTodo todo = ProduceTodo();
	//todo.type = ProduceTodo::DAILY;

	//PRODUCER->AddTodo(static_cast<Todo>(todo));

	//�ӽ�
	//namedWindow("sample", cv::WINDOW_GUI_NORMAL);
	//namedWindow("����", cv::WINDOW_AUTOSIZE);
	PRODUCER->SetStatus(ProducerAI::EVENT_IMPORTANT);

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
	Sleep(5000);

// #ifdef __linux__
#ifdef false
	const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    
    ssize_t n;

    fd = open(dev, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
        return;
    }
#endif
}


MainProc::~MainProc() {
	ProducerAI::delInstance();
	ResMgr::delInstance();
	GameClientMgr::delInstance();
	SceneMgr::delInstance();

	cvDestroyAllWindows();
}

#ifdef __linux__
bool MainProc::LinuxKeyEvent(){
	ssize_t n = read(fd, &ev, sizeof ev);
        if (n == (ssize_t)-1) {
            if (errno == EINTR)
                return true;
            else
                return true;
        } else
        if (n != sizeof ev) {
            errno = EIO;
            return true;
        }

        //
        if (ev.type == EV_KEY){
            if(ev.code == 1 && ev.value == 1){
                b_isQuit = true;
				return false;
            }
			if(ev.code == 107 && ev.value == 1 && PRODUCER->GetIsChacked() == 198294) {
				b_isPause = !b_isPause;
				return false;
			}
        }
}
#endif

void MainProc::Update() {
#ifdef WIN32
	if(GetAsyncKeyState(VK_ESCAPE)) {
		b_isQuit = true;
		return;
	}

	if(GetAsyncKeyState(VK_END) && PRODUCER->GetIsChacked() == 198294) {
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
#elif __linux__
	// LinuxKeyEvent();
#endif

	if(GAME->isQuit) {
		b_isQuit = true;
		return;
	}

	//���� ��Ƽ��Ƽ ����
	GetCurrentActivity();

	//Sleep(100);

	GAME->UpdateScreenImage();
	if (GAME->GetScreenImage() == nullptr) {
		return;
	}

	//http ����
	if (isReport)
		MakeCurrentStateReport();

	GetRemotedInfo();

	//������ �����ϴ°��� Ȯ��
	if(m_currentActivity != ENSTARS) {

		Scene* scene = nullptr;

		switch (m_currentActivity) {
		case UNKOWN: {
			if(!GAME->GetIsVpn())
				GAME->SendAdbCommand("adb shell am start -n com.kakaogames.estarskr/com.happyelements.kirara.KakaoActivity");
			return;
		}

		case MARKET: {
			scene = SCENE->GetScene<PlayStoreScene>();
		} break;

		case EASYVPN: {
			scene = SCENE->GetScene<EasyVPNScene>();
		} break;

		case OPENVPN: {
			scene = SCENE->GetScene<OpenVPNScene>();
		} break;
		
		case ULTRA: {
			scene = SCENE->GetScene<UltraSurfScene>();
		} break;
		case HOLA: {
			scene = SCENE->GetScene<HolaScene>();
		} break;
		default: break;
		}

		if (scene == nullptr) return;

		scene->CheckScene();
		scene->ReadData();
		scene->ActionDecision();

		return;
	}



	PRODUCER->Update();

	if (b_isPause) {
		std::cout << "Pause Macro...\n";
		//cvShowImage("sample", GAME->GetScreenImage());
		//cvWaitKey(1);
		return;
	}


	Scene* currentScene = nullptr;

	//cvShowImage("sample", GAME->GetScreenImage());
	//cvWaitKey(1);

	//����Ÿ �߰�����
	//�ε��� �����ʰ� Ÿ�Ӿƿ��� ���ѵ� Ƚ���� �Ѿ��ٸ�
	//vpn�� �վ���
	if(!isLoaded) {
		if(loadingCount > 5) {
			loadingCount = 0;
			GAME->SendAdbCommand("adb shell am force-stop com.kakaogames.estarskr");
			Sleep(1000);
			GAME->SendAdbCommand("adb shell am force-stop org.hola");
			Sleep(1000);
			GAME->SendAdbCommand("adb shell am start -n org.hola/org.hola.browser_activity");
			Sleep(5000);
			isUsedVPN = true;
			return;
		}
	}

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
		if(m_unkownAccrue <= 0) {
			
		}else if(m_unkownAccrue == 1){
			GAME->SendAdbCommand("adb shell input keyevent KEYCODE_BACK");
		} else {
			GAME->SendAdbCommand("adb shell am force-stop com.kakaogames.estarskr");
		}
		m_unkownCount = 0;
		m_unkownAccrue++;
		MakeBugReport();
	}

	if (currentScene == nullptr) {
		//cvShowImage("sample", GAME->GetScreenImage());
		//cvWaitKey(10);
		m_unkownCount++;
		return;
	}

	if(currentScene->isSkiped()) {
		m_unkownCount++;
		currentScene->SetSkiped(false);
		return;
	}

	if (!currentScene->GetIsIgnorePrevScene())
		SCENE->SetCurrentScene(currentScene);

	if(!Scene::isScene<UnkownScene>(currentScene)) {
		m_unkownCount = 0;
		m_unkownAccrue = 0;
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

	if(!isLoaded && Scene::isScene<ReconnectScene>(currentScene)) {
		loadingCount++;
	}else if(!isLoaded){
		isLoaded = true;
	}

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


void MainProc::SetGunStars(bool enable) {

	isGunStars = enable;
	GAME->SetIsGunstars(isGunStars);
}


void MainProc::SetMarket(bool enable) {

	isMarket = enable;
	GAME->SendAdbCommand("adb shell am start -a android.intent.action.VIEW -d \'market://details?id=com.kakaogames.estarskr\'");

}


void MainProc::SetReport(bool enable) {
	isReport = enable;
	GAME->SetHttp(enable);
}


void MainProc::MakeBugReport() {
#ifdef WIN32
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
	file << "�˼����� ���� Ƚ�� : " << m_unkownAccrue << std::endl;

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

		index++;
	}

	file.close();
#endif
}


void MainProc::MakeCurrentStateReport() {
#ifdef WIN32

	SYSTEMTIME oTime;
	GetLocalTime(&oTime);
	std::stringstream name;
	name << (int)oTime.wYear << "-" << (int)oTime.wMonth << "-" << (int)oTime.wDay << "\t" << (int)oTime.wHour << "h "
		<< (int)oTime.wMinute << "m " << (int)oTime.wSecond << "s";


	Scene* prev_scene = SCENE->GetPrevScene();
	std::string prevSceneName = (prev_scene == nullptr) ? "NULLPTR" : prev_scene->GetName();

	std::ofstream file("C:\\http\\report\\current.txt", std::ios::out);

	file << "date : " << name.str() << std::endl;
	file << "lastScene : " << prevSceneName << std::endl;
	file << "unkown count : " << m_unkownAccrue << std::endl;

	auto ap = PRODUCER->GetAP();
	auto lp = PRODUCER->GetLP();
	long currentTime = timeGetTime();

	long ap_remain = ap.achieveTime - currentTime;
	long lp_remain = lp.achieveTime - currentTime;

	file << "\nlevel : " << PRODUCER->GetRank()
		<< "\nAP : " << ap.current << " / " << ap.max << "\t" << ProducerAI::Millisecond2Min(ap_remain) << "min " << ProducerAI::Millisecond2Second(ap_remain) << "sec"
		<< "\nLP : " << lp.current << " / " << lp.max << "\t" << ProducerAI::Millisecond2Min(lp_remain) << "min " << ProducerAI::Millisecond2Second(lp_remain) << "sec"
		<< "\n\n";

	file << "todo count : " << PRODUCER->GetTodoSize() << std::endl;

	int index = 0;
	for (auto todo : PRODUCER->GetTodoList()) {
		Scene* scene = todo->targetScene;
		file << "����[" << index << "]\n\t��ǥ ȭ�� : " << (scene != nullptr ? scene->GetName() : "Null") << std::endl;
		file << "\t���� ��ȿ ���� : " << (todo->isAvailable() ? "true" : "false") << std::endl;
		file << "\t���ڿ� �� : " << todo->todo_str << std::endl;
		file << "\t�߿䵵 : " << todo->important << std::endl;
		file << std::endl;

		index++;
	}

	file.close();
#endif
}


void MainProc::GetRemotedInfo() {
#ifdef WIN32
	SYSTEMTIME oTime;
	GetLocalTime(&oTime);

	if (remoteDate == oTime.wHour * 100 + oTime.wMinute) {
		return;
	}

	std::ifstream file("C:\\http\\remote\\info.txt", std::ios::in);

	if (!file.is_open()) return;


	remoteDate = oTime.wHour * 100 + oTime.wMinute;

	char buf[100];

	std::cout << "\n[Read Remote Info]\n";

	while(!file.eof()) {
		file.getline(buf, 100);
		std::string str = buf;
		if(str.find("a_status") != std::string::npos) {

			if(str.find("e_light") != std::string::npos && 
				PRODUCER->GetStatus() != ProducerAI::EVENT_LIGHT) {
				PRODUCER->SetStatus(ProducerAI::EVENT_LIGHT);
				auto todo = PRODUCER->GetTodo<ProduceTodo>();
				if(todo != nullptr)
					todo->type = ProduceTodo::LIMIT;
			}

			else if (str.find("e_normal") != std::string::npos &&
				PRODUCER->GetStatus() != ProducerAI::EVENT_NOMAL) {
				PRODUCER->SetStatus(ProducerAI::EVENT_NOMAL);
				auto todo = PRODUCER->GetTodo<ProduceTodo>();
				if (todo != nullptr)
					todo->type = ProduceTodo::LIMIT;
			}

			else if (str.find("e_important") != std::string::npos &&
				PRODUCER->GetStatus() != ProducerAI::EVENT_IMPORTANT) {
				PRODUCER->SetStatus(ProducerAI::EVENT_IMPORTANT);
				auto todo = PRODUCER->GetTodo<ProduceTodo>();
				if (todo != nullptr)
					todo->type = ProduceTodo::LIMIT;
			}

			else if (str.find("normal") != std::string::npos &&
				PRODUCER->GetStatus() != ProducerAI::NOMAL) {
				PRODUCER->SetStatus(ProducerAI::NOMAL);
				auto todo = PRODUCER->GetTodo<ProduceTodo>();
				if (todo != nullptr)
					todo->type = ProduceTodo::DAILY;
			}

			continue;
		}

		if (str.find("a_getCurrent") != std::string::npos) {
			
			if (str.find("true") != std::string::npos && !isReport) {
				SetReport(true);
			}

			else if (str.find("false") != std::string::npos && isReport) {
				SetReport(false);
			}

		}

	}
#endif
}


void MainProc::GetCurrentActivity() {

	m_currentActivity = UNKOWN;
#ifdef WIN32
	FILE* fpipe = popen("adb\\adb shell \"dumpsys window windows | grep -E \'mCurrentFocus|mFocusedApp\'\"", "r");
#elif __linux__
	FILE* fpipe = popen("adb shell \"dumpsys window windows | grep -E \'mCurrentFocus|mFocusedApp\'\"", "r");
#endif
	
	if (fpipe == NULL)
		std::cout << "\nadb is not available.(GetActivity)\n";

	char adb_result_str[1024] = {};
	std::string adb_result;

	fread(adb_result_str, 1024, 1, fpipe);
	adb_result = adb_result_str;

	pclose(fpipe);


	//�ӽ�Ÿ ������ Ȯ��
	if (adb_result.find("com.kakaogames.estarskr/com.happyelements.kirara.KakaoActivity") != std::string::npos) {
		m_currentActivity = ENSTARS;
		return;
	}

	//���� ������ Ȯ��
	if (adb_result.find("com.android.vending") != std::string::npos) {
		m_currentActivity = MARKET;
		return;
	}

	////easy vpn ������ Ȯ��
	//if (adb_result.find("com.easyovpn.easyovpn") != std::string::npos) {
	//	m_currentActivity = EASYVPN;
	//	return;
	//}

	////open vpn ������ Ȯ��
	//if (adb_result.find("net.openvpn.openvpn") != std::string::npos) {
	//	m_currentActivity = OPENVPN;
	//	return;
	//}

	//ultrasurf �˾����� Ȯ��
	if (adb_result.find("us.ultrasurf.mobile.ultrasurf") != std::string::npos) {
		m_currentActivity = ULTRA;
		return;
	}

	//hola �˾����� Ȯ��
	if (adb_result.find("org.hola") != std::string::npos) {
		m_currentActivity = HOLA;
		return;
	}

	//vpn �˾����� Ȯ��
	if (adb_result.find("com.android.vpndialogs") != std::string::npos) {
		m_currentActivity = ULTRA;
		return;
	}

}
