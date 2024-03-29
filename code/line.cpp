
#include "headers/Line.h"
#include <vector>
#include <cmath>
//-------LINE GENERATOR-------------//
void generateLines(std::vector<Line>& lines)
{

    for (int i = 0; i < 2500; i++)
    {
        Line line;
        line.z = i * segL;

        if (i > 300 && i < 700)
            line.curve = 0.8; //for right turn in road

        if (i > 1100 && i < 1400)
            line.curve = -0.7; //for left turn in road

        lines.push_back(line); //each line is added to VECTOR CONTAINER

    }
}

//spriteX is X distance multiplier
void generateObstacles(Sprite ob[], std::vector<Line>& lines)
{
    for (int i = 0; i < 2500; i++)
    {
        if (i == 305)
        {
            lines[i].spriteX = 3; lines[i].sprite = ob[6];
        } //warning sign
        if (i == 1175)
        {
            lines[i].spriteX = -4; lines[i].sprite = ob[6];
        } //warning sign
        if (i == 797)
        {
            lines[i].spriteX = 3; lines[i].sprite = ob[15];
        } //warning2 sign
        if (i == 1657)
        {
            lines[i].spriteX = -4; lines[i].sprite = ob[15];
        } //warning2 sign

        if (i > 0 && i < 451 && i % 15 == 0)
        {
            lines[i].spriteX = 2; lines[i].sprite = ob[3];
        } //tree 1
        if (i > 55 && i < 1000 && i % 19 == 0)
        {
            lines[i].spriteX = -2.9; lines[i].sprite = ob[1];
        } //tree 1

        if (i > 1800 && (i - 99) % 91 == 0)
        {
            lines[i].spriteX = -2; lines[i].sprite = ob[7];
        } //tree
        if (i > 1600 && i % 151 == 0)
        {
            lines[i].spriteX = 3; lines[i].sprite = ob[7];
        } //tree

        if (i == 275)
        {
            lines[i].spriteX = -2.5; lines[i].sprite = ob[16];
        } //curve ahead
        if (i == 1075)
        {
            lines[i].spriteX = 3.5; lines[i].sprite = ob[17];
        } //curve ahead

        if (i == 1500)
        {
            lines[i].spriteX = 2; lines[i].sprite = ob[5];
        } //traffic light
        if (i == 2495)
        {
            lines[i].spriteX = -0.2; lines[i].sprite = ob[5];
        } //traffic light

        if (i == 1101 || i == 2200)
        {
            lines[i].spriteX = -0.3; lines[i].sprite = ob[4];
        } //red car
        if (i == 1600)
        {
            lines[i].spriteX = -1.1; lines[i].sprite = ob[13];
        } //white car

        //if (i > 1300 && i < 2200 && i % 101 == 0)
        //{
        //    lines[i].spriteX = -2.7; lines[i].sprite = ob[3];
        //} //poles
        // if (i > 0 && i < 851 && i % 29 == 0)
        // {
        //     lines[i].spriteX = 0.8; lines[i].sprite = ob[3];
        // } //poles
//--------------ROAD DIVIEDER-------------------------------------------//
        if (i < 200 && (i % 61) == 0)
        {
            lines[i].spriteX = -0.1; lines[i].sprite = ob[10];
        }
        if (i > 200 && (i % 101) == 0)
        {
            lines[i].spriteX = -0.1; lines[i].sprite = ob[10];
        }

        if ((i > 700 && i < 1000) && (i % 51) == 0)
        {
            lines[i].spriteX = 3; lines[i].sprite = ob[10];
        }
        if ((i < 700 || i>1000) && (i % 201) == 0)
        {
            lines[i].spriteX = 1.9; lines[i].sprite = ob[10];
        }

        if (i > 1500 && i < 2000 && (i % 221) == 0)
        {
            lines[i].spriteX = -2.5; lines[i].sprite = ob[10];
        }
        if ((i < 1500 || i>2000) && (i % 51) == 0)
        {
            lines[i].spriteX = -2.5; lines[i].sprite = ob[10];
        }
        //-------------------STONE---------------------------------------//
        if (i < 800 && (i % 311) == 0)
        {
            lines[i].spriteX = -0.0; lines[i].sprite = ob[11];
        }
        if (i > 800 && (i % 111) == 0)
        {
            lines[i].spriteX = -0.0; lines[i].sprite = ob[11];
        }

        if (i > 501 && (i + 21) % 111 == 0)
        {
            lines[i].spriteX = 0.8; lines[i].sprite = ob[11];
        }

        if ((i > 200 && i < 800) && (i % 81) == 0)
        {
            lines[i].spriteX = -1.2; lines[i].sprite = ob[11];
        }
        if (i > 800 && (i % 91) == 0)
        {
            lines[i].spriteX = -1.2; lines[i].sprite = ob[11];
        }
        //-------------------CAR FRONT---------------------------------------//
        if (i == 551 || i == 1551 || i == 1951)
        {
            lines[i].spriteX = 0.5; lines[i].sprite = ob[8];
        }
        if (i == 301 || i == 701 || i == 1351 || i == 2291)
        {
            lines[i].spriteX = 0.3; lines[i].sprite = ob[8];
        }
        if (i == 1151 || i == 1751 || i == 2491)
        {
            lines[i].spriteX = -1.2; lines[i].sprite = ob[8];
        }
        //-------------------CAR BACK---------------------------------------//
        if (i == 101 || i == 1651 || i == 2151)
        {
            lines[i].spriteX = -0.0; lines[i].sprite = ob[9];
        }
        if (i == 1251 || i == 1851)
        {
            lines[i].spriteX = 0.3; lines[i].sprite = ob[9];
        }
        if (i == 201 || i == 801 || i == 1453 || i == 2391)
        {
            lines[i].spriteX = -1.0; lines[i].sprite = ob[9];
        }
        //----------------------------------------------------------//
        if (i == 1101 || i == 2102)
        {
            lines[i].spriteX = -0.5; lines[i].sprite = ob[12];
        }

        if (lines[i].sprite.position_in_screen.w < 0)
        {
            lines[i].sprite.position_in_screen.w = 0;
        }
		if (lines[i].sprite.position_in_screen.w > width)
		{
			lines[i].sprite.position_in_screen.w = 0;
		}
        if (lines[i].sprite.position_in_screen.h < 0)
        {
            lines[i].sprite.position_in_screen.h = 0;

        }
		
		if (lines[i].sprite.position_in_screen.h > height)
		{
			lines[i].sprite.position_in_screen.h = 0;
		}
		//if width or height are NaN width and height =0
        if (isnan(lines[i].sprite.position_in_screen.w)||isnan(lines[i].sprite.position_in_screen.h))
        {
            lines[i].sprite.position_in_screen.w = lines[i].sprite.position_in_screen.h = 0;
        }
    }
}

void generateCoins(Sprite& coinn, std::vector<Line>& lines)
{
    for (int i = 0; i < 2500; i++)
    {
        if (i > 150 && (i + 21) % 59 == 0)
        {
            lines[i].coinX = 0.5; lines[i].coin = coinn;
        }
        else if (i > 150 && i % 59 == 0)
        {
            lines[i].coinX = -2.5; lines[i].coin = coinn;
        }
        else if (i > 101 && (i - 21) % 49 == 0)
        {
            lines[i].coinX = 2.2; lines[i].coin = coinn;
        }


    }
}

