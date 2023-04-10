#include "..\MGVEditor\MGVEditor.h"

void UnpackMGV()
{
	MGVFile::MGVUnpack mgv(L"op.mgv");
	mgv.Unpack();
}

void PackMGV()
{
	MGVFile::MGVPack mgv(L"1.mgv");
	MGVHeader header = { 0 };
	header.aSignature[0] = 'M';
	header.aSignature[1] = 'G';
	header.aSignature[2] = 'V';
	header.aSignature[3] = '1';

	header.uiVideoSize = 0x59F4762;
	header.uiAudioSize = 0x192566;
	header.uiBitCount = 32;
	header.usWidth = 1280;
	header.usHight = 720;
	header.uiFrameIndexCount = 0x0DF0; // FrameRate * Time. If Value Too Large Game Will Get Stuck After Playback
	header.uiFrameRateTime = 1000; //1s
	header.uiFrameRate = 30 * 1000; //30 * 1000

	mgv.InitMGVInfo(&header);
	mgv.MakePack(L"op.mgv.ogg", L"op.mgv.ogv");
}

int main()
{
	UnpackMGV();
	//PackMGV();
}