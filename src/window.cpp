//
// Created by FryFly55 on 14.06.2023.
//

#include "window.h"
#include "renderer.h"

namespace Window {
    int m_screenWidth;
    int m_screenHeight;
    int m_fontWidth;
    int m_fontHeight;

    HANDLE m_hConsole;
    HANDLE m_hConsoleIn;
    HANDLE m_hOriginalConsole;
    SMALL_RECT m_rectWindow;
    CHAR_INFO* m_screenBuffer;
    COORD m_coord;

    int Window::Window(int width, int height, int fontWidth, int fontHeight) {
        Window::m_fontHeight = fontHeight;
        Window::m_fontWidth = fontWidth;
        Window::m_screenHeight = height;
        Window::m_screenWidth = width;

        Window::m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        Window::m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
        Window::m_hOriginalConsole = GetStdHandle(STD_ERROR_HANDLE);

        return 0;
    }

    int Window::createWindow() {
        // Set Console output to unicode
        SetConsoleCP(UNICODE);

        // check if handle is valid
        if (m_hConsole == INVALID_HANDLE_VALUE) {
            Window::Error(L"Invalid Handle!");
            return 1;
        }
        if (m_hOriginalConsole == INVALID_HANDLE_VALUE) {
            Window::Error(L"Invalid Original Handle!");
            return 1;
        }

        // set visible console size to a minimum to ease working with the screen buffer.
        m_rectWindow = {0, 0, 1, 1};
        SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow);

        // set size of the screen buffer afterwards
        m_coord = {(short) m_screenWidth, (short) m_screenHeight};
        if (!SetConsoleScreenBufferSize(m_hConsole, m_coord)) {
            Window::Error(L"SetConsoleActiveScreenBuffer: setting screen buffer size failed");
            return 2;
        }
        // Now assign screen buffer to the console
        if (!SetConsoleActiveScreenBuffer(m_hConsole)) {
            Window::Error(L"SetConsoleActiveScreenBuffer: assigning screen buffer failed");
            return 3;
        }

        // Now set font size
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = m_fontWidth;
        cfi.dwFontSize.Y = m_fontHeight;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Consolas");
        if (!SetCurrentConsoleFontEx(m_hConsole, false, &cfi)) {
            Window::Error(L"SetCurrentConsoleFontEx");
            return 4;
        }


        // get buffer info and check that window size is within allowed bounds
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(m_hConsole, &bufferInfo)) {
            Window::Error(L"GetConsoleScreenBufferInfo: failed");
            return 5;
        }
        if (m_screenHeight > bufferInfo.dwMaximumWindowSize.Y) {
            Window::Error(L"[int fontHeight] to big");
            return 6;
        }
        if (m_screenWidth > bufferInfo.dwMaximumWindowSize.X) {
            Window::Error(L"[int fontWidth] to big");
            return 7;
        }

        // Now set console window size to be bigger again, after changing the buffer.
        // Size now depends on fontSize and resolution/size of the buffer
        m_rectWindow = {0, 0, static_cast<SHORT>((short) m_screenWidth - 1),
                        static_cast<SHORT>((short) m_screenHeight - 1)};
        if (!SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow)) {
            Window::Error(L"SetConsoleWindowInfo, failed after changing window size according to buffer");
            return 8;
        }

        // allow mouse inputs
        if (!SetConsoleMode(m_hConsoleIn,
                            ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) {
            Window::Error(L"SetConsoleMode: allowing mouse input failed");
            return 9;
        }

        // Allocate memory for screen buffer
        m_screenBuffer = new CHAR_INFO[m_screenWidth * m_screenHeight];


        SetConsoleCtrlHandler(nullptr, FALSE);

        return 0;
    }

    int Window::updateWindow() {
        // modify screenbuffer here
        Renderer::rasterize();
        // Refresh screenbuffer
        SetConsoleActiveScreenBuffer(m_hConsole);
        PSMALL_RECT pRect = &m_rectWindow;
        const _CHAR_INFO* copyBuffer = m_screenBuffer;
        WriteConsoleOutputW(m_hConsole, copyBuffer, m_coord, {0, 0},
                            pRect);
        // clear it after we're done
        std::memset(m_screenBuffer, 0, sizeof(CHAR_INFO) * m_screenWidth * m_screenHeight);

        return 0;
    }

    int Window::Error(const wchar_t* msg)
    {
        wchar_t buf[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      buf, 256, NULL);
        SetConsoleActiveScreenBuffer(m_hOriginalConsole);
        wprintf(L"ERROR: %s\n\t%s\n", msg, buf);
        return 0;
    }
}