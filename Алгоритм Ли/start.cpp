#include "fstream"
#include "iostream"
#include "windows.h"

using namespace std;

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

char** uploadArrayFromFile(char*file, int FieldHeight, int FieldWidth) {
	ifstream fin("Field.txt");
	char**arr = new char*[FieldWidth]; 
	for (int i = 0; i < FieldWidth; i++) {
		arr[i] = new char[FieldHeight];
	}
	char temp;
	for (int i = 0; i < FieldHeight; i++) {
		for (int j = 0; j < FieldWidth; j++) {
			fin >> temp;
			arr[i][j] = temp;
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
			else if (arr[i][j] == '+') {
				SetColor(LightRed, Black);
				cout << arr[i][j];
				SetColor(White, Black);
				continue;
			}
			cout<<arr[i][j];
		}
		cout << endl;
	}
}

struct Point{
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
		for (int j = 0; j < FieldHeight; j++) {
			if (tempMap[i][j] == num) {
				if (i == end.y&&j == end.x) {
					result = true;
					return result;
				}
				//print(tempMap, 10, 10);
				if (i > 0) {//вверх
					if (tempMap[i - 1][j]==1<<30) {
						tempMap[i - 1][j] = tempMap[i][j]+1;
					}
				}
				if (j > 0) {//слева
					if (tempMap[i][j - 1]== 1 << 30) {
						tempMap[i][j - 1] = tempMap[i][j]+1;
					}
				}

				if (i < FieldHeight - 1) {//снизу
					if (tempMap[i + 1][j]== 1 << 30) {
						tempMap[i + 1][j] = tempMap[i][j]+1;
					}
				}

				if (j < FieldWidth - 1) {//справа
					if (tempMap[i][j + 1]== 1 << 30) {
						tempMap[i][j + 1] = tempMap[i][j]+1;
					}
				}
			}

		}


	return result;
}
void reverseRoad(int**tempMap, Point*road,int arr_length, int& FieldHeight, int& FieldWidth, Point end) {
	road[0] = end;
	for (int i = 1; i < arr_length; i++) {
		if (end.x > 0)//вверх 
			if (tempMap[end.y][end.x] - tempMap[end.y - 1][end.x] == 1) { // 01
				--end.y;
				road[i].x = end.x;
				road[i].y = end.y;

			
			}
		else if (end.y<FieldHeight - 1)//вниз  
			if (tempMap[end.y][end.x] - tempMap[end.y + 1][end.x] == 1) { // 01
				++end.y;
				road[i].x = end.x;
				road[i].y = end.y;

			}
		else if (end.x<FieldHeight - 1)//вправо
			if (tempMap[end.y][end.x] - tempMap[end.y][end.x+1] == 1) { // 01
				++end.x;
				road[i].x = end.x;
				road[i].y = end.y;

			}
		else if (end.x>0)//влево
			if (tempMap[end.y][end.x] - tempMap[end.y][end.x-1] == 1) { // 01
				--end.x;
				road[i].x = end.x;
				road[i].y = end.y;

			}
	}
}
Road* RoadTo4k(char**map, int FieldHeight, int FieldWidth, Point begin, Point end) {
	//create Matrix with road;
	int**tempMap = new int*[FieldWidth];
	for (int i = 0; i < FieldWidth; i++)
		tempMap[i] = new int[FieldHeight];
	//fill Matrix with road
	for (int i = 0; i < FieldHeight; i++)
		for (int j = 0; j < FieldWidth; j++)
			if (map[i][j] == '#')
				tempMap[i][j] = (1<<30)+1;
			else tempMap[i][j] = 1 << 30;
	//start position
	tempMap[begin.y][begin.x] = 0;
	bool exit = false; 
	//wave
	for(int i = 0; i<FieldWidth*FieldHeight; i++){
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
		reverseRoad(tempMap, result->road, road_length, FieldHeight, FieldWidth, end);
		for (int i = 0; i < road_length; i++) {
			map[result->road[i].y][result->road[i].x]='+';
		}
		return result;
	}
	else {
		return NULL;
	}
}
void main() {
	SetColor(White, Black);
	char**map = uploadArrayFromFile("Field.txt", 10, 10);

	print(map,10,10);

	Road* road = RoadTo4k(map, 10, 10, Point(2, 2), Point(1, 8));
	if (!road)
		cout << "no road" << endl;

	print(map,10,10);
	
	system("pause>nul");
}