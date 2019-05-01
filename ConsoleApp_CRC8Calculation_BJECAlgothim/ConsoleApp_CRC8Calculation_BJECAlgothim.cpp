// ConsoleApp_CRC8Calculation_BJECAlgothim.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

// CMakeProject1_CRC8Calculation_BJECAlgothim.cpp: 定义应用程序的入口点。
//

#include <stdio.h> 

using namespace std;

#define CeE2ELib_u_CRC8Algorithm_J1850 1u
#define CeE2ELib_u_CRC8Algorithm_BJEV 2u
#define CeE2ELib_u_CRC8AlgorithmType  CeE2ELib_u_CRC8Algorithm_BJEV

#define CeE2ELib_u_CRC8Init 0xFFu
#define CeE2ELib_u_CRC8Polynomial 0x1Du
#define CeE2ELib_u_CRC8XORValue 0xFFu

typedef unsigned char uint8;

static void E2ELib_CRC8_J1850Calc(const uint8* DataPtr, uint8 DataLen, uint8* CRCResult);

int main()
{
	char str[200];
	uint8 InputData[200] = { 'G' };
	uint8 index;
	uint8 Index_InputData = 0;
	uint8 CRCreselt_2 = 0;

	cout << "Input data for calculating:" << '\n';
	scanf_s("%[^\n]", &str[0],200); //读到'\n'结束读取
	//cin >> str;

	for (index = 0; index < strlen(str); index++)
	{
		if (str[index] == ' ')
		{
			/*do nothing*/
		}
		else if ((str[index] >= '0') && (str[index] <= '9'))
		{
			InputData[Index_InputData] = str[index] - '0';
			Index_InputData++;
		}
		else if ((str[index] >= 'a') && (str[index] <= 'f'))
		{
			InputData[Index_InputData] = str[index] - 87;
			Index_InputData++;
		}
		else if ((str[index] >= 'A') && (str[index] <= 'F'))
		{
			InputData[Index_InputData] = str[index] - 55;
			Index_InputData++;
		}
		else
		{
			printf("there is some illegal char");
		}
	}

#if 0
	for (Index_InputData = 0; Index_InputData < 200; Index_InputData++)
	{
		printf("%X", InputData[Index_InputData]);
	}
#endif

	for (index = 0; index < Index_InputData; index += 2)
	{
		InputData[index] <<= 4;
		InputData[index] |= InputData[index + 1];
		InputData[index / 2] = InputData[index];
	}

	E2ELib_CRC8_J1850Calc(&InputData[0], index / 2, &CRCreselt_2);

	printf("%X", CRCreselt_2);

	return 0;

}


static void E2ELib_CRC8_J1850Calc(const uint8* DataPtr, uint8 DataLen, uint8* CRCResult)
{
	uint8 CalcTemp;
	uint8 ByteIndex;
	uint8 BitIndex;

#if (CeE2ELib_u_CRC8AlgorithmType == CeE2ELib_u_CRC8Algorithm_J1850)

	CalcTemp = CeE2ELib_u_CRC8Init;

	for (ByteIndex = 0u; ByteIndex < DataLen; ByteIndex++)
	{
		CalcTemp ^= *(DataPtr + ByteIndex);

		for (BitIndex = 0u; BitIndex < 8u; BitIndex++)
		{
			if (0u != (CalcTemp & 0x80u))
			{
				CalcTemp <<= 1u;
				CalcTemp ^= CeE2ELib_u_CRC8Polynomial;
			}
			else
			{
				CalcTemp <<= 1u;
			}
		}
	}

	CalcTemp ^= CeE2ELib_u_CRC8XORValue;

#endif

#if (CeE2ELib_u_CRC8AlgorithmType == CeE2ELib_u_CRC8Algorithm_BJEV)

	/* step1 follow as BJEC SPEC*/
	CalcTemp = CeE2ELib_u_CRC8Init;

	CalcTemp ^= 0xFFu;

	CalcTemp ^= *(DataPtr + 0u);   /*DataID Low*/

	for (BitIndex = 0u; BitIndex < 8u; BitIndex++)
	{
		if (0u != (CalcTemp & 0x80u))
		{
			CalcTemp <<= 1u;
			CalcTemp ^= CeE2ELib_u_CRC8Polynomial;
		}
		else
		{
			CalcTemp <<= 1u;
		}
	}

	CalcTemp ^= CeE2ELib_u_CRC8XORValue;

	/* step2 follow as BJEC SPEC*/
	CalcTemp ^= 0xFFu;

	for (ByteIndex = 1u; ByteIndex < DataLen; ByteIndex++)  /*CAN message Byte1~7*/
	{
		CalcTemp ^= *(DataPtr + ByteIndex);

		for (BitIndex = 0u; BitIndex < 8u; BitIndex++)
		{
			if (0u != (CalcTemp & 0x80u))
			{
				CalcTemp <<= 1u;
				CalcTemp ^= CeE2ELib_u_CRC8Polynomial;
			}
			else
			{
				CalcTemp <<= 1u;
			}
		}
	}

	CalcTemp ^= CeE2ELib_u_CRC8XORValue;

	/* step3 follow as BJEC SPEC*/
	CalcTemp ^= 0xFFu;

#endif

	* CRCResult = CalcTemp;
}


