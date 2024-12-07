#pragma once
#include "../ImGui/imconfig.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx9.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui_internal.h"
#include "../ImGui/imstb_rectpack.h"
#include "../ImGui/imstb_textedit.h"
#include "../ImGui/imstb_truetype.h"
#include <dxgiformat.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include<iostream>
#include<Windows.h>
#include<thread>
#include<tchar.h>

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#include<dwmapi.h>
#pragma comment(lib,"dwmapi.lib")



namespace myimgui
{
    struct windowinfo				//������Ϣ�ṹ��
    {
        HWND hwnd = nullptr;					//���ھ��

        LPCSTR ClassName = nullptr;		//��������
        LPCSTR TitleName = nullptr;		//���ڱ�����

        ImVec2 pos = { 0,0 };					//����λ��
        ImVec2 size = { 0,0 };				//���ڳߴ�
    };
	typedef void (*myFun)(void);
	int CreateWindow_Violet(LPCSTR ����, LPCSTR title_name, myimgui::myFun fun, const char* Fontsname, float Fonts_size, bool Synclnterval, int Menustyle);
	inline windowinfo mywindow;		//͸�����ڵĴ�����Ϣ
	inline windowinfo gamewindow;		//��Ϸ���ڵĴ�����Ϣ

    void UpdateWindow();
}


inline static LPDIRECT3D9              g_pD3D = nullptr;
inline static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
inline static bool                     g_DeviceLost = false;
inline static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
inline static D3DPRESENT_PARAMETERS    g_d3dpp = {};
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
