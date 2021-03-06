#include "ProducerAI.h"
#ifdef WIN32
#include <Windows.h>
#elif __linux__
#include "windows_for_linux.h"
#endif
// #include <iostream>
#include "ConcertTodo.h"
#include <fstream>
#include "GameClientMgr.h"

IMPLEMENT_SINGLETON(ProducerAI);

ProducerAI::ProducerAI() {
	
	m_concertDance = EnsembleConcertUnit();
	m_concertVocal = EnsembleConcertUnit();
	m_concertPerformance = EnsembleConcertUnit();

	m_concertDance.type = DANCE;
	m_concertVocal.type = VOCAL;
	m_concertPerformance.type = PERFORMANCE;

}


ProducerAI::~ProducerAI() {
	Exterminate();
}


void ProducerAI::Exterminate() {


	for (auto todo : m_todoList) {
		SAFE_DELETE(todo);
	}

	m_todoList.clear();


}


void ProducerAI::Update() {

	long currentTime = timeGetTime();

	long ap_coolTime = m_ap.achieveTime - currentTime;
	long lp_coolTime = m_lp.achieveTime - currentTime;

	// std::cout << "achieveTime = " << m_ap.achieveTime << ", currentTime = " << currentTime << '\n';
#ifdef WIN32
	if(GAME->GetIsAutoReboot()){
		SYSTEMTIME oTime;
		GetLocalTime(&oTime);
		oTime.wHour = (oTime.wHour + 9) % 24;
		if (oTime.wMinute / 10 == 1 && 
			oTime.wHour != 5 && oTime.wHour != 7 && oTime.wHour != 11 && oTime.wHour != 17 && oTime.wHour != 0) {
			std::ifstream reboot_in("reboot.dat");

			if (!reboot_in.is_open()) {
				std::ofstream reboot("reboot.dat");
				reboot << "���� �Ⱦ� ����";
				reboot.close();
			}

			reboot_in.close();
		}else if(oTime.wMinute / 10 == 1 && (oTime.wHour == 5 || oTime.wHour == 7 || oTime.wHour == 11 || oTime.wHour == 17 || oTime.wHour == 0)) {
			std::ifstream reboot_in("reboot.dat");

			if (reboot_in.is_open()) {
				reboot_in.close();
				std::remove("reboot.dat");
				std::cout << "Remove reboot.dat\n";
				Sleep(1000);

				//GAME->SendAdbCommand("adb shell am force-stop com.kakaogames.estarskr");
				Sleep(2000);

				if(oTime.wHour == 7) {
					GAME->isQuit = true;

					return;
				}

				if (GAME->GetIsGunstars()) {
					std::string str = "start ";
					str.append(GAME->GetGunStarsPath());
					system(str.c_str());
				} else {
					//GAME->SendAdbCommand("adb shell am start -n com.kakaogames.estarskr/com.happyelements.kirara.KakaoActivity");
					Sleep(2000);
				}
				GAME->isQuit = true;
				return;
			}

			reboot_in.close();
		}
	}
#endif
	if(ap_coolTime <= 0) {
		if(m_ap.current + 1 >= m_ap.max) {
			m_ap.achieveTime = currentTime + ap_coolTime;
			//m_ap.current = m_ap.max;
		}else {
			m_ap.achieveTime = currentTime + GetMillisecond(3, 0) + ap_coolTime;
			m_ap.current++;
		}
	}

	if (lp_coolTime <= 0) {
		if (m_lp.current + 1 >= m_lp.max) {
			m_lp.achieveTime = currentTime + lp_coolTime;
			//m_lp.current = m_lp.max;
		} else {
			m_lp.achieveTime = currentTime + GetMillisecond(30, 0) + lp_coolTime;
			m_lp.current++;
		}
	}

	std::cout << "\n\n[쿨타임]\nAP는 " << Millisecond2Min(ap_coolTime) << "분 " << Millisecond2Second(ap_coolTime) << "초 후 쿨타임 (" << m_ap.current << "/" << m_ap.max << ")\n"
				<< "LP는 " << Millisecond2Min(lp_coolTime) << "분 " << Millisecond2Second(lp_coolTime) << "초 후 쿨타임 (" << m_lp.current << "/" << m_lp.max << ")\n\n";

	std::vector<Todo*> swaped(m_todoList);
	for(const auto& todo : swaped) {
		if (todo == nullptr) continue;
		todo->Update();
	}

	//�ܼ�Ʈ�� �ߺ����� ���� �� ��� �����ϰ� �ٽ� Ȯ��
	auto concert_todos = GetAllTodo<ConcertTodo>();
	if (concert_todos.size() > 1) {
		for (auto todo : concert_todos) {
			RemoveTodo(todo);
		}
	}

}


EnsembleConcertUnit ProducerAI::GetConcertPerformance() const {
	return m_concertPerformance;
}


void ProducerAI::SetConcertPerformance(const EnsembleConcertUnit& concert_performance) {
	m_concertPerformance = concert_performance;
}


EnsembleConcertUnit ProducerAI::GetConcertVocal() const {
	return m_concertVocal;
}


void ProducerAI::SetConcertVocal(const EnsembleConcertUnit& concert_vocal) {
	m_concertVocal = concert_vocal;
}


EnsembleConcertUnit ProducerAI::GetConcertDance() const {
	return m_concertDance;
}


void ProducerAI::SetConcertDance(const EnsembleConcertUnit& concert_dance) {
	m_concertDance = concert_dance;
}


void ProducerAI::SetAP(EnsembleUnit ap) {
	m_ap = ap;
}


void ProducerAI::SetLP(EnsembleUnit lp) {
	m_lp = lp;
}


void ProducerAI::SetRank(int rank, int currentEXP, int totalEXP) {

	if(rank != -1) {
		m_rank = rank;
	}

	if (currentEXP != -1) {
		m_currentEXP = currentEXP;
	}

	if (totalEXP != -1) {
		m_totalEXP = totalEXP;
	}

}


void ProducerAI::SetStatus(STATUS status) {

	m_status = status;

}


void ProducerAI::AddTodo(Todo* todo) {
	m_todoList.insert(m_todoList.begin(), todo);
}


void ProducerAI::PushBackTodo(Todo* todo) {
	m_todoList.push_back(todo);
}


void ProducerAI::AddTodoWithCondition(TodoWithCondition todo) {
	m_todoWithConditionList.push_back(todo);
}


void ProducerAI::RemoveTodo(Todo* todo) {

	if(todo == nullptr) {
		todo = GetTodo();
	}

	auto iObj = std::find(m_todoList.begin(), m_todoList.end(), todo);

	if (iObj != m_todoList.end()) {
		m_todoList.erase(iObj);
		SAFE_DELETE(todo);
	}

}


Todo* ProducerAI::GetTodo() const {

	Todo* todo = nullptr;

	DWORD time = timeGetTime();

	for(auto _todo : m_todoList) {
		if(_todo->isAvailable()) {
			todo = _todo;
			break;
		}
	}

	//for(auto todoWC : m_todoWithConditionList) {
	//	
	//	int remainMinutes = Millisecond2Min(todoWC.endTime - time);
	//	int currentImportant = todoWC.important_max / ((remainMinutes < 10 ? 10 : remainMinutes) / 10);
	//	todoWC.important = currentImportant;

	//	if(currentImportant > todo.important) {
	//		todo = static_cast<Todo>(todoWC);
	//	}

	//}

	return todo;

}


bool ProducerAI::isTodoEmpty() const {

	return m_todoList.empty();

}


int ProducerAI::GetTodoSize() const {

	return m_todoList.size();

}


std::vector<Todo*> ProducerAI::GetTodoList() const {

	return m_todoList;

}


ProducerAI::STATUS ProducerAI::GetStatus() const {

	return m_status;

}


long ProducerAI::GetMillisecond(int min, int second) {

	return ((long)min * 60000) + ((long)second * 1000);

}


int ProducerAI::Millisecond2Min(long millisecond) {

	return millisecond / 60000;

}


int ProducerAI::Millisecond2Second(long millisecond) {

	return (millisecond / 1000) % 60;

}


int ProducerAI::GetIsChacked() const {
	return isChacked;
}


void ProducerAI::SetIsChacked(int is_chacked) {
	isChacked = is_chacked;
}
