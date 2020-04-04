#pragma once
#include "gameNode.h"

struct object
{
	RECT rc;
	POINT location;

	float angle;
};

struct lastArrow
{
	int score;
	float X;
	float Y;
	RECT rc;
	bool rcOn;
};

class mainGame : public gameNode
{
private:
	object _target;
	object _arrow;

	int _arrowNum;
	bool _arrowOn;
	lastArrow _marking[8];

	RECT _marker;

	bool _gameOn;
	int _count;

	char _printAngle[10];
	char _printScore[10];
	char _test[100];

public:
	HRESULT init();
	void release();
	void update();
	void render();

	mainGame() {}
	~mainGame() {}
};

