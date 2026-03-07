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

#define _AFX_SYSTEM_C
#define _AUX_UX_C
#define _AUX_SHELL_C
#define _AUX_SHELL_IMPL
#define _AUX_WINDOW_C
#define _AUX_WINDOW_IMPL
//#include "qwadro/../_luna/luna_vm.h"
//#include "qwadro/../_luna/luna.h"
#include "qowBase.h"
#include "qowVideo_W32.h"
#include "qowAudio_W32.h"
#include "../qwadro_afx/coree/draw/avxSurfaceDDK.h"
//#include "../qwadro_afx/src/mix/ddi/avxImpl_Sink.h"

QOW afxError _QowDoutDtorCb_GDI(afxSurface dout);
QOW afxError _QowDoutCtorCb_GDI(afxSurface dout, void** args, afxUnit invokeNo);

QOW afxError _ZglDoutCtorCb(afxSurface dout, void** args, afxUnit invokeNo);
QOW afxError _ZglDoutDtorCb(afxSurface dout);

afxError getInteropDoutCls(afxDrawSystem dsys, afxString const* tool, afxClassConfig* cfg)
{
    afxError err = { 0 };

    if (0 == AfxCompareString(tool, 0, "wgl", 0, FALSE))
    {
        afxClassConfig doutClsCfg = _AVX_CLASS_CONFIG_DOUT;
        doutClsCfg.fixedSiz = sizeof(AFX_OBJ(afxSurface));
        doutClsCfg.ctor = (void*)_ZglDoutCtorCb;
        doutClsCfg.dtor = (void*)_ZglDoutDtorCb;
        *cfg = doutClsCfg;
    }
    else if (   (0 == AfxCompareString(tool, 0, "w32", 0, FALSE)) || 
                (0 == AfxCompareString(tool, 0, "gdi", 0, FALSE)) ||
                (0 == AfxCompareString(tool, 0, "soft", 0, FALSE)) ||
                (0 == AfxCompareString(tool, 0, "sw", 0, FALSE)) ||
                (0 == AfxCompareString(tool, 0, "", 0, FALSE)))
    {
        afxClassConfig doutClsCfg = _AVX_CLASS_CONFIG_DOUT;
        doutClsCfg.fixedSiz = sizeof(AFX_OBJ(afxSurface));
        doutClsCfg.ctor = (void*)_QowDoutCtorCb_GDI;
        doutClsCfg.dtor = (void*)_QowDoutDtorCb_GDI;
        *cfg = doutClsCfg;
    }
    else
    {
        *cfg = (afxClassConfig) { 0 };
        err = afxError_UNSUPPORTED;
    }

    return err;
}

afxError getInteropSinkCls(afxMixSystem msys, afxString const* tool, afxClassConfig* cfg)
{
    afxError err = { 0 };

    if ((0 == AfxCompareString(tool, 0, "w32", 0, FALSE)) ||
        (0 == AfxCompareString(tool, 0, "wasapi", 0, FALSE)) ||
        (0 == AfxCompareString(tool, 0, "soft", 0, FALSE)) ||
        (0 == AfxCompareString(tool, 0, "sw", 0, FALSE)) ||
        (0 == AfxCompareString(tool, 0, "", 0, FALSE)))
    {
        afxClassConfig sinkClsCfg = _AMX_ASIO_CLASS_CONFIG;
        sinkClsCfg.fixedSiz = sizeof(AFX_OBJ(afxSink));
        sinkClsCfg.ctor = (void*)_QowSinkCtorCb;
        sinkClsCfg.dtor = (void*)_QowSinkDtorCb;
        *cfg = sinkClsCfg;
    }
    else
    {
        *cfg = (afxClassConfig) { 0 };
        err = afxError_UNSUPPORTED;
    }
    return err;
}

_QOW afxError afxIcdHook(afxModule icd, afxUri const* manifest)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);

    afxClassConfig envClsCfg = _AUX_ENV_CLASS_CONFIG;
    envClsCfg.fixedSiz = sizeof(AFX_OBJ(afxEnvironment));
    envClsCfg.ctor = (void*)_QowEnvCtorCb;
    envClsCfg.dtor = (void*)_QowEnvDtorCb;

    afxClassConfig dpyClsCfg = _AUX_DPY_CLASS_CONFIG;
    dpyClsCfg.fixedSiz = sizeof(AFX_OBJ(afxDisplay));
    dpyClsCfg.ctor = (void*)_QowDpyCtorCb;
    dpyClsCfg.dtor = (void*)_QowDpyDtorCb;

    _auxImplementation impl = { 0 };
    impl.dpyCls = dpyClsCfg;
    impl.envCls = envClsCfg;
    impl.getInteropDoutCls = getInteropDoutCls;
    impl.getInteropSinkCls = getInteropSinkCls;

    _AuxIcdImplement(icd, &impl);

    RegisterPresentVdus(icd);

    return err;
}
