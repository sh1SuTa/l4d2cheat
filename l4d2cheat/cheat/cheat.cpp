#include "cheat.h"

void fun() {
	POINT Point{};
	RECT Rect{};
	GetClientRect(基址定义::g_hwnd, &Rect);
	ClientToScreen(基址定义::g_hwnd, &Point);
	cheat::g_size = ImVec2((float)Rect.right, (float)Rect.bottom);
	cheat::屏幕宽度 = (Rect.right - Rect.left) / 2;
	cheat::屏幕高度 = (Rect.bottom - Rect.top) / 2;
	
	ShowImguiMenu();
	Traverse();
}

void Traverse() {
	mem::Read(基址定义::g_handle, 基址定义::engineAddress + 0x6487C, &cheat::矩阵地址, sizeof(cheat::矩阵地址));
	mem::Read(基址定义::g_handle, cheat::矩阵地址 + 0x20c, &cheat::Matrix, sizeof(cheat::Matrix));
	
	//遍历对象玩家地址
	
	for (int i = 0; i < 100; i++)
	{
		if (!ReadLocalPawn()) {//包含了血量，队伍
			continue;
		}
		//对象玩家地址[0]获取处
		if (!mem::Read(基址定义::g_handle, 基址定义::clientAddress + 0x72213C, &cheat::ActorPlayer.Address[0], 8)) {

			continue;
		}
		
		//对象玩家地址[1]获取处
		if (!mem::Read(基址定义::g_handle, cheat::ActorPlayer.Address[0]  + 8 * i, &cheat::ActorPlayer.Address[1], 8)) {
			continue;
		}
		
		if (!ReadActorPawn())//读取其他人坐标，每一次for循环就是一个实体
		{
			continue;
		}

		//计算2D方框大小
		The2DBoxsize();

		if (Menu::box == 1) {
			绘制框框();
		}


		if (Menu::Health)
		{
			绘制血条();
		}
		
		
	}
	if (Menu::DrawFov)
	{
		ImGui::GetForegroundDrawList()->AddCircle({ cheat::屏幕宽度,cheat::屏幕高度 }, Menu::Fov * 8, ImColor(255, 155, 255));
	}

	
	
}



//读取本地玩家
bool ReadLocalPawn() {
	
	//基础地址
	if ( !mem::Read(基址定义::g_handle, 基址定义::clientAddress + 0x0726BD8, &cheat::LocalPlayer.Address[0], 8) ) {
		cout << "读取基址失败：" << endl;
		return false;
	}
	
	//血量
	if (!mem::Read(基址定义::g_handle, cheat::LocalPlayer.Address[0] + 0xEC, &cheat::LocalPlayer.Health, 4)	) {
		
		return false;
	}

	return true;
}

//读取对象玩家Pawn
bool ReadActorPawn() {
	//血量
	if (!mem::Read(基址定义::g_handle, cheat::ActorPlayer.Address[1] + 0xEC, &cheat::ActorPlayer.Health, 4)) {
		cout << "读取对象血量失败：" << endl;
		return false;
	}
	if (cheat::ActorPlayer.Health < 0 )
	{
		return false;
	}
	//X
	if (	!mem::Read(基址定义::g_handle, cheat::ActorPlayer.Address[1] + 0x124, &cheat::ActorPlayer.Axis.x, 8)	) {
		//cout << "读取对象X失败：" << endl;
		return false;
	}
	//x过滤
	if (cheat::ActorPlayer.Axis.x == 599) {
		return false;
	}
	cout << "X坐标：" << cheat::ActorPlayer.Axis.x << std::endl;
	//Y
	if (
		!mem::Read(基址定义::g_handle, cheat::ActorPlayer.Address[1] + 0x128, &cheat::ActorPlayer.Axis.y, 4)
		) {
		return false;
	}
	//cout << "y坐标：" << cheat::ActorPlayer.Axis.y << std::endl;
	//Z
	if (	!mem::Read(基址定义::g_handle, cheat::ActorPlayer.Address[1] + 0x12C, &cheat::ActorPlayer.Axis.z, 4)	) {
		return false;
	}
	cout << "z坐标：" << cheat::ActorPlayer.Axis.z << std::endl;
	//过滤坐标
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


//三维转二维
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

//计算2D方框大小
bool The2DBoxsize() {
	//脚部
	float 世界坐标[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z };
	float 屏幕坐标[2];
	if (!WorldScreen2d(世界坐标, 屏幕坐标))
	{
		return false;
	}
	//头部
	float 世界坐标2[3] = { cheat::ActorPlayer.Axis.x, cheat::ActorPlayer.Axis.y ,cheat::ActorPlayer.Axis.z + 70.f };
	float 屏幕坐标2[2];
	if (!WorldScreen2d(世界坐标2, 屏幕坐标2))
	{
		return false;
	}
	cheat::ActorPlayer.x1 = 屏幕坐标2[0] - (屏幕坐标[1] - 屏幕坐标2[1]) / 4.f;
	cheat::ActorPlayer.y1 = 屏幕坐标2[1];
	cheat::ActorPlayer.x2 = 屏幕坐标[0] + (屏幕坐标[1] - 屏幕坐标2[1]) / 4.f;
	cheat::ActorPlayer.y2 = 屏幕坐标[1];
	return true;
}

void 绘制血条() {

	float health = (cheat::ActorPlayer.Health / 100.f) * (cheat::ActorPlayer.y1 - cheat::ActorPlayer.y2);
	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	drawList->AddRect({ cheat::ActorPlayer.x1 - 7,cheat::ActorPlayer.y1 }, { cheat::ActorPlayer.x1 - 2,cheat::ActorPlayer.y2 }, ImColor(0, 0, 0));

	ImGui::GetForegroundDrawList()->AddRectFilledMultiColor(
		{ cheat::ActorPlayer.x1 - 3,cheat::ActorPlayer.y2 + health },
		{ cheat::ActorPlayer.x1 - 6,cheat::ActorPlayer.y2 }, ImColor(78, 173, 0), ImColor(137, 115, 0), ImColor(196, 58, 0), ImColor(255, 0, 0)
	);
}
