//Using arduino mega

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <URTouch.h>

//Defining the display pins
#define TFT_CS 53
#define TFT_DC 9
#define TFT_MOSI 51
#define TFT_CLK 52
#define TFT_RST 22
#define TFT_MISO 50

//Defining the touch interface pin
#define t_SCK 26
#define t_CS 27
#define t_MOSI 28
#define t_MISO 29
#define t_IRQ 30

Adafruit_ILI9341 scrn = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
URTouch touchIF(t_SCK, t_CS, t_MOSI, t_MISO, t_IRQ);

void setup()
{
	//Initializing the screen
	scrn.begin();
	scrn.setRotation(2);
	scrn.fillScreen(ILI9341_BLACK);

	//Initilizing the touch interface
	touchIF.InitTouch(PORTRAIT);
	touchIF.setPrecision(PREC_EXTREME);
}

void loop()
{
}