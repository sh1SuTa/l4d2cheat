#pragma once



#pragma once

#include"../myImgui/myimgui.h"
#include "../myImgui/menu.h"
#include"../mem.h"
#include"../͸��.h"

#include<cmath>
#include <iostream>
#include<vector>
using namespace std;


struct D2D
{
	float x, y;
};
struct D3D
{
	float x, y, z;
};





namespace cheat {

	inline char* EnterAimAddress[2];
	inline float ActorDistance[2] = { 99999.f,999999.f };


	inline float ��Ļ���;
	inline float ��Ļ�߶�;




	inline ImVec2 g_size;

	inline char* �����ַ;
	inline float Matrix[4][4]; //����
	inline char* AimAddress[2];
	inline float FOV;//���鷶Χ
	inline D2D Aimmouse;


	struct ActorInfo
	{
		char* Address[2]; 	//������ַ
		char* SkeletonAddress[2]; //������ַ
		int Health; //Ѫ��
		int camp;	 //��Ӫ
		D3D Axis; //����
		float x1;
		float y1;
		float x2;
		float y2;
		float ����;
	};
	inline ActorInfo LocalPlayer; //����
	inline ActorInfo ActorPlayer; //�������

	inline std::vector<ActorInfo> targets; // �洢��ǰ������׼��Ŀ��



}

void fun();
void Traverse();
//��ȡ�������Pawn
bool ReadLocalPawn();
//��ȡ�������Pawn
bool ReadActorPawn();
//��άת��ά
bool WorldScreen2d(_In_ float world[3], _Out_ float screen[2]);
void ����Ѫ��();

bool The2DBoxsize();



void fun();
