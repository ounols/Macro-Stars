#include "Scene.h"



Scene::Scene() {}


Scene::~Scene() {}


bool Scene::ReadData() {
	return true;
}


bool Scene::isSkiped() const {
	return m_isSkiped;
}


void Scene::SetSkiped(bool skiped) {
	m_isSkiped = skiped;
}


bool Scene::GetIsIgnorePrevScene() const {
	return m_isIgnorePrevScene;
}
