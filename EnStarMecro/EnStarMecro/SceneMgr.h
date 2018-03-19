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

	std::vector<Scene*> GetScenes() const;
private:
	std::vector<Scene*> m_scenes;
	Scene* m_prevScene = nullptr;

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
