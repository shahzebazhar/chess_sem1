#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
#include <conio.h>
using namespace std;
#include <windows.h>


void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	FlushConsoleInputBuffer(hInput);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void ChangeColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

#define GRIDSIZE 8
enum Color { BLACK, WHITE, BLANK };
struct ChessPiece {
	char Symbol;
	Color Side;
	bool HasMoved;
};

struct ChangeStruct {
	int pR;
	int pC;
	int toR;
	int toC;
	Color pSide;
	Color toSide;
	char pPiece;
	char toPiece;
};

void RunGame(ChessPiece Board[][GRIDSIZE], char Players[][31], int& strtR, int& strtC, int& strtLN, int& strtCN,
	int& BlockR, int& BlockC, ChangeStruct Changes[], int& ChangesSize, char GameFile[], int& Turn, int& Moves,
	char BoardFile[], bool& isSaveAndExit);
void PlayMain();
void ReplayMain();
void ContinueMain();
char ModeSelect();
void Play_init(ChessPiece Board[][GRIDSIZE], char Players[][31], int& Turn, char GameFile[], char BoardFile[]);
void SetBoard(ChessPiece Board[][GRIDSIZE], char BoardFile[]);
void Replay_init(ChessPiece Board[][GRIDSIZE], char GameFile[]);
void Continue_init(ChessPiece Board[][GRIDSIZE], char Players[][31], int& Turn, char GameFile[], char BoardFile[],
	ChangeStruct Changes[], int& ChangeSize);
void CreatePieces(ChessPiece& Rook, ChessPiece& Knight, ChessPiece& Bishop, 
				  ChessPiece& King, ChessPiece& Queen, ChessPiece& Pawn, ChessPiece& Empty);
void scrnCoordtoIndex(int& ri, int& ci, int strtR, int strtC, int strtLN, int strtCN, int BlockR, int BlockC);
void pSelect(ChessPiece Board[][GRIDSIZE], char Player[], int& pR, int& pC, int Turn, 
	int strtR, int strtC, int strtLN, int strtCN, int BlockR, int BlockC);
void toSelect(ChessPiece Board[][GRIDSIZE], int& toR, int& toC, 
	int strtR, int strtC, int strtLN, int strtCN, int BlockR, int BlockC);
bool isValidMove(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC, int Turn);
void UpdateBoard(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC);
void DisplayBoard(ChessPiece Board[][GRIDSIZE], int strtR, int strtC, int strtLN, int strtCN, int BlockR, int BlockC);
void DisplayPiece(ChessPiece Piece);
char toLower(char c);
bool isStraightPathClear(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC);
bool isDiagPathClear(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC);
bool RookValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC);
int Absolute(int N);
bool BishopValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC);
bool KingValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC, int Turn);
bool QueenValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC);
bool KnightValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC);
bool PawnValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC, int Turn);
bool isEnemyOnLocation(ChessPiece Board[][GRIDSIZE], int toR, int toC, int Turn);
bool isFriendlyOnLocation(ChessPiece Board[][GRIDSIZE], int toR, int toC, int Turn);
bool isOutofBounds(int toR, int toC);
bool isKingCheck(ChessPiece Board[][GRIDSIZE], int toR, int toC, int Turn);
bool isPieceCheck(ChessPiece Board[][GRIDSIZE], int toR, int toC, int Turn);
void GetKing(ChessPiece Board[][GRIDSIZE], int& kR, int& kC, int Turn);
void PawnPromotion(ChessPiece Board[][GRIDSIZE], int ri, int ci);
bool isCheckMate(ChessPiece Board[][GRIDSIZE], int kR, int kC, int eR, int eC, int Turn, bool isCheck);
bool NonCheckMove(ChessPiece Board[][GRIDSIZE], int kR, int kC, int pR, int pC, int toR, int toC, int Turn);
void GetisCheckEnemy(ChessPiece Board[][GRIDSIZE], int& eR, int& eC, int kR, int kC, int Turn);
bool CanBlockPath(ChessPiece Board[][GRIDSIZE], int eR, int eC, int kR, int kC, int Turn);
void DisplayPath(ChessPiece Board[][GRIDSIZE], int pR, int pC, int Turn,
	int strtR, int strtC, int strtLN, int strtCN, int BlockR, int BlockC);
char toUpper(char c);
void SaveChange(ChangeStruct Changes[], ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC, int& ChangesSize);
void WriteChanges(ChangeStruct Changes[], int ChangesSize, char GameFile[]);
void LoadChanges(ChangeStruct Changes[], int& ChangeSize, char GameFile[]);
void ApplyChange(ChessPiece Board[][GRIDSIZE], ChangeStruct Change, char Action);
void NameToFile(char GameFile[]);
int chartoInt(char c);
bool isLower(char c);
bool isUpper(char c);
bool CanCastle(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC);
void ChangeMoveState(ChessPiece& Piece);
void PerformCastling(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC);
void DisplayButtons(int undoR, int undoC, int saeR, int saeC, int restartR, int restartC);
bool UndoClicked(int scrnR, int scrnC, int undoR, int undoC);
bool SaveAndExitClicked(int scrnR, int scrnC, int saeR, int saeC);
bool RestartClicked(int scrnR, int scrnC, int restartR, int restartC);
bool Undo(ChessPiece Board[][GRIDSIZE], ChangeStruct Changes[], int& ChangesSize);
void SaveAndExit(ChangeStruct Changes[], int ChangesSize, char GameFile[]);
void Restart(int &ChangesSize);

int main() {

	char Selection = ModeSelect();
	system("cls");
	switch (Selection) {
	case 'R': ReplayMain(); break;
	case 'P': PlayMain(); break;
	case 'C': ContinueMain(); break;
	}
	cout << endl;
	system("pause");
	return 0;
}

void PlayMain() {
	srand(time(0));
	ChessPiece Board[GRIDSIZE][GRIDSIZE];
	
	ChangeStruct Changes[100];
	int ChangesSize = 0;
	char GameFile[31];
	char Players[2][31];
	int Turn;
	bool isSaveAndExit = false;
	
	int strtR = 0, strtC = 0, strtLN = 1, strtCN = 1, BlockR = 1, BlockC = 3;
	int Moves = 0;
	
	
	char BoardFile[] = { "Board.txt" };
	

	Play_init(Board, Players, Turn, GameFile, BoardFile);
	
	NameToFile(GameFile);
	
	RunGame(Board, Players, strtR, strtC, strtLN, strtCN, BlockR, BlockC, Changes, ChangesSize, GameFile, Turn, Moves,
		BoardFile, isSaveAndExit);

	if (isSaveAndExit)
		return;

	DisplayBoard(Board, strtR, strtC, strtLN, strtCN, BlockR, BlockC);
	cout << endl;
	cout << "CheckMate!" << endl;
	WriteChanges(Changes, ChangesSize, GameFile);
	Turn = (Turn + 1) % 2;
	cout << Players[Turn] << " won the match in " << Moves / 2 + Turn << " moves!" << endl;

}

void ReplayMain() {
	ChessPiece Board[GRIDSIZE][GRIDSIZE];
	int strtR = 0, strtC = 0, strtLN = 1, strtCN = 1, BlockR = 1, BlockC = 3;
	int i = -1;
	char GameFile[31], Action = 0;
	int ChangeSize;
	ChangeStruct Changes[100];
	Replay_init(Board, GameFile);
	NameToFile(GameFile);
	LoadChanges(Changes, ChangeSize, GameFile);
	cout << "CONTROLS:" << endl << endl << "SPACE BAR for previous move." << endl << "ENTER for next move." << endl
		<< "ESC to exit." << endl;
	cout << endl << "Press a key to start the game replay." << endl;
	_getch();

	while (true) {
		DisplayBoard(Board, strtR, strtC, strtLN, strtCN, BlockR, BlockC);
		
		if (i == ChangeSize - 1)
			cout << endl << "CheckMate!" << endl;


		Action = _getch();
		if (Action == 27)
			return;
		else if (Action == 13) {
			i++;
			if (i == ChangeSize) {
				for (i = i - 1; i > -1; i--)
					ApplyChange(Board, Changes[i], 32);
			}
			else {
				ApplyChange(Board, Changes[i], Action);
			}
		}
		else if (Action == 32) {

			if (i == -1) {
				for (i = 0; i < ChangeSize; i++) {
					ApplyChange(Board, Changes[i], 13);
				}
			}
			else {
				ApplyChange(Board, Changes[i], Action);
			}
			i--;
		}
		else {
			continue;
		}

	}


}

void ContinueMain() {
	srand(time(0));
	ChessPiece Board[GRIDSIZE][GRIDSIZE];

	ChangeStruct Changes[100];
	int ChangesSize = 0;
	char GameFile[31];
	char Players[2][31];
	int Turn;
	bool isSaveAndExit = false;
	int kR, kC, eR, eC;
	bool isCheck;


	int strtR = 0, strtC = 0, strtLN = 1, strtCN = 1, BlockR = 1, BlockC = 3;
	int Moves = 0;

	char BoardFile[] = { "Board.txt" };
	
	Continue_init(Board, Players, Turn, GameFile, BoardFile, Changes, ChangesSize);
	Moves = ChangesSize;

	GetKing(Board, kR, kC, Turn);
	GetisCheckEnemy(Board, eR, eC, kR, kC, Turn);
	isCheck = isKingCheck(Board, kR, kC, Turn);

	if (!isCheckMate(Board, kR, kC, eR, eC, Turn, isCheck)) {
		RunGame(Board, Players, strtR, strtC, strtLN, strtCN, BlockR, BlockC, Changes, ChangesSize, GameFile, Turn, Moves,
			BoardFile, isSaveAndExit);
	}

	if (isSaveAndExit)
		return;

	DisplayBoard(Board, strtR, strtC, strtLN, strtCN, BlockR, BlockC);
	cout << endl;
	cout << "CheckMate!" << endl;
	WriteChanges(Changes, ChangesSize, GameFile);
	Turn = (Turn + 1) % 2;
	cout << Players[Turn] << " won the match in " << Moves / 2 + Turn << " moves!" << endl;

}

void RunGame(ChessPiece Board[][GRIDSIZE], char Players[][31], int& strtR, int& strtC, int& strtLN, int& strtCN, 
	int& BlockR, int& BlockC, ChangeStruct Changes[], int& ChangesSize, char GameFile[], int& Turn, int& Moves,
	char BoardFile[], bool& isSaveAndExit) {

	ChessPiece P;
	int pR, pC, toR, toC, kR = -1, kC = -1, eR = -1, eC = -1;
	bool isCheck = false, isCastling = false;
	int undoR = 1, undoC = 33, saeR = 4, saeC = 33, restartR = 7, restartC = 33;
	bool isUndo = false, isRestart = false;

	do {
		DisplayBoard(Board, strtR, strtC, strtLN, strtCN, BlockR, BlockC);
		DisplayButtons(undoR, undoC, saeR, saeC, restartR, restartC);
		gotoRowCol(strtR + strtLN + GRIDSIZE * BlockR, 0);
		cout << endl;


		if (isCheck) {
			cout << "Check!" << endl;
		}
		while (true) {
			while (true) {
				pSelect(Board, Players[Turn], pR, pC, Turn, strtR, strtC, strtLN, strtCN, BlockR, BlockC);

				if (UndoClicked(pR, pC, undoR, undoC)) {
					if (Undo(Board, Changes, ChangesSize)) {
						Turn = (Turn + 1) % 2;
						isUndo = true;
						break;
					}
					else {
						cout << "No move was played" << endl;
						continue;
					}
				}
				else if (SaveAndExitClicked(pR, pC, saeR, saeC)) {
					SaveAndExit(Changes, ChangesSize, GameFile);
					isSaveAndExit = true;
					return;
				}
				else if (RestartClicked(pR, pC, restartR, restartC)) {
					Restart(ChangesSize);
					SetBoard(Board, BoardFile);
					isRestart = true;
					Turn = 1;
					break;
				}

				scrnCoordtoIndex(pR, pC, strtR, strtC, strtLN, strtCN, BlockR, BlockC);
				DisplayPath(Board, pR, pC, Turn, strtR, strtC, strtLN, strtCN, BlockR, BlockC);
				toSelect(Board, toR, toC, strtR, strtC, strtLN, strtCN, BlockR, BlockC);
				scrnCoordtoIndex(toR, toC, strtR, strtC, strtLN, strtCN, BlockR, BlockC);

				DisplayBoard(Board, strtR, strtC, strtLN, strtCN, BlockR, BlockC);
				DisplayButtons(undoR, undoC, saeR, saeC, restartR, restartC);
				gotoRowCol(strtR + strtLN + GRIDSIZE * BlockR, 0);
				cout << endl;
				if (isOutofBounds(toR, toC)) {
					cout << "Your Move is out of bounds." << endl;
					continue;
				}
				isCastling = CanCastle(Board, pR, pC, toR, toC);
				if (!isCastling) {
					if (!isValidMove(Board, pR, pC, toR, toC, Turn))
						cout << "Invalid Move" << endl;
					else
						break;
				}
				else
					break;
			}
			if (isUndo || isRestart)
				break;

			P = Board[toR][toC];
			SaveChange(Changes, Board, pR, pC, toR, toC, ChangesSize);
			ChangeMoveState(Board[pR][pC]);

			if (!isCastling)
				UpdateBoard(Board, pR, pC, toR, toC);
			else
				PerformCastling(Board, pR, pC, toR, toC);

			PawnPromotion(Board, toR, toC);
			GetKing(Board, kR, kC, Turn);
			isCheck = isKingCheck(Board, kR, kC, Turn);
			if (isCheck) {
				cout << "Invalid Move, Please Remove Check!" << endl;
				Board[pR][pC] = Board[toR][toC];
				Board[toR][toC] = P;
			}
			else {
				break;
			}
		}
		if (isUndo || isRestart) {
			isUndo = false, isRestart = false;
			continue;
		}
		Moves++;
		Turn = (Turn + 1) % 2;
		GetKing(Board, kR, kC, Turn);
		GetisCheckEnemy(Board, eR, eC, kR, kC, Turn);
		isCheck = isKingCheck(Board, kR, kC, Turn);

	} while (!isCheckMate(Board, kR, kC, eR, eC, Turn, isCheck));
}

char ModeSelect() {
	char Selection;
	cout << "Select a mode \n\n Enter R to replay a game. \n\n Enter P to play a game. \n\n Enter C to continue an old game." 
		<< endl << endl;
	while (true) {
		cin >> Selection;
		cin.ignore();
		Selection = toUpper(Selection);
		if (Selection != 'R' && Selection != 'P' && Selection != 'C')
			cout << "Invalid Input. Select Again: " << endl;
		else
			break;
	}

	return Selection;
}

void SetBoard(ChessPiece Board[][GRIDSIZE], char BoardFile[]) {
	ifstream fin(BoardFile);
	/*
	if (fin.good()) {
		for (int ri = 0; ri < GRIDSIZE; ri++) {
			for (int ci = 0; ci < GRIDSIZE; ci++) {
				fin >> Board[ri][ci].Symbol;
				Board[ri][ci].HasMoved = false;
				if (Board[ri][ci].Symbol == '-')
					Board[ri][ci].Side = BLANK;
				else if (isUpper(Board[ri][ci].Symbol))
					Board[ri][ci].Side = BLACK;
				else if (isLower(Board[ri][ci].Symbol)) {
					Board[ri][ci].Symbol = toUpper(Board[ri][ci].Symbol);
					Board[ri][ci].Side = WHITE;
				}
			}
		}
	}
	*/

	for (int ri = 0; ri < GRIDSIZE; ri++) {
		for (int ci = 0; ci < GRIDSIZE; ci++) {
			if (ri == 0 || ri == 7) {
				switch (ci) {
				case 0:
				case 7:
					Board[ri][ci].Symbol = 'R';
					break;
				case 1:
				case 6:
					Board[ri][ci].Symbol = 'H';
					break;
				case 2:
				case 5:
					Board[ri][ci].Symbol = 'B';
					break;
				case 3:
					Board[ri][ci].Symbol = 'Q';
					break;
				case 4:
					Board[ri][ci].Symbol = 'K';
					break;
				}
			}
			else if (ri == 1 || ri == 6)
				Board[ri][ci].Symbol = 'P';
			else
				Board[ri][ci].Symbol = '-';

			if (ri == 0 || ri == 1)
				Board[ri][ci].Side = BLACK;
			else if (ri == 6 || ri == 7)
				Board[ri][ci].Side = WHITE;
			else
				Board[ri][ci].Side = BLANK;

			Board[ri][ci].HasMoved = false;
		}
	}

}

void Play_init(ChessPiece Board[][GRIDSIZE], char Players[][31], int& Turn, char GameFile[], char BoardFile[]) {
	cout << "Player 1 Name(Max 31 characters): ";
	cin.getline(Players[0], 31);
	cout << "Player 2 Name(Max 31 characters): ";
	cin.getline(Players[1], 31);

	cout << "Enter Game Name(Max 31 characters): ";
	cin.getline(GameFile, 31);

	SetBoard(Board, BoardFile);

	Turn = 1;


}

void Replay_init(ChessPiece Board[][GRIDSIZE], char GameFile[]) {
	char BoardFile[] = { "Board.txt" };
	cout << "Enter Game Name(Max 31 characters): ";
	cin.getline(GameFile, 31);

	SetBoard(Board, BoardFile);
}

void Continue_init(ChessPiece Board[][GRIDSIZE], char Players[][31], int& Turn, char GameFile[], char BoardFile[],
	ChangeStruct Changes[], int& ChangeSize) {
	cout << "Player 1 Name(Max 31 characters): ";
	cin.getline(Players[0], 31);
	cout << "Player 2 Name(Max 31 characters): ";
	cin.getline(Players[1], 31);

	cout << "Enter Game Name(Max 31 characters): ";
	cin.getline(GameFile, 31);
	NameToFile(GameFile);

	SetBoard(Board, BoardFile);

	LoadChanges(Changes, ChangeSize, GameFile);
	for (int i = 0; i < ChangeSize; i++)
		ApplyChange(Board, Changes[i], 13);

	Turn = Changes[ChangeSize - 1].pSide;
	Turn = (Turn + 1) % 2;
}


void CreatePieces(ChessPiece& Rook, ChessPiece& Knight, ChessPiece& Bishop,
				  ChessPiece& King, ChessPiece& Queen, ChessPiece& Pawn, ChessPiece& Empty) {
	Rook.Symbol = 'R';

	Knight.Symbol = 'H';

	Bishop.Symbol = 'B';

	King.Symbol = 'K';

	Queen.Symbol = 'Q';

	Pawn.Symbol = 'P';

	Empty.Symbol = '-';
}


void DisplayBoard(ChessPiece Board[][GRIDSIZE],
	int strtR, int strtC, int strtLN, int strtCN, int BlockR, int BlockC) {
	
	system("cls");

	gotoRowCol(strtR, strtC);
	cout << " ";
	for (int i = 0; i < BlockC * GRIDSIZE; i++)
		cout << "_";

	for (int i = 0; i <= strtLN - 1; i++)
		cout << endl;

	for (int ri = 0; ri < GRIDSIZE; ri++) {

		cout << "|";

		for (int i = 0; i < BlockR / 2; i++)
			cout << endl;

		for (int ci = 0; ci < GRIDSIZE; ci++) {

			for (int i = 0; i < BlockC / 2; i++)
				cout << " ";

			DisplayPiece(Board[ri][ci]);

			for (int i = (BlockC / 2) + 1; i < BlockC; i++)
				cout << " ";

			if (BlockC == 0)
				cout << " ";
		}
		cout << "|";
		for (int i = BlockR / 2 + 1; i <= BlockR; i++)
			cout << endl;

		if (BlockR == 0)
			cout << endl;

	}
	cout << "|";
	for (int i = 0; i < BlockC * GRIDSIZE; i++)
		cout << "_";
	cout << "|";

}


void DisplayPiece(ChessPiece Piece) {
	if (Piece.Side == WHITE)
		ChangeColor(7);
	else if (Piece.Side == BLACK)
		ChangeColor(11);
	cout << Piece.Symbol;
	ChangeColor(15);
}


char toLower(char c) {
	if (c >= 65 && c <= 90)
		return c + 32;
	return c;
}

void scrnCoordtoIndex(int& ri, int& ci, int strtR, int strtC, int strtLN, int strtCN, int BlockR, int BlockC) {
	ri -= strtR;
	ri -= strtLN;
	ri /= BlockR;

	ci -= strtC;
	ci -= strtCN;
	ci /= BlockC;
}

void pSelect(ChessPiece Board[][GRIDSIZE], char Player[], int& pR, int& pC, int Turn,
	int strtR, int strtC, int strtLN, int strtCN, int BlockR, int BlockC) {
	cout << Player << "'s(";
	if (Turn)
		cout << "WHITE) TURN" << endl;
	else
		cout << "BLACK) TURN" << endl;
	getRowColbyLeftClick(pR, pC);

}

void toSelect(ChessPiece Board[][GRIDSIZE], int& toR, int& toC,
	int strtR, int strtC, int strtLN, int strtCN, int BlockR, int BlockC) {
	getRowColbyLeftClick(toR, toC);

}


void UpdateBoard(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC) {
	Board[toR][toC] = Board[pR][pC];
	ChessPiece Empty;
	Empty.Symbol = '-';
	Empty.Side = BLANK;
	Board[pR][pC] = Empty;
}


bool isValidMove(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC, int Turn) {

	if (Board[pR][pC].Side != Turn)
		return false;

	if (pR == toR && pC == toC)
		return false;

	if (isFriendlyOnLocation(Board, toR, toC, Turn))
		return false;
	switch (Board[pR][pC].Symbol) {
	case 'R':
		return RookValidation(Board, pR, pC, toR, toC);
	case 'H':
		return KnightValidation(Board, pR, pC, toR, toC);
	case 'B':
		return BishopValidation(Board, pR, pC, toR, toC);
	case 'Q':
		return QueenValidation(Board, pR, pC, toR, toC);
	case 'K':
		return KingValidation(Board, pR, pC, toR, toC, Turn);
	case 'P':
		return PawnValidation(Board, pR, pC, toR, toC, Turn);
	}
	return false;
}


bool isStraightPathClear(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC) {
	for (int i = pR + 1; i < toR; i++) {
		if (Board[i][pC].Symbol != '-')
			return false;
	}

	for (int i = pC + 1; i < toC; i++) {
		if (Board[pR][i].Symbol != '-')
			return false;
	}

	for (int i = toR + 1; i < pR; i++) {
		if (Board[i][toC].Symbol != '-')
			return false;
	}

	for (int i = toC + 1; i < pC; i++) {
		if (Board[toR][i].Symbol != '-')
			return false;
	}

	return true;
}


bool isDiagPathClear(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC) {
	if (toC < pC) {
		for (int i = 1; toR + i < pR; i++) {
			if (Board[toR + i][toC + i].Symbol != '-')
				return false;
		}
		for (int i = 1; pR + i < toR; i++) {
			if (Board[pR + i][pC - i].Symbol != '-')
				return false;
		}
	}
	else if (pC < toC) {
		for (int i = 1; toR + i < pR; i++) {
			if (Board[toR + i][toC - i].Symbol != '-')
				return false;
		}
		for (int i = 1; pR + i < toR; i++) {
			if (Board[pR + i][pC + i].Symbol != '-')
				return false;
		}
	}

	return true;
}


bool RookValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC) {
	bool flag = false;
	if (pR == toR)
		flag = true;
	if (pC == toC) {
		if (!flag)
			flag = true;
	}

	return flag && isStraightPathClear(Board, pR, pC, toR, toC);
}

int Absolute(int N) {
	if (N < 0)
		return N * -1;
	return N;
}

bool BishopValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC) {
	bool flag = false;
	if (Absolute(toR - pR) == Absolute(toC - pC) && toR - pR != 0 && toC - pC != 0)
		flag = true;
	return flag && isDiagPathClear(Board, pR, pC, toR, toC);
}

bool KingValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC, int Turn) {
	if (Absolute(toR - pR) > 1)
		return false;
	if (Absolute(toC - pC) > 1)
		return false;
	if (isKingCheck(Board, toR, toC, Turn))
		return false;
	return true;
}

bool QueenValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC) {
	return RookValidation(Board, pR, pC, toR, toC) || BishopValidation(Board, pR, pC, toR, toC);
}


bool KnightValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC) {
	if (Absolute(toR - pR) == 2 && Absolute(toC - pC) == 1)
		return true;
	if (Absolute(toR - pR) == 1 && Absolute(toC - pC) == 2)
		return true;
	return false;


}
bool PawnValidation(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC, int Turn) {
	if (Board[pR][pC].Side == BLACK) {
		if (toR - pR == 1) {
			if (toC - pC == 0) {
				if (!isEnemyOnLocation(Board, toR, toC, Turn))
					return true;
			}
			else if (Absolute(toC - pC) == 1) {
				if (isEnemyOnLocation(Board, toR, toC, Turn))
					return true;
			}
		}
		else if (toR - pR == 2 && Absolute(toC - pC) == 0 && !isEnemyOnLocation(Board, toR - 1, toC, Turn)
			&& !isEnemyOnLocation(Board, toR, toC, Turn) && pR == 1) {
			return true;
		}
	}
	else {
		if (pR - toR == 1) {
			if (toC - pC == 0) {
				if (!isEnemyOnLocation(Board, toR, toC, Turn))
					return true;
			}
			else if (Absolute(toC - pC) == 1) {
				if (isEnemyOnLocation(Board, toR, toC, Turn))
					return true;
			}
		}
		else if (pR - toR == 2 && Absolute(toC - pC) == 0 && !isEnemyOnLocation(Board, toR + 1, toC, Turn)
			&& !isEnemyOnLocation(Board, toR, toC, Turn) && pR == 6) {
			return true;
		}
	}

	return false;
}

bool isEnemyOnLocation(ChessPiece Board[][GRIDSIZE], int toR, int toC, int Turn) {
	if (Board[toR][toC].Symbol == '-')
		return false;
	if (Board[toR][toC].Side == Turn)
		return false;
	return true;
}

bool isFriendlyOnLocation(ChessPiece Board[][GRIDSIZE], int toR, int toC, int Turn) {
	if (Board[toR][toC].Symbol == '-')
		return false;
	if (Board[toR][toC].Side == Turn)
		return true;
	return false;
}

bool isOutofBounds(int toR, int toC) {
	return (toR < 0 || toR >= GRIDSIZE || toC < 0 || toC >= GRIDSIZE);
}

bool isKingCheck(ChessPiece Board[][GRIDSIZE], int toR, int toC, int Turn) {
	int kR, kC;
	if (toR == -1 || toC == -1)
		return false;
	ChessPiece P = Board[toR][toC];
	GetKing(Board, kR, kC, Turn);
	Board[toR][toC] = Board[kR][kC];
	Board[kR][kC].Symbol = '-';
	Board[kR][kC].Side = BLANK;
	Turn = (Turn + 1) % 2;
	for (int ri = 0; ri < GRIDSIZE; ri++) {
		for (int ci = 0; ci < GRIDSIZE; ci++) {
			if (Board[ri][ci].Symbol != '-' && Board[ri][ci].Side == Turn && isValidMove(Board, ri, ci, toR, toC, Turn)) {
				Board[kR][kC] = Board[toR][toC];
				Board[toR][toC] = P;
				return true;
			}
		}
	}
	Turn = (Turn + 1) % 2;
	Board[kR][kC] = Board[toR][toC];
	Board[toR][toC] = P;
	return false;
}

bool isPieceCheck(ChessPiece Board[][GRIDSIZE], int toR, int toC, int Turn) {
	int kR, kC;
	GetKing(Board, kR, kC, Turn);
	Turn = (Turn + 1) % 2;
	for (int ri = 0; ri < GRIDSIZE; ri++) {
		for (int ci = 0; ci < GRIDSIZE; ci++) {
			if (Board[ri][ci].Side == Turn && isValidMove(Board, ri, ci, toR, toC, Turn) && 
				NonCheckMove(Board, kR, kC, ri, ci, toR, toC, Turn))
				return true;
		}
	}
	return false;
}


void GetKing(ChessPiece Board[][GRIDSIZE], int& kR, int& kC, int Turn) {
	for (int ri = 0; ri < GRIDSIZE; ri++) {
		for (int ci = 0; ci < GRIDSIZE; ci++) {
			if (Board[ri][ci].Symbol == 'K' && Board[ri][ci].Side == Turn) {
				kR = ri, kC = ci;
				return;
			}
		}
	}
}

	
bool NonCheckMove(ChessPiece Board[][GRIDSIZE], int kR, int kC, int pR, int pC, int toR, int toC, int Turn) {
	ChessPiece P = Board[toR][toC];
	ChessPiece blank;
	blank.Symbol = '-';
	blank.Side = BLANK;
	Board[toR][toC] = Board[pR][pC];
	Board[pR][pC] = blank;
	Turn = (Turn + 1) % 2;

	for (int ri = 0; ri < GRIDSIZE; ri++) {
		for (int ci = 0; ci < GRIDSIZE; ci++) {
			if (Board[ri][ci].Side == Turn && isValidMove(Board, ri, ci, kR, kC, Turn)) {
				Board[pR][pC] = Board[toR][toC];
				Board[toR][toC] = P;
				return false;
			}
		}
	}
	Board[pR][pC] = Board[toR][toC];
	Board[toR][toC] = P;
	return true;
}

void PawnPromotion(ChessPiece Board[][GRIDSIZE], int ri, int ci) {

	if (Board[ri][ci].Symbol != 'P')
		return;

	char promoteTo;

	if (ri == 0 || ri == 7) {
		while (true) {
			cout << "What do you want to select your pawn to?" << endl
				<< "Q for Queen" << endl
				<< "B for Bishop" << endl
				<< "H for Knight" << endl
				<< "R for Rook" << endl;
			cin >> promoteTo;
			promoteTo = toUpper(promoteTo);
			if (promoteTo == 'Q' || promoteTo == 'B' || promoteTo == 'H' || promoteTo == 'R')
				break;
			else {
				cout << "Invalid input. Please select again." << endl;
			}
		}
		Board[ri][ci].Symbol = promoteTo;
	}

}

void GetisCheckEnemy(ChessPiece Board[][GRIDSIZE], int& eR, int& eC, int kR, int kC, int Turn) {
	Turn = (Turn + 1) % 2;
	for (eR = 0; eR < GRIDSIZE; eR++) {
		for (eC = 0; eC < GRIDSIZE; eC++) {
			if (Board[eR][eC].Side == Turn && isValidMove(Board, eR, eC, kR, kC, Turn))
				return;
		}
	}
	eR = -1;
	eC = -1;
}


bool isCheckMate(ChessPiece Board[][GRIDSIZE], int kR, int kC, int eR, int eC, int Turn, bool isCheck) {
	if (!isCheck)
		return false;
	if (eR == -1 || eC == -1)
		return false;
	if (kR == -1 || kC == -1)
		return false;
	for (int ri = kR - 1; ri <= kR + 1; ri++) {
		for (int ci = kC - 1; ci <= kC + 1; ci++) {
			if (!(ri == kR && ci == kC) && !isOutofBounds(ri, ci) && !isFriendlyOnLocation(Board, ri, ci, Turn)
				&& KingValidation(Board, kR, kC, ri, ci, Turn))
				return false;
		}
	}
	Turn = (Turn + 1) % 2;
	if (isPieceCheck(Board, eR, eC, Turn))
		return false;
	Turn = (Turn + 1) % 2;

	if (CanBlockPath(Board, eR, eC, kR, kC, Turn))
		return false;

	return true;
}

bool CanBlockPath(ChessPiece Board[][GRIDSIZE], int eR, int eC, int kR, int kC, int Turn) {
	if (eR == kR || eC == kC) {
		for (int i = eR + 1; i < kR; i++) {
			for (int ri = 0; ri < GRIDSIZE; ri++) {
				for (int ci = 0; ci < GRIDSIZE; ci++) {
					if (Board[ri][ci].Side == Turn && isValidMove(Board, ri, ci, i, eC, Turn) && 
						NonCheckMove(Board, kR, kC, ri, ci, i, eC, Turn))
						return true;
				}
			}
		}

		for (int i = eC + 1; i < kC; i++) {
			for (int ri = 0; ri < GRIDSIZE; ri++) {
				for (int ci = 0; ci < GRIDSIZE; ci++) {
					if (Board[ri][ci].Side == Turn && isValidMove(Board, ri, ci, eR, i, Turn) &&
						NonCheckMove(Board, kR, kC, ri, ci, eR, i, Turn))
						return true;
				}
			}
		}

		for (int i = kR + 1; i < eR; i++) {
			for (int ri = 0; ri < GRIDSIZE; ri++) {
				for (int ci = 0; ci < GRIDSIZE; ci++) {
					if (Board[ri][ci].Side == Turn && isValidMove(Board, ri, ci, i, kC, Turn) &&
						NonCheckMove(Board, kR, kC, ri, ci, i, kC, Turn))
						return true;
				}
			}
		}

		for (int i = kC + 1; i < eC; i++) {
			for (int ri = 0; ri < GRIDSIZE; ri++) {
				for (int ci = 0; ci < GRIDSIZE; ci++) {
					if (Board[ri][ci].Side == Turn && isValidMove(Board, ri, ci, kR, i, Turn) &&
						NonCheckMove(Board, kR, kC, ri, ci, kR, i, Turn))
						return true;
				}
			}
		}
	}
	else {
		if (kC < eC) {
			for (int i = 1; kR + i < eR; i++) {
				for (int ri = 0; ri < GRIDSIZE; ri++) {
					for (int ci = 0; ci < GRIDSIZE; ci++) {
						if (Board[ri][ci].Side == Turn && isValidMove(Board, ri, ci, kR + i, kC + i, Turn) &&
							NonCheckMove(Board, kR, kC, ri, ci, kR + i, kC + i, Turn))
							return true;
					}
				}
			}
			for (int i = 1; eR + i < kR; i++) {
				for (int ri = 0; ri < GRIDSIZE; ri++) {
					for (int ci = 0; ci < GRIDSIZE; ci++) {
						if (Board[ri][ci].Side == Turn && isValidMove(Board, ri, ci, eR + i, eC - i, Turn) &&
							NonCheckMove(Board, kR, kC, ri, ci, eR + i, eC - i, Turn))
							return true;
					}
				}
			}
		}
		else if (eC < kC) {
			for (int i = 1; kR + i < eR; i++) {
				for (int ri = 0; ri < GRIDSIZE; ri++) {
					for (int ci = 0; ci < GRIDSIZE; ci++) {
						if (Board[ri][ci].Side == Turn && isValidMove(Board, ri, ci, kR + i, kC - i, Turn) &&
							NonCheckMove(Board, kR, kC, ri, ci, kR + i, kC - i, Turn))
							return true;
					}
				}
			}
			for (int i = 1; eR + i < kR; i++) {
				for (int ri = 0; ri < GRIDSIZE; ri++) {
					for (int ci = 0; ci < GRIDSIZE; ci++) {
						if (Board[ri][ci].Side == Turn && isValidMove(Board, ri, ci, eR + i, eC + i, Turn) &&
							NonCheckMove(Board, kR, kC, ri, ci, eR + i, eC + i, Turn))
							return true;
					}
				}
			}
		}
	}

	return false;
	
}

void DisplayPath(ChessPiece Board[][GRIDSIZE], int pR, int pC, int Turn,
	int strtR, int strtC, int strtLN, int strtCN, int BlockR, int BlockC) {

	int cordR, cordC;
	cordR = strtR + strtLN;
	cordC = strtC + strtCN;

	for (int toR = 0; toR < GRIDSIZE; toR++) {
		for (int toC = 0; toC < GRIDSIZE; toC++) {
			if (isValidMove(Board, pR, pC, toR, toC, Turn) || CanCastle(Board, pR, pC, toR, toC)) {
				gotoRowCol(cordR + toR * BlockR, cordC + toC * BlockC);
				if (Board[toR][toC].Symbol != '-') {
					cout << "|";
					DisplayPiece(Board[toR][toC]);
					cout << "|";
				}
				else
					cout << " * ";
			}
		}
	}
	gotoRowCol(strtR + strtLN + GRIDSIZE * BlockR, 0);
}

char toUpper(char c) {
	if (c >= 'a' && c <= 'z')
		return c - 32;
	return c;
}

void SaveChange(ChangeStruct Changes[], ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC, int& ChangesSize) {
	Changes[ChangesSize].pR = pR;
	Changes[ChangesSize].pC = pC;
	Changes[ChangesSize].pPiece = Board[pR][pC].Symbol;
	Changes[ChangesSize].pSide = Board[pR][pC].Side;
	Changes[ChangesSize].toR = toR;
	Changes[ChangesSize].toC = toC;
	Changes[ChangesSize].toPiece = Board[toR][toC].Symbol;
	Changes[ChangesSize].toSide = Board[toR][toC].Side;

	ChangesSize++;

}

void WriteChanges(ChangeStruct Changes[], int ChangesSize, char GameFile[]) {
	ofstream fout(GameFile);

	fout << ChangesSize << endl;

	for (int i = 0; i < ChangesSize; i++) {
		fout << Changes[i].pR << " " << Changes[i].pC << " " << Changes[i].pPiece << " " << Changes[i].pSide << " ";
		fout << Changes[i].toR << " " << Changes[i].toC << " " << Changes[i].toPiece << " " << Changes[i].toSide << endl;
	}
}


void LoadChanges(ChangeStruct Changes[], int& ChangeSize, char GameFile[]) {
	ifstream fin(GameFile);
	fin >> ChangeSize;

	int pSide, toSide;

	for (int i = 0; i < ChangeSize; i++) {
		fin >> Changes[i].pR >> Changes[i].pC >> Changes[i].pPiece >> pSide >>
			Changes[i].toR >> Changes[i].toC >> Changes[i].toPiece >> toSide;

		Changes[i].pSide = (Color)pSide;
		Changes[i].toSide = (Color)toSide;

	}
}

int chartoInt(char c) {
	return c - 48;
}

void ApplyChange(ChessPiece Board[][GRIDSIZE], ChangeStruct Change, char Action) {

	if (Action == 13) {
		Board[Change.toR][Change.toC] = Board[Change.pR][Change.pC];
		Board[Change.pR][Change.pC].Symbol = '-';
		Board[Change.pR][Change.pC].Side = BLANK;

	}
	else if (Action == 32) {
		Board[Change.pR][Change.pC].Symbol = Change.pPiece;
		Board[Change.pR][Change.pC].Side = Change.pSide;

		Board[Change.toR][Change.toC].Symbol = Change.toPiece;
		Board[Change.toR][Change.toC].Side = Change.toSide;
	}

}

void NameToFile(char GameFile[]) {
	int i;
	for (i = 0; GameFile[i] != '\0'; i++);
	if (GameFile[i - 1] == 't' && GameFile[i - 2] == 'x' && GameFile[i - 3] == 't' && GameFile[i - 4] == '.')
		return;

	GameFile[i] = '.';
	i++;
	GameFile[i] = 't';
	i++;
	GameFile[i] = 'x';
	i++;
	GameFile[i] = 't';
	i++;
	GameFile[i] = '\0';
}

bool isLower(char c) {
	if (c >= 'a' && c <= 'z')
		return true;
	return false;
}

bool isUpper(char c) {
	if (c >= 'A' && c <= 'Z')
		return true;
	return false;
}

void ChangeMoveState(ChessPiece& Piece) {
	Piece.HasMoved = true;
}

bool CanCastle(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC) {
	int Turn = Board[pR][pC].Side;
	if (Board[pR][pC].Symbol != 'K')
		return false;
	if (pR != toR)
		return false;
	if (Board[pR][pC].HasMoved)
		return false;

	if (Absolute(toC - pC) != 2)
		return false;

	if (isKingCheck(Board, pR, pC, Turn))
		return false;

	if (pC > toC) {
		if (isKingCheck(Board, pR, toC + 1, Turn) || (Board[pR][toC + 1].Symbol != '-'))
			return false;
		if (isKingCheck(Board, pR, toC, Turn) || (Board[pR][toC].Symbol != '-'))
			return false;
		if ((Board[pR][toC + 2].Symbol != '-'))
			return false;

		return Board[pR][0].Symbol == 'R' && !Board[pR][0].HasMoved;
	}
	else if (toC > pC) {

		if (isKingCheck(Board, pR, toC - 1, Turn) || (Board[pR][toC - 1].Symbol != '-'))
			return false;
		if (isKingCheck(Board, pR, toC, Turn) || (Board[pR][toC].Symbol != '-'))
			return false;

		return Board[pR][GRIDSIZE - 1].Symbol == 'R' && !Board[pR][GRIDSIZE - 1].HasMoved;
	}

	return false;
}

void PerformCastling(ChessPiece Board[][GRIDSIZE], int pR, int pC, int toR, int toC) {
	UpdateBoard(Board, pR, pC, toR, toC);
	if (pC > toC) {
		UpdateBoard(Board, pR, 0, toR, toC + 1);
		ChangeMoveState(Board[pR][0]);
	}
	else {
		UpdateBoard(Board, pR, 7, toR, toC - 1);
		ChangeMoveState(Board[pR][7]);
	}
}

void DisplayButtons(int undoR, int undoC, int saeR, int saeC, int restartR, int restartC) {
	gotoRowCol(undoR, undoC);
	cout << "[UNDO]";

	gotoRowCol(saeR, saeC);
	cout << "[SAVE AND EXIT]";


	gotoRowCol(restartR, restartC);
	cout << "[RESTART]";

}

bool UndoClicked(int scrnR, int scrnC, int undoR, int undoC) {
	return ((scrnR == undoR) && (scrnC >= undoC && scrnC <= undoC + 6));
}

bool SaveAndExitClicked(int scrnR, int scrnC, int saeR, int saeC) {
	return ((scrnR == saeR) && (scrnC >= saeC && scrnC <= saeC + 15));
}

bool RestartClicked(int scrnR, int scrnC, int restartR, int restartC) {
	return ((scrnR == restartR) && (scrnC >= restartC && scrnC <= restartC + 9));
}

bool Undo(ChessPiece Board[][GRIDSIZE], ChangeStruct Changes[], int& ChangesSize) {
	if (ChangesSize == 0)
		return false;
	ChangesSize--;
	ApplyChange(Board, Changes[ChangesSize], 32);
	return true;
}

void SaveAndExit(ChangeStruct Changes[], int ChangesSize, char GameFile[]) {
	WriteChanges(Changes, ChangesSize, GameFile);
}

void Restart(int& ChangesSize) {
	ChangesSize = 0;
}