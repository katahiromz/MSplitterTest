#include "MSplitterWnd.hpp"

class MSplitterTest : public MWindowBase
{
public:
    MSplitterTest()
    {
    }

    virtual ~MSplitterTest()
    {
    }

    virtual LPCTSTR GetWndClassNameDx() const
    {
        return TEXT("MZC4 MSplitterTest Class");
    }

    virtual VOID ModifyWndClassDx(WNDCLASSEX& wcx)
    {
    }

    virtual LRESULT CALLBACK
    WindowProcDx(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hwnd, WM_SIZE, OnSize);
        HANDLE_MSG(hwnd, WM_SYSCOLORCHANGE, OnSysColorChange);
        HANDLE_MSG(hwnd, WM_NOTIFY, OnNotify);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        default:
            return DefaultProcDx();
        }
    }

    INT Run()
    {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return INT(msg.wParam);
    }

protected:
    LONG m_cxy1;
    LONG m_cxy2;
    HWND m_hEdit1;
    HWND m_hEdit2;
    HWND m_hEdit3;
    MSplitterWnd m_splitter1;
    MSplitterWnd m_splitter2;

    BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
    {
        m_cxy1 = 100;
        m_cxy2 = 100;

        DWORD style, exstyle;

        style = WS_CHILD | WS_VISIBLE | SWS_HORZ | SWS_LEFTALIGN;
        if (!m_splitter1.CreateDx(hwnd, 2, style))
            return FALSE;
        style = WS_CHILD | WS_VISIBLE | SWS_VERT | SWS_BOTTOMALIGN;
        if (!m_splitter2.CreateDx(m_splitter1, 2, style))
            return FALSE;

        style = WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL;
        exstyle = WS_EX_CLIENTEDGE;
        m_hEdit1 = CreateWindowEx(exstyle, TEXT("EDIT"), NULL, style, 0, 0, 0, 0,
            m_splitter1, NULL, GetModuleHandle(NULL), NULL);
        m_hEdit2 = CreateWindowEx(exstyle, TEXT("EDIT"), NULL, style, 0, 0, 0, 0,
            m_splitter2, NULL, GetModuleHandle(NULL), NULL);
        m_hEdit3 = CreateWindowEx(exstyle, TEXT("EDIT"), NULL, style, 0, 0, 0, 0,
            m_splitter2, NULL, GetModuleHandle(NULL), NULL);
        if (!m_hEdit1 || !m_hEdit2 || !m_hEdit3)
            return FALSE;

        m_splitter1.SetPane(0, m_hEdit1);
        m_splitter1.SetPane(1, m_splitter2);
        m_splitter1.SetPaneExtent(0, m_cxy1);

        m_splitter2.SetPane(0, m_hEdit2);
        m_splitter2.SetPane(1, m_hEdit3);
        m_splitter2.SetPaneExtent(1, m_cxy2);

        PostMessage(hwnd, WM_SIZE, 0, 0);
        return TRUE;
    }

    void OnSize(HWND hwnd, UINT state, int cx, int cy)
    {
        RECT rc;
        GetClientRect(hwnd, &rc);
        SIZE siz = SizeFromRectDx(&rc);
        MoveWindow(m_splitter1, rc.left, rc.top, siz.cx, siz.cy, TRUE);
    }

    void OnSysColorChange(HWND hwnd)
    {
        m_splitter1.SendMessageDx(WM_SYSCOLORCHANGE);
    }

    LRESULT OnNotify(HWND hwnd, int idFrom, LPNMHDR pnmhdr)
    {
        if (pnmhdr->hwndFrom == m_splitter1)
        {
            if (m_splitter1.GetPaneCount() >= 1)
            {
                m_cxy1 = m_splitter1.GetPaneExtent(0);
            }
        }
        else if (pnmhdr->hwndFrom == m_splitter2)
        {
            if (m_splitter2.GetPaneCount() >= 1)
            {
                m_cxy2 = m_splitter2.GetPaneExtent(0);
            }
        }
        return 0;
    }

    void OnDestroy(HWND hwnd)
    {
        DestroyWindow(m_splitter1);
        DestroyWindow(m_splitter2);
        DestroyWindow(m_hEdit1);
        DestroyWindow(m_hEdit2);
        DestroyWindow(m_hEdit3);
        PostQuitMessage(0);
    }
};

INT WINAPI
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpCmdLine,
        INT         nCmdShow)
{
    InitCommonControls();
    MSplitterTest mainWnd;
    if (!mainWnd.CreateWindowDx(NULL, NULL))
    {
        MessageBox(NULL, TEXT("CreateWindowDx failed"), NULL, MB_ICONERROR);
        return -1;
    }
    ShowWindow(mainWnd, nCmdShow);
    UpdateWindow(mainWnd);
    return mainWnd.Run();
}
