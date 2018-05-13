#include "windows.h"
#include <fstream>
#include "iostream"
#include"conio.h"
#define HEIGHT 17
#define WIDTH 21
using namespace std;
void hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

enum ConsoleColor
{
	Black, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray,
	LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
};
void SetColor(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void goCursorTo(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

char symbolOnPosition(int x, int y)
{
	goCursorTo(x, y);
	char c = '\0';
	CONSOLE_SCREEN_BUFFER_INFO con;
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hcon != INVALID_HANDLE_VALUE &&
		GetConsoleScreenBufferInfo(hcon, &con))
	{
		DWORD read = 0;
		if (!ReadConsoleOutputCharacterA(hcon, &c, 1,
			con.dwCursorPosition, &read) || read != 1
			)
			c = '\0';
	}
	return c;
}

char** uploadArrayFromFile(char*file, int FieldHeight, int FieldWidth) {
	ifstream fin("Field.txt");
	char**arr = new char*[FieldHeight];
	for (int i = 0; i < FieldHeight; i++) {
		arr[i] = new char[FieldWidth];
	}
	char temp;
	for (int i = 0; i < FieldHeight; i++) {
		for (int j = 0; j < FieldWidth; j++) {
			fin.get(temp);
			if (temp != '\n')
				arr[i][j] = temp;
			else j--;
		}

	}
	return arr;
}

void print(char**arr, int FieldHeight, int FieldWidth) {
	for (int i = 0; i < FieldHeight; i++) {
		for (int j = 0; j < FieldWidth; j++) {
			if (arr[i][j] == 'C') {
				SetColor(LightRed, Black);
				cout << arr[i][j];
				SetColor(White, Black);
				continue;
			}
			else if (arr[i][j] == '1') {
				SetColor(LightGray, Black);
				cout << arr[i][j];
				SetColor(White, Black);
				continue;
			}
			else if (arr[i][j] == '2') {
				SetColor(LightGreen, Black);
				cout << arr[i][j];
				SetColor(White, Black);
				continue;
			}
			else if (arr[i][j] == '3') {
				SetColor(LightCyan, Black);
				cout << arr[i][j];
				SetColor(White, Black);
				continue;
			}
			else if (arr[i][j] == '4') {
				SetColor(Yellow, Black);
				cout << arr[i][j];
				SetColor(White, Black);
				continue;
			}
			else if (arr[i][j] == '5') {
				SetColor(Magenta, Black);
				cout << arr[i][j];
				SetColor(White, Black);
				continue;
			}
			else if (arr[i][j] == 'o') {
				SetColor(LightRed, Black);
				cout << arr[i][j];
				SetColor(White, Black);
				continue;
			}
			cout << arr[i][j];
		}
		cout << endl;
	}
}

struct Point {
	int x;
	int y;
	Point() {
		x = 0; y = 0;
	}
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

struct Road {
	Point* road;
	int length;
};

bool neighboursPP(int**tempMap, int num, int& FieldHeight, int& FieldWidth, Point&end) {
	bool result = false;

	for (int i = 0; i < FieldHeight; i++)
		for (int j = 0; j < FieldWidth; j++) {
			if (tempMap[i][j] == num) {
				if (i == end.y&&j == end.x) {
					result = true;
					return result;
				}

				//print(tempMap, 10, 10);
				if (i > 0) {//вверх
					if (tempMap[i - 1][j] == 1 << 30) {
						tempMap[i - 1][j] = tempMap[i][j] + 1;
					}
				}
				if (j > 0) {//слева
					if (tempMap[i][j - 1] == 1 << 30) {
						tempMap[i][j - 1] = tempMap[i][j] + 1;
					}
				}

				if (i < FieldHeight - 1) {//снизу
					if (tempMap[i + 1][j] == 1 << 30) {
						tempMap[i + 1][j] = tempMap[i][j] + 1;
					}
				}

				if (j < FieldWidth - 1) {//справа
					if (tempMap[i][j + 1] == 1 << 30) {
						tempMap[i][j + 1] = tempMap[i][j] + 1;
					}
				}
			}

		}

	return result;
}
void reverseRoad(int**tempMap, Point*road, int arr_length, int& FieldHeight, int& FieldWidth, Point end) {
	road[0] = end;
	for (int i = 1; i < arr_length; i++) {
		
		if (end.x>0)//влево
			if (tempMap[end.y][end.x] - tempMap[end.y][end.x - 1] == 1) { // 01
				--end.x;
				road[i].x = end.x;
				road[i].y = end.y;

			}
		else if (end.y > 0)//вверх 
			if (tempMap[end.y][end.x] - tempMap[end.y - 1][end.x] == 1) { // 01
				--end.y;
				road[i].x = end.x;
				road[i].y = end.y;


			}
		else if (end.y<FieldHeight)//вниз  
			if (tempMap[end.y][end.x] - tempMap[end.y + 1][end.x] == 1) { // 01
				++end.y;
				road[i].x = end.x;
				road[i].y = end.y;

			}
		else if (end.x<FieldWidth)//вправо
			if (tempMap[end.y][end.x] - tempMap[end.y][end.x + 1] == 1) { // 01
				++end.x;
				road[i].x = end.x;
				road[i].y = end.y;

			}
					
	}
}
Road* RoadFromAtoB(char**map, int FieldHeight, int FieldWidth, Point begin, Point end) {
	//create Matrix with road;
	if (begin.x == end.x && begin.y == end.y)
		return NULL;
	int**tempMap = new int*[FieldHeight];
	for (int i = 0; i < FieldHeight; i++)
		tempMap[i] = new int[FieldWidth];
	//fill Matrix with road
	for (int i = 0; i < FieldHeight; i++)
		for (int j = 0; j < FieldWidth; j++)
			if (map[i][j] == '#')
				tempMap[i][j] = (1 << 30) + 1;
			else tempMap[i][j] = 1 << 30;
			//start position
			tempMap[begin.y][begin.x] = 0;
			bool exit = false;
			//wave
			for (int i = 0; i<FieldWidth*FieldHeight; i++) {
				exit = neighboursPP(tempMap, i, FieldHeight, FieldWidth, end);
				if (exit) {
					break;
				}
			}

			if (exit) {
				int road_length = tempMap[end.y][end.x] + 1;
				Road* result = new Road();
				result->road = new Point[road_length];
				result->length = road_length;
				reverseRoad(tempMap, result->road, road_length, FieldHeight, FieldWidth, end);//64
				for (int i = 0; i < FieldHeight; i++)
					delete[] tempMap[i];
				delete[] tempMap;
				return result;
			}
			else {
				for (int i = 0; i < FieldHeight; i++)
					delete[] tempMap[i];
				delete[] tempMap;
				return NULL;
			}
}


class Character {
public:
	int x;
	int y;
	char icon;
	int color;
	bool isAlive;
	Character(int x, int y, char icon, int color) {
		this->x = x;
		this->y = y;
		this->icon = icon;
		this->color = color;
		isAlive = true;
	}
	void moveTo(int x, int y) {
		goCursorTo(this->x, this->y);
		cout << " ";
		if(icon=='W')
		if (symbolOnPosition(x, y) == 'o') {
			SetColor(LightRed, Black);
			goCursorTo(this->x, this->y);
			cout << 'o';
			SetColor(White, Black);
		}
		this->x = x;
		this->y = y;
	};
	void draw() {
		goCursorTo(x, y);
		SetColor(color, Black);
		cout << icon;
		SetColor(White, Black);
		//goCursorTo(0, HEIGHT);
	}
};


void main() {
	SetColor(White, Black);
	system("chcp 1251");
	system("cls");
	hideCursor();
	char**map = uploadArrayFromFile("Field.txt", HEIGHT, WIDTH);
	print(map, HEIGHT, WIDTH);
	Road* road = NULL;
	Character hero(1,1,'C', Yellow);
	Character ghost(15, 15, 'W', LightCyan);
	float counter = 0;
	while (hero.isAlive) {

		if (_kbhit()) {
			char c = _getch();
			int offsetX = 0;
			int offsetY = 0;
			if (c == 'd' || c == 'в')
				offsetX = 1;
			else if (c == 'a' || c == 'ф')
				offsetX = -1;
			else if (c == 'w' || c == 'ц')
				offsetY = -1;
			else if (c == 's' || c == 'ы')
				offsetY = 1;

			if (symbolOnPosition(hero.x + offsetX, hero.y + offsetY) != '#'&& (offsetX!=0 || offsetY!=0)) {
				hero.moveTo(hero.x + offsetX, hero.y + offsetY);
				counter += 0.8;
				if (counter >= 1.5) {
					counter = 0;
					if (road)
						delete road;
					road = RoadFromAtoB(map, HEIGHT, WIDTH, Point(ghost.x, ghost.y), Point(hero.x, hero.y));
					if (road != NULL) {
						ghost.moveTo(road->road[road->length - 2].x, road->road[road->length - 2].y);
						if (ghost.x == hero.x &&ghost.y == hero.y)
							hero.isAlive = false;
					}
				}
			}

			fflush(stdin);
		}
		
		
		hero.draw();
		ghost.draw();
		Sleep(10);
	}
	
	

	system("pause>nul");
}