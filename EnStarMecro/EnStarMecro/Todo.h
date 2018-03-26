#pragma once
#include "Scene.h"
#include <string>

class Todo {

public:
	virtual ~Todo() {
	}

	virtual bool isAvailable() { return true; }
	virtual void Update() {}
	Scene* targetScene = nullptr;
	std::string todo_str = "";
	int important = 0;

};

class TodoWithCondition : public Todo {

public:
	TodoWithCondition(unsigned long end_time, int important_max)
		: endTime(end_time),
		  important_max(important_max) {
	}


private:
	unsigned long endTime;
	int important_max = 0;

};
