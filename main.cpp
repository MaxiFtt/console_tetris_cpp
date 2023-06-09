#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>
using namespace std::literals::chrono_literals;

std::wstring tetromino[7];
int fieldWidth = 12;
int fieldHeight = 18;
unsigned char gameField[216];
unsigned char bufGameField[216];

int rotateIndex(int px, int py, int r){
	int pi;
	switch (r % 4){
		case 0: pi =  py * 4 + px;// 0
				break;
		case 1: pi = 12 + py - (px * 4); //90
				break;
		case 2: pi = 15 - (py * 4) - px; //180
				  break;
		case 3: pi = 3 - py + (px * 4); //270
				  break;
	}
	return pi;
}
void rotatePiece(std::wstring &currentPiece,int tIndex, int r){
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			currentPiece[4*y+x] = (char)tetromino[tIndex][rotateIndex(x,y,r)];
		}
	}
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
	for(int y = 0; y < fieldHeight; y++){
		for(int x = 0; x < fieldWidth; x++){
			mvaddch(y,x,bufGameField[fieldWidth*y+x]);
		}
	}
}
bool doesItfit(std::wstring currentPiece, int posX, 
		int posY, int evalX,int evalY, 
		bool testRotation = false,int tIndex = 0, int r = 0){
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			if(testRotation == true){
				rotatePiece(currentPiece, tIndex, r+1);
			}
			if(gameField[fieldWidth*(y+posY+evalY)+x+posX+evalX] != ' ' && currentPiece[4*y+x] > 46){
				return false;
			}
		}
	}
	return true;
}
void showTetromino( std::wstring currentPiece, int posX, int posY)
{
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			if(currentPiece[4*y+x] != '.'){
				bufGameField[fieldWidth*(y+posY)+x+posX] = (char)currentPiece[4*y +x];		
			}
		}
	}
}
void lockTetromino( std::wstring currentPiece, int posX, int posY)
{
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			if(currentPiece[4*y+x] != '.'){
				gameField[fieldWidth*(y+posY)+x+posX] = (char)currentPiece[4*y +x];		
			}
		}
	}
}
void moveLines(int Yindex){
	for(int y = Yindex; y > 0; y--){
		for(int x = 0; x < fieldWidth -2; x++){ //field without borders
			gameField[fieldWidth*y+x+1] = gameField[fieldWidth*(y-1)+x+1]; 
			gameField[fieldWidth*(y-1)+x+1] = ' '; 
		}
	}
}
void fullLines(int &points){
	int countInLine;
	for(int y = 0; y < fieldHeight -1; y++){
		countInLine = 0;
		for(int x = 0; x < fieldWidth -2; x++){ //field without borders
			countInLine += gameField[fieldWidth*y+x+1] != ' ' ? 1 : 0;
			if(countInLine == 10){
				moveLines(y);
				points++;
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
	srand(time(NULL));
	bool gameOver = false;
	int tIndex = rand() % 7;
	std::wstring currentPiece = tetromino[tIndex];
	int points = 0;
	int posX = 4;
	int posY = 0;
	int rotation = 0;
  int cyclesCount = 0;
	char input;
  bool anyInput = false;
	//---
  //ncurses
  initscr();
  noecho();
  raw();
  build_scenario();
		gameFieldToBuf();
		showTetromino(currentPiece,posX,posY);
		showBufGameField();
  refresh();
	// do{
	// 	gameFieldToBuf();
	// 	showTetromino(currentPiece,posX,posY);
	// 	showBufGameField();
  //   input = getch();
	// 	//controls
	// 	if(anyInput == true){
  //           if(doesItfit(currentPiece,posX,posY,-1,0)){
  //               posX += input == 'a' ? -1: 0; //left
  //           }
  //           if(doesItfit(currentPiece,posX,posY,1,0)){
  //               posX += input == 'd' ? 1 : 0; //right
  //           }
  //           if(doesItfit(currentPiece,posX,posY,0,1)){
  //               posY += input == 's' ? 1: 0; //down
  //           }
  //           if(doesItfit(currentPiece,posX,posY,0,0,true,tIndex,rotation)){
  //               rotation += input == 'w' ? 1:0;
  //               rotatePiece(currentPiece,tIndex,rotation);//rotate
  //           }
  //       }
	// 	//---
	// 	std::this_thread::sleep_for(5ms);
  //       cyclesCount++;
	// 	if(doesItfit(currentPiece,posX,posY,0,1)){
  //           if(cyclesCount % 200 == 0){
  //               posY++;
  //           }
	// 	}else{ //in ground? lock piece in place
	// 		lockTetromino(currentPiece,posX,posY);
	// 		fullLines(points);
	// 		gameOver = posY == 0 ? true : false;
	// 		tIndex = rand() % 7;
	// 		currentPiece = tetromino[tIndex];
	// 		rotation = 0;
	// 		posX=4;
	// 		posY=0;
	// 	}
  //   refresh();
	// 	// system("clear");
	// }while(gameOver == false);
	// // system("clear");
	// refresh();
	// std::cout << "Game Over!" << '\n';
	// std::cout << "Your points: " << points;
    return 0;
}
