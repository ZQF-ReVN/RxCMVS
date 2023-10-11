#pragma once
#include <cstdint>


namespace CMVS::FileHook
{
	void SetHookFolder(const char* cpFolder);

	void SetPS3Hook_210_(uint32_t fnLoadScript, uint32_t asDecScript_0, uint32_t asDecScript_1);
	void SetPB3Hook_210_(uint32_t fnLoadImage);

	void SetPS3Hook_342_(uint32_t fnLoadScript, uint32_t asDecScript_0, uint32_t asDecScript_1);
	void SetPB3Hook_342_(uint32_t fnLoadImage);

	void SetPS3Hook_380_(uint32_t fnLoadScript, uint32_t asDecScript_0, uint32_t asDecScript_1);
	void SetPB3Hook_380_(uint32_t fnLoadImage);
}


/*
CMVS_380_ 3.80-3.76
CMVS_342_ 3.42-3.20
CMVS_210_ 2.10-1.10
*/

/*
//クナド国記 PKG
#define CMVS_380_
#define SCRIPTADDR 0x0046CE10
#define SCRIPTXOR 0x0046CEF1
#define SCRIPTCOM 0x0046CF1D
#define IMAGEADDR 0x0043FE8D

//青春フラジャイル PKG
#define CMVS_380_
#define SCRIPTADDR 0x0046CB80
#define SCRIPTXOR 0x0046CC61
#define SCRIPTCOM 0x0046CC8D
#define IMAGEADDR 0x0043FF3D

//リアライブ PKG
#define CMVS_380_
#define SCRIPTADDR 0x0046CB80
#define SCRIPTXOR 0x0046CC61
#define SCRIPTCOM 0x0046CC8D
#define IMAGEADDR 0x0043FF7D

//アオイトリ PKG
#define CMVS_342_
#define SCRIPTADDR 0x0045FEE0
#define SCRIPTXOR 0x0045FFC6
#define SCRIPTCOM 0x0045FFF1
#define IMAGEADDR 0x00434485

//アマツツミ PKG
#define CMVS_342_
#define SCRIPTADDR 0x0045A7B0
#define SCRIPTXOR 0x0045A896
#define SCRIPTCOM 0x0045A8C1
#define IMAGEADDR 0x00430685

//ハピメアFD ハピメアＷパック PKG
#define CMVS_342_
#define SCRIPTADDR 0x00457BC0
#define SCRIPTXOR 0x00457CA6
#define SCRIPTCOM 0x00457CD1
#define IMAGEADDR 0x0042EA55

//クロノクロック PKG
#define CMVS_342_
#define SCRIPTADDR 0x00457AB0
#define SCRIPTXOR 0x00457B96
#define SCRIPTCOM 0x00457BC1
#define IMAGEADDR 0x0042F455

//ハピメア PKG
#define CMVS_342_
#define SCRIPTADDR 0x00452200
#define SCRIPTXOR 0x004522E6
#define SCRIPTCOM 0x00452311
#define IMAGEADDR 0x00429FC5

//しあわせ家族部 PKG
#define CMVS_210_
#define SCRIPTADDR 0x0044C100
#define SCRIPTXOR 0x0044C1C5
#define SCRIPTCOM 0x0044C1EA
#define IMAGEADDR 0x004268FF

//未来ノスタルジア PKG
#define CMVS_210_
#define SCRIPTADDR 0x0044A820
#define SCRIPTXOR 0x0044A8E5
#define SCRIPTCOM 0x0044A90A
#define IMAGEADDR 0x00425EDF

//初恋サクラメント PKG
#define CMVS_210_
#define SCRIPTADDR 0x0044A1D0
#define SCRIPTXOR 0x0044A295
#define SCRIPTCOM 0x0044A2BA
#define IMAGEADDR 0x00425F7F

//夏に奏でる僕らの詩 PKG
#define CMVS_210_
#define SCRIPTADDR 0x00443FB0
#define SCRIPTXOR 0x00444075
#define SCRIPTCOM 0x0044409A
#define IMAGEADDR 0x0042116F

//明日の君と逢うために PKG
#define CMVS_210_
#define SCRIPTADDR 0x00441460
#define SCRIPTXOR 0x00441525
#define SCRIPTCOM 0x0044154A
#define IMAGEADDR 0x0041FA5F

//メモリア PKG
#define CMVS_210_
#define SCRIPTADDR 0x00441460
#define SCRIPTXOR 0x00441525
#define SCRIPTCOM 0x0044154A
#define IMAGEADDR 0x0041FA5F

//明日の七海と逢うためにFD PKG
#define CMVS_210_
#define SCRIPTADDR 0x0043DAC0
#define SCRIPTXOR 0x0043DB85
#define SCRIPTCOM 0x0043DBAA
#define IMAGEADDR 0x0041E96F

//明日の七海と逢うために PKG
#define CMVS_210_
#define SCRIPTADDR 0x00439C10
#define SCRIPTXOR 0x00439CD8
#define SCRIPTCOM 0x00439CFE
#define IMAGEADDR 0x0041E12B

//プリミティブ リンク PKG
#define CMVS_210_
#define SCRIPTADDR 0x0042C7E0
#define SCRIPTXOR 0x0042C8A8
#define SCRIPTCOM 0x0042C8CE
#define IMAGEADDR 0x0041477B

*/