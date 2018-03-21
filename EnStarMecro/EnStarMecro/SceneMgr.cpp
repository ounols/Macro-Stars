#include "SceneMgr.h"
#include "NomalProduceScene.h"
#include "FeverScene.h"
#include "DIFScene.h"
#include "ResultProduceScene.h"
#include "MainScene.h"
#include "SelectProduceScene.h"
#include "LoadingScene.h"
#include "ReconnectScene.h"
#include "IntroScene.h"
#include "ConcertPrepareScene.h"
#include "ConcertUnitScene.h"
#include "ConcertListScene.h"
#include "ConcertResultScene.h"
#include "UnkownScene.h"
#include "KakaoPopUpScene.h"

IMPLEMENT_SINGLETON(SceneMgr);

SceneMgr::SceneMgr() {
	SetScenes();
}


SceneMgr::~SceneMgr() {
	
	for (auto scene : m_scenes) {
		SAFE_DELETE(scene);
	}

	m_scenes.clear();

}


void SceneMgr::SetScenes() {
	
	m_scenes.push_back(new LoadingScene());
	m_scenes.push_back(new NomalProduceScene());
	m_scenes.push_back(new FeverScene());
	m_scenes.push_back(new DIFScene());
	m_scenes.push_back(new ResultProduceScene());
	m_scenes.push_back(new MainScene());
	m_scenes.push_back(new SelectProduceScene());
	m_scenes.push_back(new IntroScene());
	m_scenes.push_back(new ConcertPrepareScene());
	m_scenes.push_back(new ConcertUnitScene());
	m_scenes.push_back(new ConcertListScene());
	m_scenes.push_back(new ConcertResultScene());
	m_scenes.push_back(new KakaoPopUpScene());


	m_scenes.push_back(new UnkownScene());
	m_scenes.push_back(new ReconnectScene());

}


Scene* SceneMgr::GetPrevScene() const {
	return m_prevScene;
}


void SceneMgr::SetPrevScene(Scene* prev_scene) {
	m_prevScene = prev_scene;
}


std::vector<Scene*> SceneMgr::GetScenes() const {
	return m_scenes;
}
