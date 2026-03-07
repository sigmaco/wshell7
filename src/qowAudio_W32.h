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

#ifndef QOW_AUDIO_H
#define QOW_AUDIO_H

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qowBase.h"
#define _AMX_MIX_C
#define _AUX_DISPLAY_C
#define _AUX_DISPLAY_IMPL
#define _AMX_SINK_C
#define _AMX_SINK_IMPL
#define _AMX_AUDIO_C
//#define _AMX_BUFFER_IMPL
#include "../qwadro_afx/coree/mix/amxIcd.h"
#include "../../icd_amiga/src/zalBase.h"
#include "../../icd_amiga/src/zalInteropWasapi.h"
#define _AUX_UX_C
#include "../qwadro_afx/coree/ux/auxIcd.h"

AFX_OBJECT(afxSink)
{
    AFX_OBJECT(_amxSink) m;
    union
    {
        zalWasapi wasapi;
        struct
        {
            ma_device_config deviceConfig;
            ma_device device;
            ma_pcm_rb rb;
        } miniaud;
    } idd;
};


QOW afxError _QowSinkCtorCb(afxSink asi, void** args, afxUnit invokeNo);
QOW afxError _QowSinkDtorCb(afxSink asi);

#endif//QOW_AUDIO_H
