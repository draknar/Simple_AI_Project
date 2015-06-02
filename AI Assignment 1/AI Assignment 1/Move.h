class Move
{
public:
	Move();
	Move(int y, int x, int d, bool curPlayer, int heurValue);
	int getRow();
	int getColumn();
	int getDirection();
	bool getPlayer();
	int getHValue();
private:
	int row;
	int column;
	int direction;
	bool player;
	int hValue;
};