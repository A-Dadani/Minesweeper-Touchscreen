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
	//There's probably a much better way but since this is going to run before the
	//program starts it won't impact the actual speed of the gameplay so it doesn't matter
	randomSeed(analogRead(2));
	for (unsigned short int i = 0; i < NUMBER_CELLS_H; i++)
	{
		for (unsigned short int j = 0; j < NUMBER_CELLS_V; j++)
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
	for (unsigned short int i = 0; i < NUMBER_CELLS_H; i++)
	{
		for (unsigned short int j = 0; j < NUMBER_CELLS_V; j++)
		{
			if (cells[i][j].GetContent() == Cell::Content::Bomb) continue;
			
			unsigned char nNeighboringBombs = 0;
			for (short int c = -1; c <= 1; c++)
			{
				for (short int l = -1; l <= 1; l++)
				{
					if ((c + i) < 0 || (l + j) < 0 || (c + i) >= NUMBER_CELLS_H || (l + j) >= NUMBER_CELLS_V) continue;
					else
					{
						if (cells[i + c][j + l].GetContent() == Cell::Content::Bomb)
						{
							++nNeighboringBombs;
						}
					}
				}
			}
			cells[i][j].SetnNeighboringBombs(nNeighboringBombs);
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
	DrawBorders();
	for (unsigned short int i = 0; i < NUMBER_CELLS_H; i++)
	{
		for (unsigned short int j = 0; j < NUMBER_CELLS_V; j++)
		{
			cells[i][j].Draw(scrn, BorderThiccness);
		}
	}
}

void Board::Cell::DrawBorders(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const
{
	scrn.drawFastHLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness, CELL_DIMENTIONS, lightBorderColor.GetWORD());
	scrn.drawFastHLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + CELL_DIMENTIONS - 1, CELL_DIMENTIONS, darkBorderColor.GetWORD());
	scrn.drawFastVLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness, CELL_DIMENTIONS, lightBorderColor.GetWORD());
	scrn.drawFastVLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + CELL_DIMENTIONS - 1, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness, CELL_DIMENTIONS, darkBorderColor.GetWORD());
}

void Board::Cell::Draw(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const
{
	scrn.fillRect(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness, CELL_DIMENTIONS, CELL_DIMENTIONS, cellFillingColor.GetWORD());
	DrawBorders(scrn, boardBorderThiccness);
}