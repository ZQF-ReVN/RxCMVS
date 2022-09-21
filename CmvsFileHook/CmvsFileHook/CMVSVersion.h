#pragma once
#include <Windows.h>
#include "Tools.h"

//初恋サクラメント PKG
#define CMVS_210_
#define PS2ADDR 0x0044A1D0
#define PS2XOR 0x0044A295
#define PS2Com 0x0044A2BA
#define PB3ADDR_Ver210 0x00425F7F

VOID CMVS_210_PS2Hook();
VOID CMVS_210_PB3Hook();
VOID CMVS_342_PS3Hook();
VOID CMVS_342_PB3Hook();
VOID CMVS_380_PS3Hook();
VOID CMVS_380_PB3Hook();

/*
CMVS_380_ 3.80-3.76
CMVS_342_ 3.42-3.20
CMVS_210_ 2.10-1.94
*/

/*
//クナド国記
#define CMVS_380_
#define PS3ADDR 0x0046CE10
#define PS3XOR 0x0046CEF1
#define PS3Com 0x0046CF1D
#define PB3ADDR 0x0043FE8D

//青春フラジャイル
#defind CMVS_380_
#define PS3ADDR 0x0046CB80
#define PS3XOR 0x0046CC61
#define PS3Com 0x0046CC8D
#define PB3ADDR 0x0043FF3D

//リアライブ PKG
#define CMVS_380_
#define PS3ADDR 0x0046CB80
#define PS3XOR 0x0046CC61
#define PS3Com 0x0046CC8D
#define PB3ADDR 0x0043FF7D

//リアライブ PKG
#define CMVS_342_
#define PS3ADDR 0x0045FEE0
#define PS3XOR 0x0045FFC6
#define PS3Com 0x0045FFF1
#define PB3ADDR 0x00434485

アマツツミ PKG
#define CMVS_342_
#define PS3ADDR 0x0045A7B0
#define PS3XOR 0x0045A896
#define PS3Com 0x0045A8C1
#define PB3ADDR 0x00430685

//ハピメアFD ハピメアＷパック PKG
#define CMVS_342_
#define PS3ADDR 0x00457BC0
#define PS3XOR 0x00457CA6
#define PS3Com 0x00457CD1
#define PB3ADDR 0x0042EA55

//クロノクロック PKG
#define CMVS_342_
#define PS3ADDR 0x00457AB0
#define PS3XOR 0x00457B96
#define PS3Com 0x00457BC1
#define PB3ADDR 0x0042F455

//ハピメア PKG
#define CMVS_342_
#define PS3ADDR 0x00452200
#define PS3XOR 0x004522E6
#define PS3Com 0x00452311
#define PB3ADDR 0x00429FC5

//しあわせ家族部 PKG
#define CMVS_210_
#define PS2ADDR 0x0044C100
#define PS2XOR 0x0044C1C5
#define PS2Com 0x0044C1EA
#define PB3ADDR_Ver210 0x004268FF

//未来ノスタルジア PKG
#define CMVS_210_
#define PS2ADDR 0x0044A820
#define PS2XOR 0x0044A8E5
#define PS2Com 0x0044A90A
#define PB3ADDR_Ver210 0x00425EDF
*/