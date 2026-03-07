/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // Qwadro on Windows                                                        //
//////////////////////////////////////////////////////////////////////////////

#ifndef QOW_VIDEO_H
#define QOW_VIDEO_H

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <hidusage.h>

#include <intrin.h>
#include <d3d9.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <dxva2api.h>
#include <winddi.h>
#include <d3d11.h>
#include <d3d9.h>
#include <dxgi1_2.h>

#define _AUX_UX_C
#define _AVX_DRAW_C
#define _AUX_DISPLAY_C
#define _AUX_DISPLAY_IMPL
#define _AVX_SURFACE_C
#define _AVX_SURFACE_IMPL
#include "../qwadro_afx/coree/ux/auxIcd.h"
#include "../qwadro_afx/coree/draw/avxIcd.h"
#include "../../icd_tarzgl4/src/zglDefs.h"

#define USE_DXGI_DISPLAY 1
#define USE_GDI_DISPLAY 1

AFX_OBJECT(afxDisplayPort)
{
    AFX_OBJECT(_auxDisplayPort) m;
#if USE_DXGI_DISPLAY
    IDXGIOutput* pDxgiOutput;
    UINT dxgiOutputIndex; // Should be the same as Qwadro's display port ordinal number.
    DXGI_OUTPUT_DESC dxgiDesc;
#endif
#if USE_GDI_DISPLAY
    HMONITOR hMon;
    DISPLAY_DEVICEA ddminfo;
#endif
};

typedef enum qowDisplayType
{
    qowDisplayType_DXGI,
    qowDisplayType_GDI
} qowDisplayType;

AFX_OBJECT(afxDisplay)
{
    AFX_OBJECT(_auxDisplay) m;

    qowDisplayType type;
#if USE_DXGI_DISPLAY
    IDXGIFactory* pDxgiFactory; // Could be moved to environment to avoid span and improve resource utilization.
    IDXGIAdapter* pDxgiAdapter;
    UINT dxgiAdapterIndex;
#endif

#if USE_GDI_DISPLAY
    DISPLAY_DEVICEA ddinfo;
#endif
};

AFX_OBJECT(afxSurface)
{
    AFX_OBJECT(_avxSurface) m;

    HINSTANCE               hInst;
    HWND                    hWnd;
    afxBool                 isWpp;
    HDC                     hDC;
    int                     dcPixFmt;
    PIXELFORMATDESCRIPTOR   dcPfd;
    union
    {
        struct
        {
            struct
            {
                HDC memDC; // use just one?
                HGDIOBJ oldGdiObjBkp; // required to deselect the bitmap; we can not pass NULL.
                HBITMAP hBitmap;
                void* bytemap;
            } *swaps;
        } gdi;
        struct
        {
            HGLRC       hSwapRC; // only swaps it. This is needed because hGLRC are strictly bound to a pixel format.
            HGLRC       hPrimeRC;
            int         glVerMaj;
            int         glVerMin;
            afxBool     robustCtx;
            glVmt const*gl;
            struct
            {
                GLuint  swapFbo;
                afxBool8 swapFboReady;
            } *swaps;
            afxBool swapOnWgl;
            void(*WINAPI AddSwapHintRectWIN)(GLint x, GLint y, GLsizei width, GLsizei height);
        } wgl;
        struct
        {
            ID3D11Device*   d3d11Dev;
            HANDLE          d3d11DevForGl;
            DXGI_FORMAT mSwapChainFormat;
            UINT mSwapChainFlags;
            afxBool mFirstSwap;
            IDXGISwapChain *mSwapChain;
            IDXGISwapChain1 *mSwapChain1;
            GLuint mFramebufferID;
            GLuint mColorRenderbufferID;
            HANDLE mRenderbufferBufferHandle;
            GLuint mTextureID;
            HANDLE mTextureHandle;
            afxInt mSwapInterval;
            afxInt mOrientation;
        } dxgi;
        struct
        {
            IDirect3D9Ex*       d3d9Ex;
            IDirect3DDevice9Ex* d3d9DevEx;
            HANDLE              d3d9DevExForGl;
            IDirect3DSwapChain9*d3d9Sw;
            IDirect3DTexture9* sharedTexture;
            IDirect3DSurface9* pSurface;
        } d3dsw9;
    };
};

QOW afxResult _ZglVduIoctrlCb(afxDisplay vdu, afxUnit reqCode, va_list va);

BOOL GetMonitorNameFromHMONITOR(HMONITOR hMonitor, char* outName, size_t outNameSize);
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

afxError RegisterPresentVdus(afxModule icd);

QOW afxError _QowDpyDtorCb(afxDisplay dpy);
QOW afxError _QowDpyCtorCb(afxDisplay dpy, void** args, afxUnit invokeNo);

#endif//QOW_VIDEO_H
