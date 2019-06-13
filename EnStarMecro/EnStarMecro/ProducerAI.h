#pragma once
#include "MacroDef.h"
#include "Unit.h"
#include <list>
#include "Todo.h"
#include <vector>
#include <iostream>

#define PRODUCER ProducerAI::getInstance()

class ProducerAI {
public:
	enum STATUS { NOMAL, EVENT_LIGHT, EVENT_NOMAL, EVENT_IMPORTANT };
public:
	DECLARE_SINGLETONE(ProducerAI);
	~ProducerAI();

	void Exterminate();
	void Update();

	const EnsembleUnit& GetAP() const {
		return m_ap;
	}
	const EnsembleUnit& GetLP() const {
		return m_lp;
	}


	EnsembleConcertUnit GetConcertPerformance() const;


	void SetConcertPerformance(const EnsembleConcertUnit& concert_performance);


	EnsembleConcertUnit GetConcertVocal() const;


	void SetConcertVocal(const EnsembleConcertUnit& concert_vocal);


	EnsembleConcertUnit GetConcertDance() const;


	void SetConcertDance(const EnsembleConcertUnit& concert_dance);


	void SetCurrentAP(int current) {
		m_ap.current = current;
	}

	void SetCurrentLP(int current) {
		m_lp.current = current;
	}

	int GetRank() const {
		return m_rank;
	}

	int GetCurrentEXP() const {
		return m_currentEXP;
	}

	int GetTotalEXP() const {
		return m_totalEXP;
	}

	void SetAP(EnsembleUnit ap);
	void SetLP(EnsembleUnit lp);
	void SetRank(int rank = -1, int currentEXP = -1, int totalEXP = -1);
	void SetStatus(STATUS status);

	void AddTodo(Todo* todo);
	void PushBackTodo(Todo* todo);
	void AddTodoWithCondition(TodoWithCondition todo);
	void RemoveTodo(Todo* todo = nullptr);
	Todo* GetTodo() const;
	template<class T>
	T* GetTodo() const;
	template<class T>
	T* GetFirstTodo() const;
	template<class T>
	std::vector<T*> GetAllTodo() const;
	template<class T>
	void RemoveAllTodo();
	bool isTodoEmpty() const;
	int GetTodoSize() const;
	std::vector<Todo*> GetTodoList() const;

	STATUS GetStatus() const;

	static long GetMillisecond(int min, int second);
	static int Millisecond2Min(long millisecond);
	static int Millisecond2Second(long millisecond);


	int GetIsChacked() const;
	void SetIsChacked(int is_chacked);

private:
	EnsembleUnit m_ap;
	EnsembleUnit m_lp;
	int m_rank = -1;
	int m_currentEXP = -1;
	int m_totalEXP = -1;

	EnsembleConcertUnit m_concertPerformance;
	EnsembleConcertUnit m_concertVocal;
	EnsembleConcertUnit m_concertDance;

	STATUS m_status = EVENT_NOMAL;

	std::vector<Todo*> m_todoList;
	std::vector<TodoWithCondition> m_todoWithConditionList;

	int isChacked = 0;

public:
	//юс╫ц
	bool b_isGetGift = false;
	int m_introSceneCount = -1;
};


template <class T>
T* ProducerAI::GetTodo() const {

	for(auto todo : m_todoList) {
		if(dynamic_cast<T*>(todo)) {
			return static_cast<T*>(todo);
		}
	}

	return nullptr;

}


template <class T>
T* ProducerAI::GetFirstTodo() const {

	Todo* todo = GetTodo();

	if(dynamic_cast<T*>(todo)) {
		return static_cast<T*>(todo);
	}

	return nullptr;
}


template <class T>
std::vector<T*> ProducerAI::GetAllTodo() const {

	std::vector<T*> todos;

	for (auto todo : m_todoList) {
		if (dynamic_cast<T*>(todo)) {
			todos.push_back(static_cast<T*>(todo));
		}
	}

	return todos;
}

template <class T>
void ProducerAI::RemoveAllTodo() {

	for (auto todo : m_todoList) {
		if (dynamic_cast<T*>(todo)) {
			RemoveTodo(todo);
		}
	}

}
