#include "OpenVPNScene.h"
#include "ResMgr.h"
#include "GameClientMgr.h"


OpenVPNScene::OpenVPNScene() {
	m_name = "OpenVPNScene";


	RESMGR->RegisterImage("img/OpenVPNScene/title_import.jpg", "openvpn_title_import");
	RESMGR->RegisterImage("img/OpenVPNScene/title_ovpn.jpg", "openvpn_title_ovpn");
	RESMGR->RegisterImage("img/OpenVPNScene/caution.jpg", "openvpn_caution");
	RESMGR->RegisterImage("img/OpenVPNScene/bt_disable.jpg", "openvpn_bt_disable");
	RESMGR->RegisterImage("img/OpenVPNScene/bt_able.jpg", "openvpn_bt_able");
	RESMGR->RegisterImage("img/OpenVPNScene/bt_cancel.jpg", "openvpn_bt_cancel");

}


OpenVPNScene::~OpenVPNScene() {}


bool OpenVPNScene::CheckFirst() {
	return false;
}


bool OpenVPNScene::CheckScene() {

	isPopUp = false;
	m_point = cvPoint(-1, -1);

	auto points = RESMGR->FindImages(nullptr, "openvpn_caution", 0.97, 1, true, cvRect(357, 357, 194, 180));
	if (!points.empty()) {
		m_state = CAUTION;
		return true;
	}

	points = RESMGR->FindImages(nullptr, "openvpn_bt_cancel", 0.97, 1, true, cvRect(918, 658, 443, 83));
	if (!points.empty()) {
		isPopUp = true;
		m_point = cvPoint(918 + points[0].x + 220, 658 + points[0].y + 19);
		return true;
	}

	points = RESMGR->FindImages(nullptr, "openvpn_title_import", 0.97, 1, true, cvRect(90, 64, 647, 143));
	if (!points.empty()) {
		m_state = IMPORT;
		return true;
	}

	points = RESMGR->FindImages(nullptr, "openvpn_title_ovpn", 0.97, 1, true, cvRect(90, 64, 647, 143));
	if (!points.empty()) {
		m_state = OVPN;
		return true;
	}


	m_unkownCount++;

	if (m_unkownCount > 20) {
		m_unkownCount = 0;

		GAME->SendAdbCommand("adb shell am force-stop net.openvpn.openvpn");
		Sleep(1000);
		GAME->SendAdbCommand("adb shell am start -n com.easyovpn.easyovpn/com.easyovpn.easyovpn.ui.ServerListActivity");
		Sleep(5000);

	}
}


bool OpenVPNScene::ReadData() {
	m_unkownCount = 0;
	isCleared = false;

	if (isPopUp) return true;

	if(m_state == IMPORT) {
		m_point = cvPoint(1842, 140);
		return true;
	}

	if (m_state == OVPN) {
		auto points = RESMGR->FindImages(nullptr, "openvpn_bt_disable", 0.97, 1, true, cvRect(12, 354, 191, 128));
		if (!points.empty()) {
			m_point = cvPoint(109, 422);
			return true;
		}

		points = RESMGR->FindImages(nullptr, "openvpn_bt_able", 0.97, 1, true, cvRect(12, 354, 191, 128));
		if (!points.empty()) {
			isCleared = true;
			return true;
		}

	}

	if(m_state == CAUTION) {
		m_point = cvPoint(434, 750);
		return true;
	}

	return false;

}


void OpenVPNScene::ActionDecision() {

	if(m_state == CAUTION) {
		GAME->SetMouseClick(m_point.x, m_point.y);
		Sleep(1000);
		GAME->SetMouseClick(1250, 870);
		Sleep(2000);
		return;
	}

	if(m_point.x >= 0) {
		GAME->SetMouseClick(m_point.x, m_point.y);
		Sleep(2000);
		return;
	}

	if(isCleared) {
		GAME->SendAdbCommand("adb shell am force-stop com.easyovpn.easyovpn");
		Sleep(1000);
		GAME->SendAdbCommand("adb shell am start -n com.kakaogames.estarskr/com.happyelements.kirara.KakaoActivity");
		Sleep(5000);
		return;
	}

}


void OpenVPNScene::Reset() {
	isCleared = false;
}
