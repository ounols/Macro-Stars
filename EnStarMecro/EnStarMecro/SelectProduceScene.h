#pragma once
#include "Scene.h"
class SelectProduceScene : public Scene {
private:

	enum STATE { SelectChapter, SelectProduce, ProduceDetail, OpenMainStoryPop, NONE };

public:
	SelectProduceScene();
	~SelectProduceScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	void ActionDecision() override;

	int GetUsedAP();

private:

	bool CheckSelectChapter();
	bool CheckSelectProduce();
	bool CheckProduceDetail();
	bool CheckOpenMainStory();

	void ActionSelectChapter();
	void ActionSelectProduce();
	void ActionProduceDetail();
	void ActionOpenMainStory();

private:
	STATE m_state;
	int m_usedAP;
};

