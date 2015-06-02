#include <iostream>
#include <iomanip>
#include <string>
#include "Tile.h"
#include "Move.h"

using namespace std;

bool currentPlayer = false;
int tempTokens = 0;
Tile board[4][4];
Move cpuMove;
int minMaxDepth = 3;
bool black = true;

int heurValue(Tile tBoard[4][4])
{
	int totalSpaces = 0;
	int totalTiles = 0;
	int totalNeutral = 0;
	bool inBounds;
	int y;
	int x;
	char color;
	char other;

	if (black)
	{
		color = 'B';
		other = 'W';
	}
	else
	{
		color = 'W';
		other = 'B';
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (tBoard[i][j].getOwner() == color)
			{
				for (int k = 1; k < 9; k++)
				{
					inBounds = true;
					int spaces = 0;
					int tokens = tBoard[i][j].getTokens();
					y = i;
					x = j;
					while (inBounds)
					{
						if (k == 2 || k == 3 || k == 4) x = x + 1;
						if (k == 4 || k == 5 || k == 6) y = y + 1;
						if (k == 6 || k == 7 || k == 8) x = x - 1;
						if (k == 8 || k == 1 || k == 2) y = y - 1;

						if (y > 3 || y < 0 || x > 3 || x < 0) inBounds = false;
						if (tBoard[y][x].getOwner() == other) inBounds = false;

						if (inBounds && tBoard[y][x].getOwner() == 'N') spaces -= 2;
						else if (inBounds && tBoard[y][x].getOwner() == color) spaces -= 1;
					}

					totalSpaces -= spaces;
				}
			}
			/*else if (tBoard[i][j].getOwner() == other)
			{
				totalTiles += 1;
			}*/
			/*else
			{
				totalNeutral += 1;
			}*/
		}
	}

	if (totalSpaces == 0) return 149;
	else return (totalTiles + totalNeutral - totalSpaces);
}

void draw()
{
	cout << "    A   B   C   D  " << endl;
	cout << "  -----------------" << endl;
	for (int i = 0; i < 4; i++)
	{
		cout << i+1 << " |";
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j].getOwner() != 'N')
			{
				cout << board[i][j].getOwner();
				if (board[i][j].getTokens() < 10) cout << " ";
				cout << board[i][j].getTokens();
			}
			else
			{
				cout << "   ";
			}
			cout << "|";
		}
		cout << endl << "  -----------------" << endl;
	}
	cout << "Heuristic Value: " << heurValue(board) << endl; // heuristic check
}

bool gameMove(int y, int x, int d, char self)
{
	char other;
	int spaces = 0;
	int tempTokens;
	bool blocked = false;

	if (self == 'B') other = 'W';
	else other = 'B';

	if (board[y][x].getOwner() == self)
	{
		int i = 1;
		int a[4] = {x, x, x, x};
		int b[4] = {y, y, y, y};
		tempTokens = board[y][x].getTokens();

		while (!blocked)
		{
			// direction
			if (d == 2 || d == 3 || d == 4) a[i] = a[i-1] + 1;
			if (d == 4 || d == 5 || d == 6) b[i] = b[i-1] + 1;
			if (d == 6 || d == 7 || d == 8) a[i] = a[i-1] - 1;
			if (d == 8 || d == 1 || d == 2) b[i] = b[i-1] - 1;

			// boundry check
			if (a[i] > 3 || a[i] < 0 || b[i] > 3 || b[i] < 0) blocked = true;
			else
			{
				if (board[b[i]][a[i]].getOwner() != other) spaces++;
				else blocked = true;
			}

			i++;

			if (i > 3) blocked = true;
		}

		if (spaces == 0)
		{
			if ((!currentPlayer && black) || currentPlayer && !black) cout << endl << "Cannot move in that direction." << endl;
			return false;
		}

		// remove tokens from starting square
		board[y][x].setOwner('N');
		board[y][x].setTokens(0);
		
		board[b[1]][a[1]].setOwner(self);

		if (spaces == 1)
		{
			board[b[1]][a[1]].setTokens(tempTokens + board[b[1]][a[1]].getTokens());
		}
		else if (spaces == 2)
		{
			board[b[1]][a[1]].setTokens(1 + board[b[1]][a[1]].getTokens());
			if (tempTokens >= 2)
			{
				board[b[2]][a[2]].setOwner(self);
				board[b[2]][a[2]].setTokens(tempTokens - 1 + board[b[2]][a[2]].getTokens());
			}
		}
		else if (spaces == 3)
		{
			board[b[1]][a[1]].setTokens(1 + board[b[1]][a[1]].getTokens());
			if (tempTokens >= 3)
			{
				board[b[2]][a[2]].setOwner(self);
				board[b[2]][a[2]].setTokens(2 + board[b[2]][a[2]].getTokens());
				if (tempTokens > 3) board[b[3]][a[3]].setOwner(self);
				board[b[3]][a[3]].setTokens(tempTokens - 3 + board[b[3]][a[3]].getTokens());
			}
			else if (tempTokens == 2)
			{
				board[b[2]][a[2]].setOwner(self);
				board[b[2]][a[2]].setTokens(1 + board[b[2]][a[2]].getTokens());
			}
		}
		return true;
	}
	else
	{
		cout << "Error has occured.";
		return false;
	}
}

bool checkVictory(char self)
{
	char other;

	if (self == 'B') other = 'W';
	else other = 'B';

	for (int h = 0; h < 4; h++)
	{
		for (int g = 0; g < 4; g++)
		{
			if (board[h][g].getOwner() == self)
			{
				// if any adjacent tiles are free, opponent's victory is not assured
				if (h > 0 && board[h-1][g].getOwner() != other) return false;				// up
				if (h > 0 && g < 3 &&board[h-1][g+1].getOwner() != other) return false;		// up right
				if (g < 3 && board[h][g+1].getOwner() != other) return false;				// right
				if (h < 3 && g < 3 &&board[h+1][g+1].getOwner() != other) return false;		// down right
				if (h < 3 && board[h+1][g].getOwner() != other) return false;				// down
				if (h < 3 && g > 0 && board[h+1][g-1].getOwner() != other) return false;	// down left
				if (g > 0 && board[h][g-1].getOwner() != other) return false;				// left
				if (h > 0 && g > 0 && board[h-1][g-1].getOwner() != other) return false;	// up left
			}
		}
	}

	// opponent wins
	return true;
}

Move minMax(Tile tBoard[4][4], int lastY, int lastX, int lastD, int depth, bool curPlayer)
{
	if (depth == minMaxDepth) return Move(lastY, lastX, lastD, curPlayer, heurValue(tBoard));

	char color;
	bool other;
	int bestValue;
	Move bestMove = Move(lastY, lastX, lastD, curPlayer, heurValue(tBoard));
	Move tempMove;
	Tile tempBoard[4][4];

	if (!curPlayer)
	{
		color = 'B';
		other = true;
		if (black) bestValue = 150;
		else bestValue = -150;
	}
	else
	{
		color = 'W';
		other = false;
		if (black) bestValue = -150;
		else bestValue = 150;
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tempBoard[i][j] = tBoard[i][j];
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (tBoard[i][j].getOwner() == color)
			{
				for (int k = 1; k < 9; k++)
				{
					if (gameMove(i, j, k, color))
					{
						tempMove = minMax(tBoard, i, j, k, depth+1, other);
						if ((((!curPlayer && black) || (curPlayer && !black)) && tempMove.getHValue() < bestValue) || (((curPlayer && black) || (!curPlayer && !black)) && tempMove.getHValue() > bestValue))
						{
							bestValue = tempMove.getHValue();
							bestMove = Move(i, j, k, curPlayer, bestValue);
						}
						for (int l = 0; l < 4; l++)
						{
							for (int m = 0; m < 4; m++)
							{
								tBoard[l][m] = tempBoard[l][m];
							}
						}
					}
				}
			}
		}
	}

	return bestMove;
}

void game()
{
	bool end = false;
	bool compEnd = false;
	bool incorrect;
	bool legal;
	bool select;
	bool done;
	string action = "";
	int coordX;
	int coordY;
	int dir;
	char tokOwn;

	board[0][0].setOwner('B');
	board[3][3].setOwner('W');
	board[0][0].setTokens(10);
	board[3][3].setTokens(10);

	do
	{
		done = true;
		cout << "Which game mode would you like to play?" << endl;
		cout << "1. Player vs CPU" << endl;
		cout << "2. Player vs Player" << endl;
		cout << "Option > ";
		cin >> action;
		cout << endl;

		if (action[0] == '1') select = false;
		else if (action[0] == '2') select = true;
		else
		{
			select = true;
			done = false;
			cout << "Incorrect option entered. Please try again." << endl;
		}

		action = "";

		if (!select)
		{
			cout << "What color does the player choose:" << endl;
			cout << "1. Black" << endl;
			cout << "2. White" << endl;
			cout << "Option > ";
			cin >> action;
			cout << endl;

			if (action[0] == '1') black = true;
			else if (action[0] == '2') black = false;
			else
			{
				done = false;
				cout << "Incorrect option entered. Please try again." << endl;
			}
		}

	}
	while (!done);

	draw();

	while(!end && !compEnd)
	{
		if ((!currentPlayer && black) || (currentPlayer && !black) || (currentPlayer && select))
		{
			incorrect = true;
			while (incorrect) // command
			{
				coordY = -1;
				coordX = -1;
				dir = -1;

				cout << "Please enter the command (format: 1A-4). P";
				if (!currentPlayer) cout << 1;
				else cout << 2;
				cout << " action > ";
				cin >> action;

				if (action.length() == 4)
				{
					int y = action[0] - 48;
					if (y >= 1 && y <= 4) coordY = y - 1;

					if (action[1] == 'a' || action[1] == 'A') coordX = 0;
					else if (action[1] == 'b' || action[1] == 'B') coordX = 1;
					else if (action[1] == 'c' || action[1] == 'C') coordX = 2;
					else if (action[1] == 'd' || action[1] == 'D') coordX = 3;

					int d = action[3] - 48;
					if (d >= 1 && d <= 8) dir = d;
				}

				if (coordX != -1 && coordY != -1 && action[2] == '-' && dir != -1)
				{
					incorrect = false;
				}

				if (incorrect) cout << endl << "Incorrect command entered." << endl;

				if (!currentPlayer) tokOwn = 'B';
				else tokOwn = 'W';

				if (board[coordY][coordX].getOwner() != tokOwn)
				{
					if (!incorrect) cout << endl << "There are no tokens owned by you in the location." << endl;
					incorrect = true;
				}

			} // end of command line
		}
		else
		{
			if (black)
			{
				tokOwn = 'W';
				cpuMove = minMax(board, -1, -1, -1, 0, true);
			}
			else
			{
				tokOwn = 'B';
				cpuMove = minMax(board, -1, -1, -1, 0, false);
			}
			coordY = cpuMove.getRow();
			coordX = cpuMove.getColumn();
			dir = cpuMove.getDirection();
		}

		// move tokens
		legal = gameMove(coordY, coordX, dir, tokOwn);

		draw(); // update
		
		if (((currentPlayer && black) || (!currentPlayer && !black)) && !select)
		{
			cout << "CPU moves: " << coordY+1;
			if (coordX == 0) cout << "A";
			else if (coordX == 1) cout << "B";
			else if (coordX == 2) cout << "C";
			else if (coordX == 3) cout << "D";
			cout << " " << dir << endl;
		}

		// next player's turn if legal move was perfomed
		if (legal)
		{
			if (currentPlayer) currentPlayer = false;
			else currentPlayer = true;
		}
		else if ((currentPlayer && black) || (!currentPlayer && !black))
		{
			compEnd = true; // computer made an illegal move and loses
		}

		if (!currentPlayer) end = checkVictory('B');
		else end = checkVictory('W');
	}

	if (currentPlayer) currentPlayer = false;
	else currentPlayer = true;

	cout << "Player " << currentPlayer + 1 << " Wins!" << endl << endl;
}

int main()
{
	game();

	system("PAUSE");

	return 0;
}