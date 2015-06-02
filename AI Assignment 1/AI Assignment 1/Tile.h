class Tile
{
public:
	Tile();
	char getOwner();
	void setOwner(char newOwner);
	int getTokens();
	void setTokens(int numToken);
private:
	char owner;
	int tokens;
};