#pragma once
#include <string>

class Scene {
public:
	Scene();
	virtual ~Scene();

	//성능 개선을 위한 1단계 씬 확인 작업
	virtual bool CheckFirst() = 0;
	//2단계 씬 확인
	virtual bool CheckScene() = 0;
	//데이터 읽기 및 분석
	virtual bool ReadData();
	//행동 결정
	virtual void ActionDecision() = 0;

	template<class T>
	static bool isScene(Scene* scene);

	std::string GetName() const {
		return m_name;
	}

	bool GetIsIgnorePrevScene() const;
	bool isSkiped() const;

	void SetSkiped(bool skiped);

protected:
	std::string m_name = "Scene";
	bool m_isIgnorePrevScene = false;
	bool m_isSkiped = false;

};


template <class T>
bool Scene::isScene(Scene* scene) {

	if (dynamic_cast<T*>(scene)) {
		return true;
	}

	return false;

}
