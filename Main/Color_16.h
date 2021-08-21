#pragma once
#include <stdint.h>
#include <cassert>

class Color_16
{
public:
	static uint16_t RGB888_TO_RGB565(uint8_t R, uint8_t G, uint8_t B);
	static uint16_t RGB888_TO_RGB565(uint32_t cDWORD);
	static uint16_t MakeRGB565(uint8_t R, uint8_t G, uint8_t B);
public:
	Color_16() = default;
	Color_16(uint16_t cWORD);
	Color_16(uint32_t cDWORD);
	Color_16(uint8_t R, uint8_t G, uint8_t B);
	void SetColor(uint16_t cWORD);
	uint16_t GetColor() const;
private:
	uint16_t cWORD;
};