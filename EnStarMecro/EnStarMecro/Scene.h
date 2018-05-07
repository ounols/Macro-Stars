#pragma once
#include <string>

class Scene {
public:
	Scene();
	virtual ~Scene();

	//���� ������ ���� 1�ܰ� �� Ȯ�� �۾�
	virtual bool CheckFirst() = 0;
	//2�ܰ� �� Ȯ��
	virtual bool CheckScene() = 0;
	//������ �б� �� �м�
	virtual bool ReadData();
	//�ൿ ����
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
