#include "Move.h"

Move::Move()
{
}

Move::Move(int y, int x, int d, bool curPlayer, int heurValue)
{
	row = y;
	column = x;
	direction = d;
	player = curPlayer;
	hValue = heurValue;
}

int Move::getRow()
{
	return row;
}

int Move::getColumn()
{
	return column;
}

int Move::getDirection()
{
	return direction;
}

bool Move::getPlayer()
{
	return player;
}

int Move::getHValue()
{
	return hValue;
}