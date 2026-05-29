/**
 * @author Lucas de Lima Canto
 * @author Samuel Ferreira Ribeiro
 * @author Jose Gabriel de Melo Porto
 * @date Data que está sendo feito a entrega (26/05/2026)
 */

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// ======================================================
// IDs DOS CONTROLES
// ======================================================

#define BTN_SALVAR        1
#define BTN_CARREGAR      2
#define BTN_LEVELUP       3
#define BTN_CALC          4

#define EDIT_NOME         10
#define EDIT_FORCA        11
#define EDIT_DEFESA       12
#define EDIT_MAGIA        13

// ======================================================
// HANDLES GLOBAIS
// ======================================================

HWND hEditNome;
HWND hEditForca;
HWND hEditDefesa;
HWND hEditMagia;

HWND hStaticVida;
HWND hStaticMana;

// ======================================================
// ARQUIVO INI
// ======================================================

const wchar_t *arquivoINI = L"personagem.ini";

// ======================================================
// FUNÇÃO: Atualizar Status
// ======================================================

void atualizarStatus()
{
    wchar_t texto[50];

    GetWindowText(hEditForca, texto, 50);
    int forca = _wtoi(texto);

    GetWindowText(hEditMagia, texto, 50);
    int magia = _wtoi(texto);

    int vida = forca * 10;
    int mana = magia * 15;

    wsprintf(texto, L"Vida: %d", vida);
    SetWindowText(hStaticVida, texto);

    wsprintf(texto, L"Mana: %d", mana);
    SetWindowText(hStaticMana, texto);
}

// ======================================================
// FUNÇÃO: Salvar personagem
// ======================================================

void salvarPersonagem(HWND hwnd)
{
    wchar_t nome[100];
    wchar_t forca[50];
    wchar_t defesa[50];
    wchar_t magia[50];
    wchar_t vida[50];
    wchar_t mana[50];

    GetWindowText(hEditNome, nome, 100);
    GetWindowText(hEditForca, forca, 50);
    GetWindowText(hEditDefesa, defesa, 50);
    GetWindowText(hEditMagia, magia, 50);

    wsprintf(vida, L"%d", _wtoi(forca) * 10);
    wsprintf(mana, L"%d", _wtoi(magia) * 15);

    if (!WritePrivateProfileString(L"Personagem", L"Nome", nome, arquivoINI) ||
        !WritePrivateProfileString(L"Atributos", L"Forca", forca, arquivoINI) ||
        !WritePrivateProfileString(L"Atributos", L"Defesa", defesa, arquivoINI) ||
        !WritePrivateProfileString(L"Atributos", L"Magia", magia, arquivoINI) ||
        !WritePrivateProfileString(L"Status", L"Vida", vida, arquivoINI) ||
        !WritePrivateProfileString(L"Status", L"Mana", mana, arquivoINI))
    {
        wchar_t erro[100];
        wsprintf(erro, L"Erro ao salvar! Codigo: %lu", GetLastError());
        MessageBox(hwnd, erro, L"Erro", MB_OK | MB_ICONERROR);
        return;
    }

    MessageBox(
        hwnd,
        L"Personagem salvo!",
        L"Salvar",
        MB_OK | MB_ICONINFORMATION
    );
}

// ======================================================
// FUNÇÃO: Carregar personagem
// ======================================================

void carregarPersonagem(HWND hwnd)
{
    wchar_t nome[100];
    wchar_t forca[50];
    wchar_t defesa[50];
    wchar_t magia[50];

    if (GetFileAttributes(arquivoINI) == INVALID_FILE_ATTRIBUTES)
    {
        MessageBox(hwnd, L"Erro ao carregar! Arquivo nao encontrado.",
                   L"Erro", MB_OK | MB_ICONERROR);
        return;
    }

    GetPrivateProfileString(L"Personagem", L"Nome", L"Sem Nome", nome, 100, arquivoINI);
    GetPrivateProfileString(L"Atributos", L"Forca", L"0", forca, 50, arquivoINI);
    GetPrivateProfileString(L"Atributos", L"Defesa", L"0", defesa, 50, arquivoINI);
    GetPrivateProfileString(L"Atributos", L"Magia", L"0", magia, 50, arquivoINI);

    SetWindowText(hEditNome, nome);
    SetWindowText(hEditForca, forca);
    SetWindowText(hEditDefesa, defesa);
    SetWindowText(hEditMagia, magia);

    atualizarStatus();

    MessageBox(
        hwnd,
        L"Personagem carregado!",
        L"Carregar",
        MB_OK | MB_ICONINFORMATION
    );
}

// ======================================================
// FUNÇÃO: LEVEL UP
// ======================================================

void levelUp(HWND hwnd)
{
    wchar_t texto[50];

    GetWindowText(hEditForca, texto, 50);
    int forca = _wtoi(texto);

    GetWindowText(hEditDefesa, texto, 50);
    int defesa = _wtoi(texto);

    GetWindowText(hEditMagia, texto, 50);
    int magia = _wtoi(texto);

    forca += 2;
    defesa += 1;
    magia += 3;

    wsprintf(texto, L"%d", forca);
    SetWindowText(hEditForca, texto);

    wsprintf(texto, L"%d", defesa);
    SetWindowText(hEditDefesa, texto);

    wsprintf(texto, L"%d", magia);
    SetWindowText(hEditMagia, texto);

    atualizarStatus();

    MessageBox(
        hwnd,
        L"LEVEL UP!",
        L"RPG",
        MB_OK | MB_ICONINFORMATION
    );
}

// ======================================================
// FUNÇÃO: Abrir Calculadora
// ======================================================

void abrirCalculadora(HWND hwnd)
{
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};

    si.cb = sizeof(si);

    wchar_t cmd[] = L"calc.exe";

    if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        wchar_t erro[100];
        wsprintf(erro, L"Erro ao abrir processo! Codigo: %lu", GetLastError());
        MessageBox(hwnd, erro, L"Erro", MB_OK | MB_ICONERROR);
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

// ======================================================
// CALLBACK DA JANELA
// ======================================================

LRESULT CALLBACK WindowProc(HWND hwnd,
                            UINT uMsg,
                            WPARAM wParam,
                            LPARAM lParam)
{
    switch(uMsg)
    {

    // ==================================================
    // CRIAÇÃO DA INTERFACE
    // ==================================================

    case WM_CREATE:

        CreateWindow(L"STATIC", L"Nome:", WS_VISIBLE | WS_CHILD,
                     20, 20, 100, 20, hwnd, NULL, NULL, NULL);

        hEditNome = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
                     120, 20, 200, 25, hwnd, (HMENU)EDIT_NOME, NULL, NULL);

        CreateWindow(L"STATIC", L"Forca:", WS_VISIBLE | WS_CHILD,
                     20, 60, 100, 20, hwnd, NULL, NULL, NULL);

        hEditForca = CreateWindow(L"EDIT", L"10", WS_VISIBLE | WS_CHILD | WS_BORDER,
                     120, 60, 100, 25, hwnd, (HMENU)EDIT_FORCA, NULL, NULL);

        CreateWindow(L"STATIC", L"Defesa:", WS_VISIBLE | WS_CHILD,
                     20, 100, 100, 20, hwnd, NULL, NULL, NULL);

        hEditDefesa = CreateWindow(L"EDIT", L"8", WS_VISIBLE | WS_CHILD | WS_BORDER,
                     120, 100, 100, 25, hwnd, (HMENU)EDIT_DEFESA, NULL, NULL);

        CreateWindow(L"STATIC", L"Magia:", WS_VISIBLE | WS_CHILD,
                     20, 140, 100, 20, hwnd, NULL, NULL, NULL);

        hEditMagia = CreateWindow(L"EDIT", L"3", WS_VISIBLE | WS_CHILD | WS_BORDER,
                     120, 140, 100, 25, hwnd, (HMENU)EDIT_MAGIA, NULL, NULL);

        hStaticVida = CreateWindow(L"STATIC", L"Vida: 0", WS_VISIBLE | WS_CHILD,
                     20, 190, 250, 25, hwnd, NULL, NULL, NULL);

        hStaticMana = CreateWindow(L"STATIC", L"Mana: 0", WS_VISIBLE | WS_CHILD,
                     20, 220, 250, 25, hwnd, NULL, NULL, NULL);

        CreateWindow(L"BUTTON", L"Salvar", WS_VISIBLE | WS_CHILD,
                     20, 280, 100, 35, hwnd, (HMENU)BTN_SALVAR, NULL, NULL);

        CreateWindow(L"BUTTON", L"Carregar", WS_VISIBLE | WS_CHILD,
                     130, 280, 100, 35, hwnd, (HMENU)BTN_CARREGAR, NULL, NULL);

        CreateWindow(L"BUTTON", L"Level Up", WS_VISIBLE | WS_CHILD,
                     240, 280, 100, 35, hwnd, (HMENU)BTN_LEVELUP, NULL, NULL);

        CreateWindow(L"BUTTON", L"Calculadora", WS_VISIBLE | WS_CHILD,
                     350, 280, 150, 35, hwnd, (HMENU)BTN_CALC, NULL, NULL);

        atualizarStatus();

        return 0;

    // ==================================================
    // EVENTOS DOS BOTÕES
    // ==================================================

    case WM_COMMAND:

        switch(LOWORD(wParam))
        {
            case BTN_SALVAR:
                salvarPersonagem(hwnd);
                break;

            case BTN_CARREGAR:
                carregarPersonagem(hwnd);
                break;

            case BTN_LEVELUP:
                levelUp(hwnd);
                break;

            case BTN_CALC:
                abrirCalculadora(hwnd);
                break;
        }

        return 0;

    // ==================================================
    // FECHAR JANELA
    // ==================================================

    case WM_DESTROY:

        PostQuitMessage(0);

        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ======================================================
// FUNÇÃO PRINCIPAL
// ======================================================

int WINAPI wWinMain(HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    PWSTR pCmdLine,
                    int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"RPGManager";

    WNDCLASS wc = {0};

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"RPG Character Manager",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        560,
        420,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}