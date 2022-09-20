#pragma once
#include <Windows.h>
#include "Tools.h"

/*
アマツツミ PKG
#define CMVS_326_
#define PS3ADDR 0x0045A7B0
#define PS3XOR 0x0045A896
#define PS3Com 0x0045A8C1
#define PB3ADDR 0x00430685

//クナド国記　体験版v2
#define CMVS_380_
#define PS3ADDR 0x0046CE10
#define PS3XOR 0x0046CEF1
#define PS3Com 0x0046CF1D
#define PB3ADDR 0x0043FE8D

//青春フラジャイル 体験版v2
#defind CMVS_380_
#define PS3ADDR 0x0046CB80
#define PS3XOR 0x0046CC61
#define PS3Com 0x0046CC8D
#define PB3ADDR 0x0043FF3D
*/

//青春フラジャイル 体験版v2
#define CMVS_380_
#define PS3ADDR 0x0046CB80
#define PS3XOR 0x0046CC61
#define PS3Com 0x0046CC8D
#define PB3ADDR 0x0043FF3D


VOID __declspec(naked)CMVS_326_PS3Hook();
VOID __declspec(naked)CMVS_326_PB3Hook();
VOID __declspec(naked)CMVS_380_PS3Hook();
VOID __declspec(naked)CMVS_380__PB3Hook();