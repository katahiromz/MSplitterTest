#include "MSplitterWnd.hpp"
#include "MTabCtrl.hpp"

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
    MTabCtrl m_tab;

    BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
    {
        m_cxy1 = 100;
        m_cxy2 = 100;

        DWORD style, exstyle;

        style = WS_CHILD | WS_VISIBLE | SWS_HORZ | SWS_LEFTALIGN;
        if (!m_splitter1.CreateDx(hwnd, 2, style))
            return FALSE;

        style = WS_CHILD | WS_VISIBLE | WS_BORDER | TCS_BOTTOM | TCS_TABS | TCS_TOOLTIPS |
                TCS_FOCUSNEVER | TCS_HOTTRACK | TCS_MULTILINE;
        if (!m_tab.CreateWindowDx(m_splitter1, NULL, style))
            return FALSE;
        SetWindowFont(m_tab, GetStockFont(DEFAULT_GUI_FONT), TRUE);

        style = WS_CHILD | WS_VISIBLE | SWS_HORZ | SWS_RIGHTALIGN;
        if (!m_splitter2.CreateDx(m_splitter1, 2, style))
            return FALSE;

        m_tab.InsertItem(0, TEXT("Code Editor"));
        m_tab.InsertItem(1, TEXT("Hex Viewer"));
        m_tab.SetCurSel(0);

        style = WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL;
        exstyle = WS_EX_CLIENTEDGE;
        m_hEdit1 = CreateWindowEx(exstyle, TEXT("EDIT"), TEXT("m_hEdit1"), style, 0, 0, 0, 0,
            m_splitter1, NULL, GetModuleHandle(NULL), NULL);
        m_hEdit2 = CreateWindowEx(exstyle, TEXT("EDIT"), TEXT("m_hEdit2"), style, 0, 0, 0, 0,
            m_splitter2, NULL, GetModuleHandle(NULL), NULL);
        m_hEdit3 = CreateWindowEx(exstyle, TEXT("EDIT"), TEXT("m_hEdit3"), style, 0, 0, 0, 0,
            m_splitter2, NULL, GetModuleHandle(NULL), NULL);
        if (!m_hEdit1 || !m_hEdit2 || !m_hEdit3)
            return FALSE;

        m_splitter1.SetPane(0, m_hEdit1);
        m_splitter1.SetPane(1, m_tab);
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
        SIZE siz;

        GetClientRect(hwnd, &rc);
        siz = SizeFromRectDx(&rc);
        MoveWindow(m_splitter1, rc.left, rc.top, siz.cx, siz.cy, TRUE);

        GetClientRect(m_tab, &rc);
        m_tab.AdjustRect(FALSE, &rc);
        MapWindowRect(m_tab, GetParent(m_splitter2), &rc);
        siz = SizeFromRectDx(&rc);
        MoveWindow(m_splitter2, rc.left, rc.top, siz.cx, siz.cy, TRUE);
    }

    void OnSysColorChange(HWND hwnd)
    {
        m_splitter1.SendMessageDx(WM_SYSCOLORCHANGE);
    }

    LRESULT OnNotify(HWND hwnd, int idFrom, LPNMHDR pnmhdr)
    {
        if (pnmhdr->hwndFrom == m_splitter1)
        {
            if (pnmhdr->code == MSplitterWnd::NOTIFY_CHANGED)
            {
                m_cxy1 = m_splitter1.GetPaneExtent(0);
                PostMessage(hwnd, WM_SIZE, 0, 0);
            }
        }
        else if (pnmhdr->hwndFrom == m_splitter2)
        {
            if (pnmhdr->code == MSplitterWnd::NOTIFY_CHANGED)
            {
                m_cxy2 = m_splitter2.GetPaneExtent(1);
            }
        }
        else if (pnmhdr->hwndFrom == m_tab)
        {
            if (pnmhdr->code == TCN_SELCHANGE)
            {
                // TODO:
            }
        }
        return 0;
    }

    void OnDestroy(HWND hwnd)
    {
        DestroyWindow(m_hEdit1);
        DestroyWindow(m_hEdit2);
        DestroyWindow(m_hEdit3);
        DestroyWindow(m_splitter2);
        DestroyWindow(m_tab);
        DestroyWindow(m_splitter1);
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
    if (!mainWnd.CreateWindowDx(NULL, TEXT("MSplitterTest")))
    {
        MessageBox(NULL, TEXT("CreateWindowDx failed"), NULL, MB_ICONERROR);
        return -1;
    }
    ShowWindow(mainWnd, nCmdShow);
    UpdateWindow(mainWnd);
    return mainWnd.Run();
}
