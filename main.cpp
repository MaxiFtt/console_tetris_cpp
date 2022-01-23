#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <ctime>
using namespace std::literals::chrono_literals;

std::wstring tetromino[7];
int fieldWidth = 12;
int fieldHeight = 18;
unsigned char gameField[216];
unsigned char bufGameField[216];
int rotateIndex(int px, int py, int r){
	int pi;
	switch (r){
		case 0: pi =  py * 4 + px;// 0
				break;
		case 90: pi = 12 + py - (px * 4); //90
				break;
		case 180: pi = 15 - (py * 4) - px; //180
				  break;
		case 270: pi = 3 - py + (px * 4); //270
				  break;
	}
	return pi;
}
void build_scenario(){	
	for(int y = 0; y < fieldHeight; y++){//game borders
		for(int x = 0; x < fieldWidth; x++){
			if( x == 0 || x == fieldWidth - 1 || y == fieldHeight -1)
			{
				gameField[fieldWidth*y + x]= '#';
				bufGameField[fieldWidth*y + x]= '#';
			}else{
				gameField[fieldWidth*y + x]= ' ';
				bufGameField[fieldWidth*y +x] = ' ';
			}
		}
	}
}
void gameFieldToBuf(){
	for(int i =0; i < 216;i++){
		bufGameField[i] = gameField[i];
	}
}
void showBufGameField(){
	for(int y = 0; y < fieldHeight; y++){//game borders
		for(int x = 0; x < fieldWidth; x++){
			std::cout << bufGameField[fieldWidth*y+x];
		}
		std::cout << "\n";
	}
}
bool doesItfit(int tIndex, int posX, int posY, int evalX,int evalY){
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			if(gameField[fieldWidth*(y+posY+evalY)+x+posX+evalX] != ' ' && tetromino[tIndex][4*y+x] > 46){
				return false;
			}
		}
	}
	return true;
}
void showTetromino( int tIndex, int posX, int posY)
{
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			if(tetromino[tIndex][4*y+x] != '.'){
				bufGameField[fieldWidth*(y+posY)+x+posX] = (char)tetromino[tIndex][4*y +x];		
			}
		}
	}
}
void lockTetromino( int tIndex, int posX, int posY)
{
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			if(tetromino[tIndex][4*y+x] != '.'){
				gameField[fieldWidth*(y+posY)+x+posX] = (char)tetromino[tIndex][4*y +x];		
			}
		}
	}
}
int main(){
    tetromino[0].append(L"..I.");//pieces
    tetromino[0].append(L"..I.");
    tetromino[0].append(L"..I.");
    tetromino[0].append(L"..I.");

    tetromino[1].append(L"..B.");
    tetromino[1].append(L".BB.");
    tetromino[1].append(L".B..");
    tetromino[1].append(L"....");

    tetromino[2].append(L".V..");
    tetromino[2].append(L".VV.");
    tetromino[2].append(L"..V.");
    tetromino[2].append(L"....");

    tetromino[3].append(L".CC.");
    tetromino[3].append(L".CC.");
    tetromino[3].append(L"....");
    tetromino[3].append(L"....");

    tetromino[4].append(L"..T.");
    tetromino[4].append(L".TTT");
    tetromino[4].append(L"....");
    tetromino[4].append(L"....");

    tetromino[5].append(L"..L.");
    tetromino[5].append(L"..L.");
    tetromino[5].append(L"..LL");
    tetromino[5].append(L"....");

    tetromino[6].append(L"..J.");
    tetromino[6].append(L"..J.");
    tetromino[6].append(L".JJ.");
    tetromino[6].append(L"....");
	
	//game variables
	bool gameOver = false;
	int posX = 4;
	int posY = 0;
	srand(time(NULL));
	int tIndex = rand() % 7;
	char *input;
	//---
	build_scenario();
	do{
		gameFieldToBuf();
		showTetromino(tIndex,posX,posY);
		showBufGameField();
		std::cin >> input;
		//controls
		if(doesItfit(tIndex,posX,posY,-1,0)){
			posX += input[0] == 'a' ? -1: 0; //left
		}
		if(doesItfit(tIndex,posX,posY,1,0)){
			posX += input[0]== 'd' ? 1 : 0; //right
		}
		if(doesItfit(tIndex,posX,posY,0,1)){
			posY += input[0]== 's' ? 1: 0; //down
		}
		if(doesItfit(tIndex,posX,posY,0,1)){
			posY += input[0]== 'w' ? 1: 0; //rotate
		}
		//---
		std::this_thread::sleep_for(25ms);
		if(doesItfit(tIndex,posX,posY,0,1)){
			posY++;
		}else{ //no space? lock piece in place
			lockTetromino(tIndex,posX,posY);
			gameOver = posY == 0 ? true : false;
			tIndex = rand() % 7;
			posX=4;
			posY=0;

		}
		system("clear");
	}while(gameOver == false);

    return 0;
}
