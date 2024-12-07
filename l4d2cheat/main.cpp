#include"main.h"



void RandomTitle()
{
	constexpr int length = 25;
	const auto characters = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`-=~!@#$%^&*()_+,./;'[]|{}:?甲乙丙丁戊己庚辛壬癸子丑寅卯辰巳午未申酉戌亥");
	TCHAR title[length + 1]{};
	for (int j = 0; j != length; j++)
	{
		title[j] += characters[rand() % 95];
	}
	SetConsoleTitle(title);
}

void initGame() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	srand((unsigned)time(NULL));
	RandomTitle();
	//获取窗口句柄
	基址定义::g_hwnd = FindWindowA(XorStr("Valve001"), XorStr("Left 4 Dead 2 - Direct3D 9"));
	
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
	std::cout << "HWND：" << 基址定义::g_hwnd << std::endl;
	//获取窗口对应的进程ID
	GetWindowThreadProcessId(基址定义::g_hwnd, &基址定义::g_pid);
	
	//获取窗口对应的进程句柄
	基址定义::g_handle = OpenProcess(PROCESS_ALL_ACCESS, true, 基址定义::g_pid);
	基址定义::g_handle = OpenProcess(PROCESS_ALL_ACCESS, true, 基址定义::g_pid);
	//获取client.dll模块的地址
	if (基址定义::clientAddress = mem::GetModule(基址定义::g_pid, L"client.dll")) {
		std::cout << XorStr("获取clientAddress成功") << std::endl;
	}
	if (基址定义::serverAddress = mem::GetModule(基址定义::g_pid, L"server.dll")) {
		printf("serverAddress地址：%p\n", 基址定义::serverAddress);
		
	}
	if (基址定义::engineAddress = mem::GetModule(基址定义::g_pid, L"engine.dll")) {
		printf("engineAddress地址：%p\n", 基址定义::engineAddress);
		
	}
	
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	printf("pid地址：%d\n", 基址定义::g_pid);
	printf("clientAddress地址：%p\n", 基址定义::clientAddress);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}



int main()
{

	initGame();
	

	myimgui::CreateWindow_Violet(XorStr("Valve001"), "Left 4 Dead 2 - Direct3D 9", &fun, "C:\\Windows\\Fonts\\simkai.ttf",
		18.0f, false, 1);
	




}
