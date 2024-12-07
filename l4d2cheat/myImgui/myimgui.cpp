#include "myimgui.h"



int myimgui::CreateWindow_Violet(LPCSTR 类名, LPCSTR title_name, myimgui::myFun fun, const char* Fontsname, float Fonts_size, bool Synclnterval, int Menustyle)
{
    gamewindow.ClassName = 类名;
    gamewindow.TitleName = title_name;
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);

    mywindow.hwnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOOLWINDOW,
        wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_POPUP, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);
    SetLayeredWindowAttributes(mywindow.hwnd, 0x00000000, 0, LWA_COLORKEY);

    if (!CreateDeviceD3D(mywindow.hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }


    ::ShowWindow(mywindow.hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(mywindow.hwnd);

    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    //菜单样式
    if (Menustyle == 0) { ImGui::StyleColorsDark(); }
    if (Menustyle == 1) { ImGui::StyleColorsLight(); }

    ImGui_ImplWin32_Init(mywindow.hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);
    //设置字体大小
    ImFont* font = io.Fonts->AddFontFromFileTTF(Fontsname, Fonts_size, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);

    // Main loop
    bool done = false;
    while (!done)
    {

        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            exit(0);

        UpdateWindow();

        

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }


        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        fun();
        ImGui::Render();
        

        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w ), (int)(clear_color.y * clear_color.w ), (int)(clear_color.z * clear_color.w ), 0);
        //清除重复的菜单
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
        
    }

    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(mywindow.hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

void myimgui::UpdateWindow()
{
    POINT Point{};
    RECT Rect{};

    //查找目标窗口
    gamewindow.hwnd = FindWindowA(gamewindow.ClassName, gamewindow.TitleName);

    //获取目标窗口位置
    GetClientRect(gamewindow.hwnd, &Rect);
    ClientToScreen(gamewindow.hwnd, &Point);

    //更新透明窗口位置和大小
    mywindow.pos = gamewindow.pos = ImVec2((float)Point.x, (float)Point.y);
    mywindow.size = gamewindow.size = ImVec2((float)Rect.right, (float)Rect.bottom);
    SetWindowPos(mywindow.hwnd, HWND_TOPMOST, (int)mywindow.pos.x, (int)mywindow.pos.y, (int)mywindow.size.x, (int)mywindow.size.y, SWP_SHOWWINDOW);

    //获取鼠标位置 同步到imgui中
    POINT MousePos;
    GetCursorPos(&MousePos);
    ScreenToClient(mywindow.hwnd, &MousePos);
    ImGui::GetIO().MousePos.x = (float)MousePos.x;
    ImGui::GetIO().MousePos.y = (float)MousePos.y;

    //鼠标穿透
    if (ImGui::GetIO().WantCaptureMouse)
    {
        //如果鼠标在imgui菜单区域中   那就不把他设置成分层窗口
        //printf("WantCaptureMouse=true\n");        //0xfff7ffff 会剩下当前的属性 但是会筛掉WS_EX_LAYERED属性
        SetWindowLong(mywindow.hwnd, GWL_EXSTYLE, GetWindowLong(mywindow.hwnd, GWL_EXSTYLE) & (~WS_EX_LAYERED));

    }
    else
    {
        //如果不鼠标在imgui菜单区域中  那就把他设置成分层窗口
        //printf("WantCaptureMouse=false\n");
        SetWindowLong(mywindow.hwnd, GWL_EXSTYLE, GetWindowLong(mywindow.hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    }
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT WINAPI WndProcH(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); 
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_CREATE:
    {
        //启用Windows Aero 玻璃效果
        MARGINS Margin = { -1 };
        DwmExtendFrameIntoClientArea(hWnd, &Margin);
        break;
    }
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
