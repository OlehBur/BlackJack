#pragma once

class GameItems {
	int x, y;

public:
	virtual void Draw()=0;

	void MoveToCoord(const int& x, const int& y);
	
	int& GetCoordX();
	int& GetCoordY();
};