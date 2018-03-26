#pragma once
#include "MacroDef.h"
#include "Scene.h"
#include <vector>

#define SCENE SceneMgr::getInstance()

class SceneMgr {
public:
	DECLARE_SINGLETONE(SceneMgr);
	~SceneMgr();

	void SetScenes();

	template<class T>
	T* GetScene() const;

	Scene* GetPrevScene() const;
	void SetPrevScene(Scene* prev_scene);
	void SetCurrentScene(Scene* current_scene);
	void UpdateLockState();

	std::vector<Scene*> GetScenes() const;

	void LockScene();
	bool isLocked() const {
		return m_isLocked;
	}
private:
	std::vector<Scene*> m_scenes;
	Scene* m_prevScene = nullptr;
	Scene* m_currentScene = nullptr;

	bool m_isLocked = false;
	int m_lockedCount = 0;

};


template <class T>
T* SceneMgr::GetScene() const {
	for (auto component : m_scenes) {
		if (component == nullptr) continue;
		if (dynamic_cast<T*>(component)) {
			return static_cast<T*>(component);
		}
	}

	return nullptr;

}
