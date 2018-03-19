#include "Scene.h"



Scene::Scene() {}


Scene::~Scene() {}


bool Scene::ReadData() {
	return true;
}


bool Scene::GetIsIgnorePrevScene() const {
	return m_isIgnorePrevScene;
}
