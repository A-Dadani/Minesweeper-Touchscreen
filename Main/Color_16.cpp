#include "Color_16.h"

Color_16::Color_16(uint16_t cWORD)
	:
	cWORD(cWORD)
{  }

Color_16::Color_16(uint32_t cDWORD)
{
	//Could get all of this in one line but it would be unreadable
	//So you have to deal with this ¯\_(._.)_/¯
	uint8_t R, G, B;
	B = cDWORD & 0xFF;
	cDWORD >>= 8;
	G = cDWORD & 0xFF;
	cDWORD >>= 8;
	R = (uint8_t)cDWORD;
	R >>= 3;
	G >>= 2;
	B >>= 3;
	cWORD = (R << 11) + (G << 5) + B;
}

Color_16::Color_16(uint8_t R, uint8_t G, uint8_t B)
{
	R >>= 3;
	G >>= 2;
	B >>= 3;
	cWORD = (R << 11) + (G << 5) + B;
}

uint16_t Color_16::RGB888_TO_RGB565(uint32_t cDWORD)
{
	uint8_t R, G, B;
	B = cDWORD & 0xFF;
	cDWORD >>= 8;
	G = cDWORD & 0xFF;
	cDWORD >>= 8;
	R = (uint8_t)cDWORD;
	return RGB888_TO_RGB565(R, G, B);
}

uint16_t Color_16::RGB888_TO_RGB565(uint8_t R, uint8_t G, uint8_t B)
{
	R >>= 3;
	G >>= 2;
	B >>= 3;
	return (R << 11) + (G << 5) + B;
}

uint16_t Color_16::MakeRGB565(uint8_t R, uint8_t G, uint8_t B)
{
	return uint16_t((R << 11) + (G << 5) + B);
}

void Color_16::SetColor(uint16_t cWORD)
{
	this->cWORD = cWORD;
}

uint16_t Color_16::GetColor() const
{
	return cWORD;
}
