#pragma once

enum ATTRIBUTE_UNIT { UNKOWN, DANCE, PERFORMANCE, VOCAL };

struct EnsembleUnit {

	int current = 0;
	int max = 0;

	long startTime = 0;
	long achieveTime = 0;

};

struct EnsembleConcertUnit {
	
	ATTRIBUTE_UNIT type = UNKOWN;

	int team[3] = { 0 };

};