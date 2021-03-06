// test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "..\..\cpu\CPU.h"
#include "..\..\disk\diskdefination.h"
#include "..\..\gpu\gpu.h"
#include "..\..\acpi\ACPI.h"
#include "..\..\Audio\CAudio.h"
#include "..\..\network\NetworkLib.h"
#include "..\..\edid\EDID.h"
#include "..\..\smbios\smbioslib.h"

int main()
{
	SV_ASSIST::ACPI::Excute();
	auto b = SV_ASSIST::ACPI::GetMCFG();

	SV_ASSIST::GPU::UpdateDate();
	auto a = SV_ASSIST::GPU::GetGpuInfo();

	auto ac = SV_ASSIST::CPU::GetCPUName();

	SV_ASSIST::Storage::updatediskinfo();

	SV_ASSIST::AUDIO::Exec();
	auto aa = SV_ASSIST::AUDIO::GetOutputAudio();

	SV_ASSIST::Display::Exec();
	auto ed = SV_ASSIST::Display::GetEDID(0);

	SV_ASSIST::DMI_SMBIOS::Updatesmbios();

	SV_ASSIST::Net::Exec();
	auto net = SV_ASSIST::Net::GetData();
    return 0;
}

