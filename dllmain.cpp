#include <windows.h>
#include <iostream>
#include <vector>
#include "pch.h"
using namespace std;

//Identificadores dos controles
//Cada elemento criado, precisa criar uma variavel pra ele nesse ENUM
enum Botoes {
    Botao_1 = 101,
    Botao_2 = 102,
    Input_Text = 103,
    CheckBox = 104,
    Botao_3 = 105,
    ComboBox = 106,
    Textarea = 107,
    ProgressBar = 108,
};

LPCWSTR TitulosJanela = L"Biroleibe";
int LarguraJanela = 500;
int AlturaJanela = 500;
int CorDaJanelaRGB[] = { 100,100,100 };
 
 
 
#define IDC_TEXTAREA        104 
#define IDC_RADIO1          106
#define IDC_RADIO2          107
#define IDC_PROGRESS        108
#define IDC_CHECKBOX_STATUS 109
#define IDC_INPUT_TEXT      110
#define IDC_TEXTAREA_TEXT   111

// Definição da mensagem PBM_SETPOS
#define PBM_SETPOS      (WM_USER+2)


LPCWSTR GET_INPUT_TEXT(HWND hwnd,int BotaoENUM)
{
    WCHAR buffer[5000];
    GetWindowTextW(GetDlgItem(hwnd, BotaoENUM), buffer, 5000);
    return buffer;
}

void MsgBox_OK(HWND hwnd, LPCWSTR titulo, LPCWSTR texto)
{
    MessageBoxW(hwnd, texto, titulo, MB_OK | MB_ICONINFORMATION);
}

bool IF_CHECKBOX_CHECKED(HWND hwnd, int CheckboxEnum )
{
    BOOL oi =  SendMessageW(GetDlgItem(hwnd, CheckboxEnum), BM_GETCHECK, 0, 0) == BST_CHECKED;
    return oi;
}

bool CLICOU_NO_BOTAO(HWND hwnd, WPARAM wParam, int BotaoENUM)
{
    if (LOWORD(wParam) == BotaoENUM && HIWORD(wParam) == BN_CLICKED)
        return true;
    else
        return false;
}

 

void addButton(HWND hwnd, int BotaoENUM ,LPCWSTR Titulo = L"Botao", int posx = 10, int posy = 10, int largura = 80, int altura = 25 )
{
    CreateWindowW(L"BUTTON", Titulo, WS_VISIBLE | WS_CHILD, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
}

void addInput(HWND hwnd, int BotaoENUM ,LPCWSTR placeholder = L"", int posx = 10, int posy = 10, int largura = 80, int altura = 25 )
{
    CreateWindowW(L"EDIT", placeholder, WS_VISIBLE | WS_CHILD, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
}

void addCheckBox(HWND hwnd, int BotaoENUM ,LPCWSTR Titulo = L"Checkbox", int posx = 10, int posy = 155, int largura = 100, int altura = 25 )
{
    CreateWindowW(L"BUTTON", Titulo, WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
 
}

void addCombobox(HWND hwnd, int BotaoENUM , int posx = 260, int posy = 10, int largura = 150, int altura = 200)
{ 
    CreateWindowW(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
   
}

void addComboboxItens(HWND hwnd, int BotaoENUM, LPCWSTR Item)
{
    SendMessageW(GetDlgItem(hwnd, BotaoENUM), CB_ADDSTRING, 0, (LPARAM)Item ); 
}

void addTextarea(HWND hwnd, int BotaoENUM, int posx = 10, int posy = 45, int largura = 400, int altura = 100) {
    CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
}

void addProgressBar(HWND hwnd, int BotaoENUM, int posx = 10, int posy = 215, int largura = 400, int altura = 25)
{
    CreateWindowW(L"msctls_progress32", NULL, WS_VISIBLE | WS_CHILD, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
}

void SET_PROGRESS_BAR_VALUE(HWND hwnd, int BotaoENUM, int valor = 10)
{
    SendMessageW(GetDlgItem(hwnd, BotaoENUM), (WM_USER + 2), valor, 0);
}

 

// Função de callback para o procedimento da janela
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        // Criar os controles quando a janela é criada
        addButton(hwnd, Botoes::Botao_1, L"Botao 1", 10, 10, 80, 25 );
        addCheckBox(hwnd, Botoes::CheckBox,L"Minha checkbox");
        addInput(hwnd, Botoes::Input_Text);
         
        addCombobox(hwnd, Botoes::ComboBox );
        addComboboxItens(hwnd, Botoes::ComboBox,  L"Vermelhor");
        addComboboxItens(hwnd, Botoes::ComboBox, L"Azul");
        addComboboxItens(hwnd, Botoes::ComboBox, L"Pinto Murcho");

        addTextarea(hwnd, Botoes::Textarea);

        addProgressBar(hwnd, Botoes::ProgressBar);

        //CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 10, 150, 25, hwnd, (HMENU)Botoes::Input_Text, NULL, NULL);
        //CreateWindowW(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 260, 10, 150, 200, hwnd, (HMENU)IDC_COMBOBOX, NULL, NULL);
        //SendMessageW(GetDlgItem(hwnd, IDC_COMBOBOX), CB_ADDSTRING, 0, (LPARAM)L"Opção 1");
        //SendMessageW(GetDlgItem(hwnd, IDC_COMBOBOX), CB_ADDSTRING, 0, (LPARAM)L"Opção 2");
        //SendMessageW(GetDlgItem(hwnd, IDC_COMBOBOX), CB_ADDSTRING, 0, (LPARAM)L"Opção 3");
        //CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 10, 45, 400, 100, hwnd, (HMENU)IDC_TEXTAREA, NULL, NULL);
        
        CreateWindowW(L"BUTTON", L"Opção 1", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 10, 185, 100, 25, hwnd, (HMENU)IDC_RADIO1, NULL, NULL);
        CreateWindowW(L"BUTTON", L"Opção 2", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 120, 185, 100, 25, hwnd, (HMENU)IDC_RADIO2, NULL, NULL);
        //CreateWindowW(L"msctls_progress32", NULL, WS_VISIBLE | WS_CHILD, 10, 215, 400, 25, hwnd, (HMENU)IDC_PROGRESS, NULL, NULL);
        
        
        //SendMessageW(GetDlgItem(hwnd, IDC_PROGRESS), PBM_SETPOS, 50, 0);

        //CreateWindowW(L"BUTTON", L"Verificar Checkbox", WS_VISIBLE | WS_CHILD, 10, 250, 150, 25, hwnd, (HMENU)IDC_CHECKBOX_STATUS, NULL, NULL);
        //CreateWindowW(L"BUTTON", L"Exibir Texto do Input", WS_VISIBLE | WS_CHILD, 170, 250, 150, 25, hwnd, (HMENU)IDC_INPUT_TEXT, NULL, NULL);
        //CreateWindowW(L"BUTTON", L"Exibir Texto do Textarea", WS_VISIBLE | WS_CHILD, 330, 250, 150, 25, hwnd, (HMENU)IDC_TEXTAREA_TEXT, NULL, NULL);
        break;
    }
    case WM_COMMAND: {
        if (CLICOU_NO_BOTAO(hwnd,wParam,Botoes::Botao_1))
        {
             MsgBox_OK(hwnd,  L"bla", L"Clicou VOID string"); 
        }
        else if (LOWORD(wParam) == IDC_CHECKBOX_STATUS && HIWORD(wParam) == BN_CLICKED)
        {
            if (IF_CHECKBOX_CHECKED(hwnd, Botoes::CheckBox))
                MsgBox_OK(hwnd, L"bla", L"CheckBox marcado");
            else
                MsgBox_OK(hwnd, L"bla", L"Checkbox nao marcado");
        }
        else if (LOWORD(wParam) == IDC_INPUT_TEXT && HIWORD(wParam) == BN_CLICKED) 
        {
             
            MsgBox_OK(hwnd,  L"bla", GET_INPUT_TEXT(hwnd, Botoes::Input_Text));
        }
        else if (LOWORD(wParam) == IDC_TEXTAREA_TEXT && HIWORD(wParam) == BN_CLICKED) 
        {
            MsgBox_OK(hwnd, L"bla", GET_INPUT_TEXT(hwnd, Botoes::Textarea));
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void loop()
{
     
}

DWORD WINAPI ThreadProc(LPVOID lpParam) {
    HINSTANCE hInstance = (HINSTANCE)lpParam;

    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MainWindow";
    HBRUSH hBrush = CreateSolidBrush(RGB(CorDaJanelaRGB[0], CorDaJanelaRGB[1], CorDaJanelaRGB[2]));
    wc.hbrBackground = hBrush;
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(0, L"MainWindow", TitulosJanela , WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, LarguraJanela, AlturaJanela, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        loop();
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, ThreadProc, hModule, 0, NULL);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
