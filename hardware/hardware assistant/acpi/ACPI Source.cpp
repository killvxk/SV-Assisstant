#include "stdafx.h"
#include "ACPI Source.h"

template<class T>
void CACPI::SafeDeleteArray(T*& Array)
{
	if (Array)
	{
		delete[] Array;
		Array = NULL;
	}
}

template<class T>
void CACPI::SafeDeletePoint(T*& Point)
{
	if (Point)
	{
		delete Point;
		Point = NULL;
	}
}

CACPI::CACPI()
{
	::ZeroMemory(&facp, sizeof(facp));
	::ZeroMemory(&hpet, sizeof(hpet));
	::ZeroMemory(&mcfg, sizeof(mcfg));
	fpdt = NULL;
	::ZeroMemory(&bgrt, sizeof(bgrt));
	for (size_t i = 0; i < 30; i++)
	{
		FirewareTable[i].FireWareTable = NULL;
		FirewareTable[i].FireWareTableSize = 0;
	}
	isSupportACPI = this->Get_FirmWare_Enum();
}

CACPI::~CACPI()
{
	for (size_t i = 0; i < 30; i++)
	{
		SafeDeleteArray(FirewareTable[i].FireWareTable);
	}
}

void CACPI::Get()
{
	if (isSupportACPI)
	{
		for (size_t i = 0; i < FirmwareTableID.size(); ++i)
		{
			this->Get_ACPI_Table(FirmwareTableID[i], i);
			this->Excute(FirmwareTableID[i]);
		}
	}
}

void CACPI::Excute(const DWORD Table_Type)
{
	DWORD location = 0;
	switch (Table_Type)
	{
	case RSDT:
		for (DWORD i = 0; i < FirmwareTableID.size(); ++i)
		{
			if (Table_Type == FirmwareTableID[i])
			{
				location = i;
				break;
			}
			location = -1;
		}
		ACPI_RSDT_STRUCTURE rsdt;
		if (location == 0xFFFFFFFF)		break;
		memcpy_s(&rsdt, sizeof(rsdt),FirewareTable[location].FireWareTable, FirewareTable[location].FireWareTableSize);
		break;
	case FACP:
		for (DWORD i = 0; i < FirmwareTableID.size(); ++i)
		{
			if (Table_Type == FirmwareTableID[i])
			{
				location = i;
				break;
			}
			location = -1;
		}
		if (location == 0xFFFFFFFF)		break;
		memcpy_s(&facp, sizeof(facp), FirewareTable[location].FireWareTable, FirewareTable[location].FireWareTableSize);
		break;
	case HPET:
		for (DWORD i = 0; i < FirmwareTableID.size(); ++i)
		{
			if (Table_Type == FirmwareTableID[i])
			{
				location = i;
				break;
			}
			location = -1;
		}
		if (location == 0xFFFFFFFF)		break;
		memcpy_s(&hpet, sizeof(hpet), FirewareTable[location].FireWareTable, FirewareTable[location].FireWareTableSize);
		break;
	case FPDT:
		for (DWORD i = 0; i < FirmwareTableID.size(); ++i)
		{
			if (Table_Type == FirmwareTableID[i])
			{
				location = i;
				break;
			}
			location = -1;
		}
		if (location == 0xFFFFFFFF)		break;
		fpdt = (PACPI_FPDT_STRUCTURE)FirewareTable[location].FireWareTable;
		//memcpy_s(fpdt, sizeof(*fpdt), FirewareTable[location].FireWareTable, FirewareTable[location].FireWareTableSize);
		break;
	case BGRT:
		for (DWORD i = 0; i < FirmwareTableID.size(); ++i)
		{
			if (Table_Type == FirmwareTableID[i])
			{
				location = i;
				break;
			}
			location = -1;
		}
		if (location == 0xFFFFFFFF)		break;
		memcpy_s(&bgrt, sizeof(bgrt), FirewareTable[location].FireWareTable, FirewareTable[location].FireWareTableSize);
		break;
	case MCFG:
		for (DWORD i = 0; i < FirmwareTableID.size(); ++i)
		{
			if (Table_Type == FirmwareTableID[i])
			{
				location = i;
				break;
			}
			location = -1;
		}
		if (location == 0xFFFFFFFF)		break;
		memcpy_s(&mcfg, sizeof(mcfg), FirewareTable[location].FireWareTable, FirewareTable[location].FireWareTableSize);
		break;
	case SSDT:
// 		for (DWORD i = 0; i < FirmwareTableID.size(); ++i)
// 		{
// 			if (Table_Type == FirmwareTableID[i])
// 			{
// 				location = i;
// 				break;
// 			}
// 			location = -1;
// 		}
// 		if (location == 0xFFFFFFFF)		break;
		break;
	default:
		break;
	}
}

bool CACPI::Get_FirmWare_Enum()
{
	USHORT ret = 0;
	ret = EnumSystemFirmwareTables('ACPI', NULL, NULL);
	const USHORT buffersize = ret;
	PVOID buf = new BYTE[buffersize];
	PDWORD p_FirmwareTable_ID = new DWORD;
	PDWORD temp = p_FirmwareTable_ID;
	memset(buf, 0, buffersize);
	ret = EnumSystemFirmwareTables('ACPI', buf, buffersize);
	if (!ret)
	{
		printf("Function failed!\n");
		SafeDeleteArray(buf);
		return FALSE;
	}
	p_FirmwareTable_ID = (PDWORD)buf;
	for (USHORT i = 0; i < ret/4; ++i)
	{
		//if (*p_FirmwareTable_ID == 'TDSS')		continue;
		FirmwareTableID.emplace_back(*p_FirmwareTable_ID);
		p_FirmwareTable_ID++;
	}
	SafeDeleteArray(buf);
	SafeDeletePoint(temp);
	return TRUE;
}

void CACPI::PrintACPITable(DWORD TableName)
{
	PUCHAR Name = new UCHAR[sizeof(DWORD) * 2];
	memset(Name, 0, sizeof(Name));
	memcpy_s(Name, sizeof(Name), &TableName, sizeof(TableName));
	for (DWORD i = 0; i < sizeof(DWORD); ++i)
	{
		OutputDebugPrintf("%s",Name[i]);
		if (i == 3)
		{
			OutputDebugPrintf("\n");
		}
	}
	SafeDeleteArray(Name);
}

BOOL CACPI::Get_ACPI_Table(const DWORD Table_Type, const size_t Num)
{
	USHORT ret = 0;
// 	if (Table_Type == 'TDSS')
// 	{
// 		return TRUE;
// 	}
	ret = GetSystemFirmwareTable('ACPI', Table_Type, NULL, NULL);
	if (!ret)
	{
		OutputDebugPrintf("Function failed!\n");
		return FALSE;
	}
	const USHORT buffersize = ret;
	PUCHAR buf = new BYTE[buffersize];
	memset(buf, 0, buffersize);
	ret = GetSystemFirmwareTable('ACPI', Table_Type, buf, buffersize);
	if (!ret)
	{
		OutputDebugPrintf("Function failed!\n");
		this->SafeDeleteArray(buf);
		return FALSE;
	}

	FirewareTable[Num].FireWareTable = new UCHAR[buffersize];
	memset(FirewareTable[Num].FireWareTable, 0, buffersize);
	memcpy_s(FirewareTable[Num].FireWareTable, buffersize, buf, buffersize);
	FirewareTable[Num].FireWareTableSize = buffersize;
	
	//Test Code To Show Memory
// 	PrintACPITable(Table_Type);
// 	for (size_t j = 0; j < buffersize; ++j)
// 	{
// 		cout << hex << setfill('0') << setw(2) << int(*(buf + j)) << " ";
// 		if ((j + 1) % 16 == 0)
// 		{
// 			cout << endl;
// 		}
// 	}
// 	cout << endl;
	this->SafeDeleteArray(buf);
	return TRUE;
}





