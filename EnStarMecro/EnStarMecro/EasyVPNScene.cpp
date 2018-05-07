#include "EasyVPNScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"


EasyVPNScene::EasyVPNScene() {
	m_name = "EasyVPNScene";


	RESMGR->RegisterImage("img/EasyVPNScene/korea_icon.jpg", "easyvpn_korea_icon");
	RESMGR->RegisterImage("img/EasyVPNScene/refresh.jpg", "easyvpn_refresh");
	RESMGR->RegisterImage("img/EasyVPNScene/vpn.jpg", "easyvpn_vpn");
}


EasyVPNScene::~EasyVPNScene() {}


bool EasyVPNScene::CheckFirst() {
	return false;
}


bool EasyVPNScene::CheckScene() {
	auto points = RESMGR->FindImages(nullptr, "easyvpn_vpn", 0.97, 1, true, cvRect(500, 970, 160, 98));
	if (!points.empty()) return true;

	m_unkownCount++;

	if (m_unkownCount > 20) {
		m_unkownCount = 0;

		GAME->SendAdbCommand("adb shell am force-stop com.easyovpn.easyovpn");
		Sleep(1000);
		GAME->SendAdbCommand("adb shell am start -n com.easyovpn.easyovpn/com.easyovpn.easyovpn.ui.MainActivity");
		Sleep(5000);
	}

	return false;
}


bool EasyVPNScene::ReadData() {

	m_unkownCount = 0;
	m_point = cvPoint(-1, -1);

	auto points = RESMGR->FindImages(nullptr, "easyvpn_refresh", 0.97, 1, true, cvRect(130, 970, 160, 98));
	if (!points.empty()) {
		if(!isRefeshed) {
			isRefeshed = true;
			m_point = cvPoint(190, 1020);
			return true;
		}

		m_point = cvPoint(160, 400);
		return true;
	}

	return false;
}


void EasyVPNScene::ActionDecision() {

	if(isRefeshed) {
		if (m_point.x >= 0) {
			GAME->SetMouseClick(m_point.x, m_point.y);
			Sleep(5000);
			return;
		}
	}

}


void EasyVPNScene::Reset() {
	isCleared = false;
	isRefeshed = false;
	m_unkownCount = 0;
}
