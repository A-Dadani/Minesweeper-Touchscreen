#include "Board.h"

Board::Board(Adafruit_ILI9341& scrn, unsigned short int ScreenWidth,
	unsigned short int ScreenHeight, unsigned short int BorderThiccness,
	unsigned short int nBombs, GameStatus& gameStatus, Color_16 borderColor)
	:
	scrn(scrn),
	borderColor(borderColor),
	ScreenWidth(ScreenWidth),
	ScreenHeight(ScreenHeight),
	BorderThiccness(BorderThiccness),
	nBombs(nBombs),
	gameStatus(gameStatus)
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
					if ((c + i) < 0 || (l + j) < 0 || (c + i) >= NUMBER_CELLS_H || (l + j) >= NUMBER_CELLS_V) continue; // Could be better optimized by moving the c conditions to the parent loop
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

void Board::TouchInput(const Vec2<uint16_t>& touchCoords)
{
	if (touchCoords.GetX() >= BorderThiccness && touchCoords.GetY() >= BorderThiccness
		&& touchCoords.GetX() <= (ScreenWidth - BorderThiccness) && touchCoords.GetY() <= (ScreenHeight - BorderThiccness))
	{
		Vec2<uint16_t> GridCoords = RemapTouchToGridCoords(touchCoords, BorderThiccness);
		RevealCell(GridCoords);
		if (gameStatus == GameStatus::Lost) return;
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

void Board::RevealCell(const Vec2<uint16_t>& cellCoords)
{
	cells[cellCoords.GetX()][cellCoords.GetY()].Reveal(scrn, BorderThiccness);
	if (cells[cellCoords.GetX()][cellCoords.GetY()].GetContent() == Cell::Content::Bomb)
	{
		gameStatus = GameStatus::Lost;
		RevealAllBombs();
		delay(1000);
		return;
	}
	if (cells[cellCoords.GetX()][cellCoords.GetY()].GetnNeighboringBombs() == 0)
	{
		for (short int c = -1; c <= 1; c++)
		{
			for (short int l = -1; l <= 1; l++)
			{
				if ((c + cellCoords.GetX()) < 0 || (l + cellCoords.GetY()) < 0 || (c + cellCoords.GetX()) >= NUMBER_CELLS_H || (l + cellCoords.GetY()) >= NUMBER_CELLS_V) continue;
				else if (cells[cellCoords.GetX() + c][cellCoords.GetY() + l].GetStatus() == Cell::Status::Revealed) continue;
				else
				{
					RevealCell(Vec2<uint16_t>{cellCoords.GetX() + c, cellCoords.GetY() + l});
				}
			}
		}
	}
}

void Board::Cell::Reveal(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness)
{
	cellStatus = Status::Revealed;
	if (cellContent == Content::Bomb)
	{
		DrawRevealedCellBackground(scrn, boardBorderThiccness, bombCellBackground);
		DrawBomb(scrn, boardBorderThiccness);
	}
	else
	{
		DrawRevealedCellBackground(scrn, boardBorderThiccness);
		if (nNeighboringBombs != 0)
		{
			DrawNumber(scrn, boardBorderThiccness);
		}
	}
	DrawRevealedCellBorder(scrn, boardBorderThiccness);
}

void Board::RevealAllBombs()
{
	for (auto& cL : cells)
	{
		for (auto& c : cL)
		{
			if (c.GetContent() == Cell::Content::Bomb) c.Reveal(scrn, BorderThiccness);
		}
	}
}

void Board::Cell::DrawNumber(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const
{
	scrn.drawChar(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_COUNT_CENTERING_OFFSET_X, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_COUNT_CENTERING_OFFSET_Y, nNeighboringBombs + 48, numberColors[nNeighboringBombs - 1].GetWORD(), revealedCellFillingColor.GetWORD(), NUMBER_NEIGHBORING_BOMBS_SIZE);
}

void Board::Cell::DrawBomb(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const
{
	scrn.fillCircle(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET, BOMB_RADIUS, 0x0000);
	scrn.drawFastHLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET - BOMB_RADIUS - BOMB_NIPPLE_LENGTH, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET, BOMB_NIPPLE_LENGTH, 0x0000);
	scrn.drawFastHLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET + BOMB_RADIUS + BOMB_NIPPLE_LENGTH, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET, BOMB_NIPPLE_LENGTH, 0x0000);
	scrn.drawFastVLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET - BOMB_RADIUS - BOMB_NIPPLE_LENGTH, BOMB_NIPPLE_LENGTH, 0x0000);
	scrn.drawFastVLine(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET + BOMB_RADIUS + BOMB_NIPPLE_LENGTH, BOMB_NIPPLE_LENGTH, 0x0000);
	scrn.drawRect(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET - 2, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness + BOMB_SELF_CENTERING_OFFSET - 2, 2, 2, 0xFFFF);
}

void Board::Cell::DrawRevealedCellBackground(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness, Color_16 c) const
{
	scrn.fillRect(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness, CELL_DIMENTIONS, CELL_DIMENTIONS, c.GetWORD());
	scrn.drawRect(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness, CELL_DIMENTIONS, CELL_DIMENTIONS, revealedCellBorderColor.GetWORD());
}

void Board::Cell::DrawRevealedCellBackground(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const
{
	DrawRevealedCellBackground(scrn, boardBorderThiccness, revealedCellFillingColor);
}

void Board::Cell::DrawRevealedCellBorder(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const
{
	scrn.drawRect(myPos.GetX() * CELL_DIMENTIONS + boardBorderThiccness, myPos.GetY() * CELL_DIMENTIONS + boardBorderThiccness, CELL_DIMENTIONS, CELL_DIMENTIONS, revealedCellBorderColor.GetWORD());
}
