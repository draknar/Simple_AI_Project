#include "Tile.h";

Tile::Tile()
{
	owner = 'N';
	tokens = 0;
}

char Tile::getOwner()
{
	return owner;
}

void Tile::setOwner(char newOwner)
{
	owner = newOwner;
}

int Tile::getTokens()
{
	return tokens;
}

void Tile::setTokens(int numToken)
{
	tokens = numToken;
}