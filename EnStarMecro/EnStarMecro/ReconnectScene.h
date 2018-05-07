#pragma once
#include "Scene.h"
#include <opencv2/core/cuda.inl.hpp>

class ReconnectScene : public Scene {
private:
	enum STATE { RECONNECT, QUIT_CONCERT, UNKOWN, UNKOWN_EXIT, SERVER_ERROR };
public:
	ReconnectScene();
	~ReconnectScene();


	bool CheckFirst() override;
	bool CheckScene() override;
	void ActionDecision() override;

private:
	void ActionReconnect() const;
	void ActionQuitConcert() const;
	void ActionUnkown() const;
	void ActionServerError() const;

private:
	STATE m_state = RECONNECT;
	CvPoint m_unkownPos;
};

