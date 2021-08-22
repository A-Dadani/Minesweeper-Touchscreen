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
{  }

void Board::DrawBorders()
{
	for (int i = 0; i < BorderThiccness; i++)
	{
		scrn.drawRect(i, i, ScreenWidth - (i * 2), ScreenHeight - (i * 2), borderColor.GetWORD());
	}
}

void Board::Setup(Vec2<unsigned short int>* bombCoords)
{
	//There's probably a much better way but since this is going to run before the
	//program starts it won't impact the actual speed of the gameplay so it doesn't matter

	//Giving all the cells their own positions on the grid
	for (unsigned short int i = 0; i < NUMBER_CELLS_H; i++)
	{
		for (unsigned short int j = 0; j < NUMBER_CELLS_V; j++)
		{
			cells[i][j].SetPos(Vec2<unsigned short int> {i, j});
		}
	}

	//Distributing the bombs
	for (int i = 0; i < nBombs; i++)
	{
		cells[bombCoords[i].GetX()][bombCoords[i].GetY()].SetContent(Cell::Content::Bomb);
	}

	//Giving the Number of Neighboring Bombs to each cell
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
	
	scrn.drawFastHLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + 1, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + 1, CELL_DIMENTIONS - 2, lightBorderColor.GetWORD());
	scrn.drawFastHLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + 1, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + CELL_DIMENTIONS - 2, CELL_DIMENTIONS - 2, darkBorderColor.GetWORD());
	scrn.drawFastVLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + 1, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + 1, CELL_DIMENTIONS - 2, lightBorderColor.GetWORD());
	scrn.drawFastVLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + CELL_DIMENTIONS - 2, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + 1, CELL_DIMENTIONS - 2, darkBorderColor.GetWORD());
}

void Board::Cell::Draw(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const
{
	scrn.fillRect(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness, CELL_DIMENTIONS, CELL_DIMENTIONS, cellFillingColor.GetWORD());
	DrawBorders(scrn, boardBorderThiccness);
}

void Board::Cell::DrawNumber(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const
{
	scrn.drawChar(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness, nNeighboringBombs + 48, )
}

void Board::Cell::DrawBomb(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const
{
}