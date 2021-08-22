#include "Board.h"

Board::Board(Adafruit_ILI9341& scrn, unsigned short int ScreenWidth,
	unsigned short int ScreenHeight, unsigned short int BorderThiccness,
	unsigned short int nBombs, Color_16 borderColor)
	:
	scrn(scrn),
	borderColor(borderColor),
	ScreenWidth(ScreenWidth),
	ScreenHeight(ScreenHeight),
	BorderThiccness(BorderThiccness),
	nBombs(nBombs)
{
	randomSeed(analogRead(2));
	for (unsigned short int i = 0; i < NUMBER_CELLS_V; i++)
	{
		for (unsigned short int j = 0; j < NUMBER_CELLS_H; j++)
		{
			cells[i][j].SetPos(Vec2<unsigned short int> {i, j});
		}
	}
	for (int i = 0; i < nBombs; i++)
	{
		unsigned short int randomCellX = (unsigned short int)random(0, NUMBER_CELLS_H);
		unsigned short int randomCellY = (unsigned short int)random(0, NUMBER_CELLS_V);
		if (cells[randomCellX][randomCellY].GetContent() == Cell::Content::Bomb) --i;
		else
		{
			cells[randomCellX][randomCellY].SetContent(Cell::Content::Bomb);
		}
	}
}

void Board::DrawBorders()
{
	for (int i = 0; i < BorderThiccness; i++)
	{
		scrn.drawRect(i, i, ScreenWidth - (i * 2), ScreenHeight - (i * 2), borderColor.GetWORD());
	}
}

void Board::Setup()
{
}