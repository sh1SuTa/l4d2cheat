#include "cheat.h"

void fun() {
	POINT Point{};
	RECT Rect{};
	GetClientRect(��ַ����::g_hwnd, &Rect);
	ClientToScreen(��ַ����::g_hwnd, &Point);
	cheat::g_size = ImVec2((float)Rect.right, (float)Rect.bottom);
	cheat::��Ļ��� = (Rect.right - Rect.left) / 2;
	cheat::��Ļ�߶� = (Rect.bottom - Rect.top) / 2;
	
	ShowImguiMenu();
	Traverse();
}

void Traverse() {
	mem::Read(��ַ����::g_handle, ��ַ����::engineAddress + 0x6487C, &cheat::�����ַ, sizeof(cheat::�����ַ));
	mem::Read(��ַ����::g_handle, cheat::�����ַ + 0x20c, &cheat::Matrix, sizeof(cheat::Matrix));
	
	//����������ҵ�ַ
	
	for (int i = 0; i < 100; i++)
	{
		if (!ReadLocalPawn()) {//������Ѫ��������
			continue;
		}
		//������ҵ�ַ[0]��ȡ��
		if (!mem::Read(��ַ����::g_handle, ��ַ����::clientAddress + 0x72213C, &cheat::ActorPlayer.Address[0], 8)) {

			continue;
		}
		
		//������ҵ�ַ[1]��ȡ��
		if (!mem::Read(��ַ����::g_handle, cheat::ActorPlayer.Address[0]  + 8 * i, &cheat::ActorPlayer.Address[1], 8)) {
			continue;
		}
		
		if (!ReadActorPawn())//��ȡ���������꣬ÿһ��forѭ������һ��ʵ��
		{
			continue;
		}

		//����2D�����С
		The2DBoxsize();

		if (Menu::box == 1) {
			���ƿ��();
		}


		if (Menu::Health)
		{
			����Ѫ��();
		}
		
		
	}
	if (Menu::DrawFov)
	{
		ImGui::GetForegroundDrawList()->AddCircle({ cheat::��Ļ���,cheat::��Ļ�߶� }, Menu::Fov * 8, ImColor(255, 155, 255));
	}

	
	
}



//��ȡ�������
bool ReadLocalPawn() {
	
	//������ַ
	if ( !mem::Read(��ַ����::g_handle, ��ַ����::clientAddress + 0x0726BD8, &cheat::LocalPlayer.Address[0], 8) ) {
		cout << "��ȡ��ַʧ�ܣ�" << endl;
		return false;
	}
	
	//Ѫ��
	if (!mem::Read(��ַ����::g_handle, cheat::LocalPlayer.Address[0] + 0xEC, &cheat::LocalPlayer.Health, 4)	) {
		
		return false;
	}

	return true;
}

//��ȡ�������Pawn
bool ReadActorPawn() {
	//Ѫ��
	if (!mem::Read(��ַ����::g_handle, cheat::ActorPlayer.Address[1] + 0xEC, &cheat::ActorPlayer.Health, 4)) {
		cout << "��ȡ����Ѫ��ʧ�ܣ�" << endl;
		return false;
	}
	if (cheat::ActorPlayer.Health < 0 )
	{
		return false;
	}
	//X
	if (	!mem::Read(��ַ����::g_handle, cheat::ActorPlayer.Address[1] + 0x124, &cheat::ActorPlayer.Axis.x, 8)	) {
		//cout << "��ȡ����Xʧ�ܣ�" << endl;
		return false;
	}
	//x����
	if (cheat::ActorPlayer.Axis.x == 599) {
		return false;
	}
	cout << "X���꣺" << cheat::ActorPlayer.Axis.x << std::endl;
	//Y
	if (
		!mem::Read(��ַ����::g_handle, cheat::ActorPlayer.Address[1] + 0x128, &cheat::ActorPlayer.Axis.y, 4)
		) {
		return false;
	}
	//cout << "y���꣺" << cheat::ActorPlayer.Axis.y << std::endl;
	//Z
	if (	!mem::Read(��ַ����::g_handle, cheat::ActorPlayer.Address[1] + 0x12C, &cheat::ActorPlayer.Axis.z, 4)	) {
		return false;
	}
	cout << "z���꣺" << cheat::ActorPlayer.Axis.z << std::endl;
	//��������
	if (cheat::ActorPlayer.Axis.y == 0 && cheat::ActorPlayer.Axis.z == 0 )
	{
		return false;
	}
	
	if (cheat::ActorPlayer.Axis.x < 1 && cheat::ActorPlayer.Axis.x >=0 )
	{
		if (cheat::ActorPlayer.Axis.z < 1 && cheat::ActorPlayer.Axis.z >= 0)
		{
		
			return false;
		}
		
	}
	if (cheat::ActorPlayer.Axis.z>3100)
	{

		return false;
	}
	
	return true;

}


//��άת��ά
bool WorldScreen2d(_In_ float world[3], _Out_ float screen[2]) {

	float x = cheat::Matrix[0][0] * world[0] + cheat::Matrix[0][1] * world[1] + cheat::Matrix[0][2] * world[2] + cheat::Matrix[0][3] * 1;
	float y = cheat::Matrix[1][0] * world[0] + cheat::Matrix[1][1] * world[1] + cheat::Matrix[1][2] * world[2] + cheat::Matrix[1][3] * 1;

	float w = cheat::Matrix[3][0] * world[0] + cheat::Matrix[3][1] * world[1] + cheat::Matrix[3][2] * world[2] + cheat::Matrix[3][3] * 1;

	if (w < 0.01f)
	{
		return false;
	}
	x = x / w;
	y = y / w;

	float halfwide = cheat::g_size.x * 0.5f;
	float halfheigh = cheat::g_size.y * 0.5f;

	screen[0] = halfwide * x + (halfwide + x);
	screen[1] = -(halfheigh * y) + (halfheigh + y);

	return true;
}

//����2D�����С
bool The2DBoxsize() {
	//�Ų�
	float ��������[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z };
	float ��Ļ����[2];
	if (!WorldScreen2d(��������, ��Ļ����))
	{
		return false;
	}
	//ͷ��
	float ��������2[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z + 70.f };
	float ��Ļ����2[2];
	if (!WorldScreen2d(��������2, ��Ļ����2))
	{
		return false;
	}
	cheat::ActorPlayer.x1 = ��Ļ����2[0] - (��Ļ����[1] - ��Ļ����2[1]) / 4.f;
	cheat::ActorPlayer.y1 = ��Ļ����2[1];
	cheat::ActorPlayer.x2 = ��Ļ����[0] + (��Ļ����[1] - ��Ļ����2[1]) / 4.f;
	cheat::ActorPlayer.y2 = ��Ļ����[1];
	return true;
}

void ����Ѫ��() {

	float health = (cheat::ActorPlayer.Health / 100.f) * (cheat::ActorPlayer.y1 - cheat::ActorPlayer.y2);
	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	drawList->AddRect({ cheat::ActorPlayer.x1 - 7,cheat::ActorPlayer.y1 }, { cheat::ActorPlayer.x1 - 2,cheat::ActorPlayer.y2 }, ImColor(0, 0, 0));

	ImGui::GetForegroundDrawList()->AddRectFilledMultiColor(
		{ cheat::ActorPlayer.x1 - 3,cheat::ActorPlayer.y2 + health },
		{ cheat::ActorPlayer.x1 - 6,cheat::ActorPlayer.y2 }, ImColor(78, 173, 0), ImColor(137, 115, 0), ImColor(196, 58, 0), ImColor(255, 0, 0)
	);
}
