#include <windows.h>
#include <CommCtrl.h>
#include <WinUser.h>
#include <iostream>
#include <vector>
#include "pch.h" 
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <tlhelp32.h>
using namespace std;

HANDLE g_hThread = NULL; // Handle da thread global
volatile bool g_bRun = true; // Vari�vel de controle global

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
    Radio1 = 109,
    CheckBox_Status = 110,
    RadioStatus = 111,
};

LPCWSTR TitulosJanela = L"Biroleibe do Piru";
int LarguraJanela = 500;
int AlturaJanela = 500;
int CorDaJanelaRGB[] = { 100,100,100 };
 
 
HFONT hFont = CreateFont(
    15,                        // Altura da fonte
    0,                         // Largura da fonte
    0,                         // �ngulo de escapamento
    0,                         // �ngulo de orienta��o
    FW_BOLD,                   // Espessura da fonte
    FALSE,                     // It�lico
    FALSE,                     // Sublinhado
    FALSE,                     // Riscado
    DEFAULT_CHARSET,           // Conjunto de caracteres
    OUT_DEFAULT_PRECIS,        // Precis�o de sa�da
    CLIP_DEFAULT_PRECIS,       // Precis�o de recorte
    DEFAULT_QUALITY,           // Qualidade da fonte
    DEFAULT_PITCH | FF_SWISS,  // Fam�lia e espa�amento da fonte
    L"Arial"                   // Nome da fonte
);

 
 


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

/*
bool IF_CHECKBOX_CHANGED(HWND hwnd, int CheckboxEnum )
{
    //LOWORD(wParam) == IDC_CHECKBOX_STATUS && HIWORD(wParam) == BN_CLICKED
    BOOL oi =  SendMessageW(GetDlgItem(hwnd, CheckboxEnum), BM_GETCHECK, 0, 0) == BN_CLICKED;
    return oi;
}
*/

/*
bool IF_RADIO_CHANGED(HWND hwnd, int RADIOEnum )
{
    //LOWORD(wParam) == IDC_CHECKBOX_STATUS && HIWORD(wParam) == BN_CLICKED
    BOOL oi =  SendMessageW(GetDlgItem(hwnd, RADIOEnum), BM_GETCHECK, 0, 0) == BN_CLICKED;
    return oi;
}
*/

BOOL IF_RADIO_CHECKED(HWND hwnd, int RadioENUM)
{
    BOOL oi = SendMessageW(GetDlgItem(hwnd, RadioENUM), BM_GETCHECK, 0, 0) == BST_CHECKED;
    return oi;
}

bool CLICOU_NO_BOTAO(HWND hwnd, WPARAM wParam, int BotaoENUM)
{
    if (LOWORD(wParam) == BotaoENUM && HIWORD(wParam) == BN_CLICKED)
        return true;
    else
        return false;
}


 

void addButton(HWND hwnd, int BotaoENUM ,LPCWSTR Titulo = L"Botao", int posx = 10, int posy = 10, int largura = 80, int altura = 25, COLORREF corTexto = RGB(0, 0, 0), COLORREF corFundo = RGB(255, 0, 255))
{
    HWND componente = CreateWindowW(L"BUTTON", Titulo, WS_VISIBLE | WS_CHILD, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
    SendMessage(componente, WM_SETFONT, (WPARAM)hFont, TRUE);
     
     
}
 

void addInput(HWND hwnd, int BotaoENUM ,LPCWSTR placeholder = L"", int posx = 10, int posy = 10, int largura = 80, int altura = 25 )
{
    HWND componente = CreateWindowW(L"EDIT", placeholder, WS_VISIBLE | WS_CHILD, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
    SendMessage(componente, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void addCheckBox(HWND hwnd, int BotaoENUM ,LPCWSTR Titulo = L"Checkbox", int posx = 10, int posy = 155, int largura = 100, int altura = 25 )
{
    HWND componente = CreateWindowW(L"BUTTON", Titulo, WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
    SendMessage(componente, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void addCombobox(HWND hwnd, int BotaoENUM , int posx = 260, int posy = 10, int largura = 150, int altura = 200)
{ 
    HWND componente = CreateWindowW(L"COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
    SendMessage(componente, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void addComboboxItens(HWND hwnd, int BotaoENUM, LPCWSTR Item)
{
      SendMessageW(GetDlgItem(hwnd, BotaoENUM), CB_ADDSTRING, 0, (LPARAM)Item );
    //SendMessage(componente, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void addTextarea(HWND hwnd, int BotaoENUM, int posx = 10, int posy = 45, int largura = 400, int altura = 100) {
    HWND componente = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
    SendMessage(componente, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void addProgressBar(HWND hwnd, int BotaoENUM, int posx = 10, int posy = 215, int largura = 400, int altura = 25)
{
    CreateWindowW(L"msctls_progress32", NULL, WS_VISIBLE | WS_CHILD, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
}

void SET_PROGRESS_BAR_VALUE(HWND hwnd, int BotaoENUM, int valor = 10)
{
    SendMessageW(GetDlgItem(hwnd, BotaoENUM), (WM_USER + 2), valor, 0);
}

void addRadio(HWND hwnd, int BotaoENUM, LPCWSTR Titulo = L"Radio", int posx = 10, int posy = 185, int largura = 100, int altura = 25) 
{
    HWND componente = CreateWindowW(L"BUTTON", Titulo, WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, posx, posy, largura, altura, hwnd, (HMENU)BotaoENUM, NULL, NULL);
    SendMessage(componente, WM_SETFONT, (WPARAM)hFont, TRUE);
}



 

 

// Fun��o para criar a lista
void addListBox(HWND hwnd, int ListENUM, int posx = 10, int posy = 10, int largura = 200, int altura = 150)
{
    // Criar o ListBox
    HWND hListBox = CreateWindowW(L"LISTBOX", NULL, WS_VISIBLE | WS_CHILD | LBS_STANDARD | LBS_NOTIFY,
        posx, posy, largura, altura, hwnd, (HMENU)ListENUM, NULL, NULL);
    SendMessage(hListBox, WM_SETFONT, (WPARAM)hFont, TRUE); 
}

// Fun��o para adicionar itens � lista
void addItemListBox(HWND hwnd, int ListENUM,LPCWSTR texto)
{
    // Adicionar um item ao ListBox
    SendMessage(GetDlgItem(hwnd, ListENUM), LB_ADDSTRING, 0, (LPARAM)texto);
}

// Fun��o para verificar se um item foi clicado
BOOL IF_SELECIONOU_ITEM_LISTBOX(HWND hwnd, WPARAM wParam, int ListENUM)
{
    // Mensagem de clique no ListBox
    if (HIWORD(wParam) == LBN_SELCHANGE) // Verifica se houve uma mudan�a na sele��o do ListBox
    {
        int indexSelecionado = SendMessage(GetDlgItem(hwnd, ListENUM), LB_GETCURSEL, 0, 0); // Obt�m o �ndice do item selecionado

        if (indexSelecionado != LB_ERR) // Verifica se o �ndice � v�lido
        {
            return true;
        }
    }
    return false;
}

// Fun��o para retornar o texto do item selecionado
LPCWSTR textoItemSelecionado(HWND hwnd,int ListENUM)
{
    int indexSelecionado = SendMessage(GetDlgItem(hwnd, ListENUM), LB_GETCURSEL, 0, 0); // Obt�m o �ndice do item selecionado

    if (indexSelecionado != LB_ERR) // Verifica se o �ndice � v�lido
    {
        static wchar_t textoSelecionado[256]; // Buffer para armazenar o texto selecionado
        SendMessage(GetDlgItem(hwnd, ListENUM), LB_GETTEXT, (WPARAM)indexSelecionado, (LPARAM)textoSelecionado); // Obt�m o texto do item selecionado
        return textoSelecionado;
    }
    else
    {
        return L""; // Retorna uma string vazia se nenhum item estiver selecionado
    }
}



 


void LOG(const std::string& nomeArquivo, const std::string& texto) { 
    std::ofstream arquivo(nomeArquivo, std::ios::app); 
    if (arquivo.is_open()) { 
        arquivo << texto << std::endl; 
        arquivo.close(); 
    } 
}

std::string LPCWSTRToString(LPCWSTR texto) { 
    std::wstring wtexto(texto);
    wstring_convert< codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wtexto);
}

extern "C" __declspec(dllexport) void UnloadDll(HMODULE hModule) {
    g_bRun = false;
    if (g_hThread) {
        WaitForSingleObject(g_hThread, INFINITE);
        CloseHandle(g_hThread);
    }
    FreeLibraryAndExitThread(hModule, 0);
}

 

DWORD GetProcessIdByName(const std::wstring& processName) {
    DWORD processId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32W);
        if (Process32FirstW(snapshot, &processEntry)) {
            do {
                if (_wcsicmp(processEntry.szExeFile, processName.c_str()) == 0) {
                    processId = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &processEntry));
        }
        CloseHandle(snapshot);
    }
    return processId;
}

int alterarINT(HWND hwnd)
{
    const std::wstring targetProcessName = L"notepad.exe";

    // Obter o ID do processo alvo
    DWORD processId = GetProcessIdByName(targetProcessName);
    if (processId == 0) {
        //std::cerr << "Processo n�o encontrado." << std::endl;
        MsgBox_OK(hwnd, L"bla", L"Processo n�o encontrado.");
        return 1;
    }

    // Abrir o processo alvo com permiss�es de leitura/escrita
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        //std::cerr << "Falha ao abrir o processo." << std::endl;
        MsgBox_OK(hwnd, L"bla", L"Falha ao abrir o processo.");
        return 1;
    }

    // Endere�o de mem�ria alvo e novo valor
    LPCVOID targetAddress = (LPCVOID)0xD1FC4B9BB0; // Substitua pelo endere�o real
    int newValue = 42; // Novo valor a ser escrito

    // Escrever o novo valor no endere�o de mem�ria alvo
    SIZE_T bytesWritten;
    if (WriteProcessMemory(hProcess, (LPVOID)targetAddress, &newValue, sizeof(newValue), &bytesWritten)) {
        //std::cout << "Mem�ria alterada com sucesso." << std::endl;
        MsgBox_OK(hwnd, L"bla", L"Mem�ria alterada com sucesso.");

    }
    else {
        //std::cerr << "Falha ao alterar a mem�ria." << std::endl;
        MsgBox_OK(hwnd, L"bla", L"Falha ao alterar a mem�ria.");
    }

    // Fechar o handle do processo
    CloseHandle(hProcess);
}

int altararFloat(HWND hwnd)
{
    // Nome do processo alvo
    const wchar_t* targetProcessName = L"target_process.exe";

    // Obter o ID do processo alvo
    DWORD processId = GetProcessIdByName(targetProcessName);
    if (processId == 0) {
        //std::cerr << "Processo n�o encontrado." << std::endl;
        return 1;
    }

    // Abrir o processo alvo com permiss�es de leitura/escrita
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        //std::cerr << "Falha ao abrir o processo." << std::endl;
        return 1;
    }

    // Endere�o de mem�ria alvo e novo valor
    LPVOID targetAddress = (LPVOID)0x12345678; // Substitua pelo endere�o real
    float newValue = 3.14f; // Novo valor a ser escrito (por exemplo, pi)

    // Escrever o novo valor no endere�o de mem�ria alvo
    SIZE_T bytesWritten;
    if (WriteProcessMemory(hProcess, targetAddress, &newValue, sizeof(newValue), &bytesWritten)) {
       // std::cout << "Mem�ria alterada com sucesso." << std::endl;
    }
    else {
       // std::cerr << "Falha ao alterar a mem�ria." << std::endl;
    }

    // Fechar o handle do processo
    CloseHandle(hProcess);

    return 0;
}


int lerMemoriaINT(HWND hwnd)
{
    // Nome do processo alvo
    const wchar_t* targetProcessName = L"notepad.exe";

    // Obter o ID do processo alvo
    DWORD processId = GetProcessIdByName(targetProcessName);
    if (processId == 0) {
        MsgBox_OK(hwnd, L"bla", L"Processo n�o encontrado.");
        return 1;
    }

    // Abrir o processo alvo com permiss�es de leitura
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, processId);
    if (hProcess == NULL) {
        MsgBox_OK(hwnd, L"bla", L"Processo falha ao abrir");
        return 1;
    }

    // Endere�o de mem�ria alvo
    LPVOID targetAddress = (LPVOID)0xC992F99CB0; // Substitua pelo endere�o real

    // Vari�vel para armazenar o valor lido
    int value = 0;

    // Ler o valor do endere�o de mem�ria alvo
    SIZE_T bytesRead;
    if (ReadProcessMemory(hProcess, targetAddress, &value, sizeof(value), &bytesRead)) {
        //std::cout << "Valor lido: " << value << std::endl;
         
        std::wstring wideStr = std::to_wstring(value);

        // Alocar mem�ria para o LPCWSTR e copiar a string wide para l�
        wchar_t* result = new wchar_t[wideStr.length() + 1];
        wcscpy_s(result, wideStr.length() + 1, wideStr.c_str());
        MsgBox_OK(hwnd, L"bla", result);
    }
    else {
        MsgBox_OK(hwnd, L"bla", L"Processo falha ao ler emroaira");
    }

    // Fechar o handle do processo
    CloseHandle(hProcess);

    return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {

        

        addButton(hwnd, Botoes::Botao_1, L"Botao 1", 10, 10, 80, 25);
        //addListBox(hwnd,Botoes::Botao_1);
        //addItemListBox(hwnd, Botoes::Botao_1,L"a");
        //addItemListBox(hwnd, Botoes::Botao_1,L"b");
        //addItemListBox(hwnd, Botoes::Botao_1,L"c");
        
         
        
        /*
        // Criar os controles quando a janela � criada
        addButton(hwnd, Botoes::Botao_1, L"Botao 1", 10, 10, 80, 25 );
        addCheckBox(hwnd, Botoes::CheckBox,L"Minha checkbox");
        //  addInput(hwnd, Botoes::Input_Text);
         
        addCombobox(hwnd, Botoes::ComboBox );
        addComboboxItens(hwnd, Botoes::ComboBox,  L"Vermelhor");
        addComboboxItens(hwnd, Botoes::ComboBox, L"Azul");
        addComboboxItens(hwnd, Botoes::ComboBox, L"Pinto Murcho");

        addTextarea(hwnd, Botoes::Textarea);

        addProgressBar(hwnd, Botoes::ProgressBar);
        addRadio(hwnd, Botoes::Radio1,L"Meu Radio");
        */
        break;
    }
    case WM_COMMAND: {
        if (CLICOU_NO_BOTAO(hwnd,wParam,Botoes::Botao_1))
        {
             // MsgBox_OK(hwnd,  L"bla", L"Clicou VOID string"); 
            lerMemoriaINT(hwnd);
               
             // g_bRun = false;
             // if (g_hThread) {
              //    WaitForSingleObject(g_hThread, INFINITE);
              //    CloseHandle(g_hThread);
             // }
        }
        else if (CLICOU_NO_BOTAO(hwnd,wParam,Botoes::CheckBox))
        {
            if (IF_CHECKBOX_CHECKED(hwnd,  Botoes::CheckBox))
                MsgBox_OK(hwnd, L"bla", L"CheckBox marcado");
            else
                MsgBox_OK(hwnd, L"bla", L"Checkbox nao marcado");
        } 
        else if (CLICOU_NO_BOTAO(hwnd, wParam, Botoes::Radio1))
        {
            if (IF_RADIO_CHECKED(hwnd, Botoes::Radio1))
                MsgBox_OK(hwnd, L"bla", L"Radio1 marcado");
            else
                MsgBox_OK(hwnd, L"bla", L"Radio1 nao marcado");
        } 
        else if (IF_SELECIONOU_ITEM_LISTBOX(hwnd, wParam, Botoes::Botao_1))
        {
            //MsgBox_OK(hwnd, L"", textoItemSelecionado(hwnd, Botoes::Botao_1));
            LOG("C:/Users/usuario/Documents/GitHub/BresoGUI-DLL/log.txt", LPCWSTRToString(textoItemSelecionado(hwnd, Botoes::Botao_1)));
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

void loop( )
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
    while (g_bRun && GetMessageW(&msg, NULL, 0, 0)) {
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
        g_bRun = false;

        // Espera a thread terminar
        if (g_hThread) {
            WaitForSingleObject(g_hThread, INFINITE);
            CloseHandle(g_hThread);
        }
        break;
    }
    return TRUE;
}

LPCWSTR IntToLPCWSTR(int intValue) {
    // Converter o inteiro para uma string wide
    std::wstring wideStr = std::to_wstring(intValue);

    // Alocar mem�ria para o LPCWSTR e copiar a string wide para l�
    wchar_t* result = new wchar_t[wideStr.length() + 1];
    wcscpy_s(result, wideStr.length() + 1, wideStr.c_str());

    // Retornar o LPCWSTR
    return result;
}

string intToString(int intValue) { 
    char buffer[20]; // Tamanho do buffer deve ser grande o suficiente para a string resultante
    sprintf(buffer, "%d", intValue);
    return buffer;
}
