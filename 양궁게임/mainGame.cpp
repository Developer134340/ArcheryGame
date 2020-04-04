#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();

	IMAGEMANAGER->addImage("배경", "배경.bmp", 892, 673);

	IMAGEMANAGER->addImage("화살", "화살.bmp", 28, 9, true, RGB(255, 0, 255));
	_arrow.location = { 83, 380 };
	_arrow.rc = { 0, 0, 0, 0 };
	_arrow.angle = 0.f;

	_arrowOn = false;
	_arrowNum = 7;
	
	for (int i = 0; i < 8; i++)
	{
		_marking[i].score = 0;
		_marking[i].X = 0;
		_marking[i].Y = 0;
		_marking[i].rc = { 0, 0, 0, 0 };
		_marking[i].rcOn = false;
	}

	IMAGEMANAGER->addImage("엑스", "엑스.bmp", 15, 14, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("과녁", "과녁.bmp", 25, 95, true, RGB(255, 0, 255));
	_target.location = { 830, 150 };
	_target.rc = RectMake(_target.location.x, _target.location.y, 25, 95);
	_target.angle = 0.f;

	_marker = RectMake(642, 154, 158, 132);
	_gameOn = false;
	_count = 0;

	sprintf(_printAngle, "%4.1lf°", _arrow.angle);
	sprintf(_test, "");
	

	return S_OK;
}

//=============================================================
//	## 해제 ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();
	//이미지 클래스를 나갈때까진 사용할 일 없다
	//동적할당 new를 사용했다면 이곳에서 SAFE_DELETE() 사용한다
	
	//플레이어 클래스 해제
}


//=============================================================
//	## 업데이트 ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();

	if (_gameOn == false)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			_gameOn = true;
		}
	}
	else
	{
		_target.location.y += 2;
		_target.rc = RectMake(_target.location.x, _target.location.y, 25, 95);
		_arrow.rc = RectMake( _arrow.location.x, _arrow.location.y, 28, 9 );
		sprintf(_printAngle, "%4.1lf°", _arrow.angle);

		if (_target.rc.bottom >= WINSIZEY - 149)
		{
			_target.location.y = 150;
		}

		if (KEYMANAGER->isStayKeyDown('X'))
		{
			_arrow.angle += 0.2f;
			if (_arrow.angle >= 10.f)
			{
				_arrow.angle = 10.f;
			}
		}

		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			_arrowOn = true;
			_arrow.angle = 0.0f;
		}

		if (_arrowOn == true)
		{
			RECT temp;
			_arrow.location.x += 10;
			if (IntersectRect(&temp, &_arrow.rc, &_target.rc))
			{
				_marking[_arrowNum].X = (((float)_arrow.rc.top + 4 - (float)_target.rc.top) / 95);
				_marking[_arrowNum].rc.left = _marker.left -7 + (_marking[_arrowNum].X * 158);
				_marking[_arrowNum].rc.right = _marking[_arrowNum].rc.left + 15;
				_marking[_arrowNum].Y = _arrow.angle / 10;
				_marking[_arrowNum].rc.top = _marker.bottom -7 - (132*_marking[_arrowNum].Y);
				_marking[_arrowNum].rc.bottom = _marking[_arrowNum].rc.top + 14;
				_marking[_arrowNum].rcOn = true;
				_arrow.location = { 83, 380 };
				_arrowNum--;
				_arrowOn = false;
				
			}
			else if (_arrow.rc.left >= WINSIZEX)
			{
				_arrow.location = { 83, 380 };
				_arrowNum--;
				_arrowOn = false;
			}
		}
		
	}

	sprintf(_test, "남은 화살 개수 : %d", _arrowNum);



	if (_arrowNum == -1)
	{
		_gameOn = false;
	}


}

//=============================================================
//	## 렌더 ## render(HDC hdc)
//=============================================================
void mainGame::render()
{
	//흰색 빈 비트맵 (이것도 렌더에 그냥 두기)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================

	IMAGEMANAGER->findImage("배경")->render(getMemDC(), 0, 0);
	IMAGEMANAGER->findImage("화살")->render(getMemDC(), _arrow.rc.left, _arrow.rc.top);
	IMAGEMANAGER->findImage("과녁")->render(getMemDC(), _target.rc.left, _target.rc.top);

	for (int i = 0; i < 8; i++)
	{
		if (_marking[i].rcOn == false) continue;
		IMAGEMANAGER->findImage("엑스")->render(getMemDC(), _marking[i].rc.left, _marking[i].rc.top);
	}

	for (int i = 0; i < _arrowNum; i++)
	{
		IMAGEMANAGER->findImage("화살")->render(getMemDC(), 92, 280 - (i*12));
	}
	HFONT myFont = CreateFont(35, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "궁서체");
	HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	SetBkMode(getMemDC(), TRANSPARENT);
	TextOut(getMemDC(), 499, 216, _printAngle, lstrlen(_printAngle));
	SelectObject(getMemDC(), oldFont);
	DeleteObject(myFont);

	//TextOut(getMemDC(), 0, 0, _test, lstrlen(_test));

	/*
	EllipseMakeCenter(getMemDC(), 721, 220, 140, 117);
	EllipseMakeCenter(getMemDC(), 721, 220, 111, 92);
	EllipseMakeCenter(getMemDC(), 721, 221, 86, 70);
	EllipseMakeCenter(getMemDC(), 721, 222, 58, 49);
	EllipseMakeCenter(getMemDC(), 721, 221, 30, 24);
	*/


//=============================================================
	//백버퍼의 내용을 HDC에 그린다 (이것도 렌더에 그냥 두기)
	this->getBackBuffer()->render(getHDC());
}