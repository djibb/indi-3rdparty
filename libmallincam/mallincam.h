﻿#ifndef __mallincam_h__
#define __mallincam_h__

/* Version: 59.28981.20250715 */
/*
   Platform & Architecture:
       (1) Win32:
              (a) x64: Win7 or above
              (b) x86: XP SP3 or above; CPU supports SSE2 instruction set or above
              (c) arm64: Win10 or above
              (d) arm: Win10 or above
       (2) WinRT: x64, x86, arm64, arm; Win10 or above
       (3) macOS: x64+arm64: macOS 11.0 or above, support x64 and Apple silicon (such as M1, M2, etc)
       (4) Linux: kernel 2.6.27 or above
              (a) x64: GLIBC 2.14 or above
              (b) x86: CPU supports SSE3 instruction set or above; GLIBC 2.8 or above
              (c) arm64: GLIBC 2.17 or above; built by toolchain aarch64-linux-gnu (version 5.4.0)
              (d) armhf: GLIBC 2.8 or above; built by toolchain arm-linux-gnueabihf (version 5.4.0)
              (e) armel: GLIBC 2.8 or above; built by toolchain arm-linux-gnueabi (version 5.4.0)
       (5) Android: __ANDROID_API__ >= 24 (Android 7.0); built by android-ndk-r18b; see https://developer.android.com/ndk/guides/abis
              (a) arm64: arm64-v8a
              (b) arm: armeabi-v7a
              (c) x64: x86_64
              (d) x86
*/
/*
    doc:
       (1) en.html, English
       (2) hans.html, Simplified Chinese
*/

/*
    Please distinguish between camera ID (camId) and camera SN:
        (a) SN is unique and persistent, fixed inside the camera and remains unchanged, and does not change with connection or system restart.
        (b) Camera ID (camId) may change due to connection or system restart. Enumerate the cameras to get the camera ID, and then call the Open function to pass in the camId parameter to open the camera.
*/

#if defined(_WIN32)
#ifndef _INC_WINDOWS
#include <windows.h>
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__cplusplus) && (__cplusplus >= 201402L)
#define MALLINCAM_DEPRECATED  [[deprecated]]
#elif defined(_MSC_VER)
#define MALLINCAM_DEPRECATED  __declspec(deprecated)
#elif defined(__GNUC__) || defined(__clang__)
#define MALLINCAM_DEPRECATED  __attribute__((deprecated))
#else
#define MALLINCAM_DEPRECATED
#endif

#if defined(_WIN32) /* Windows */
#pragma pack(push, 8)
#ifdef MALLINCAM_EXPORTS
#define MALLINCAM_API(x)    __declspec(dllexport)   x   __stdcall  /* in Windows, we use __stdcall calling convention, see https://docs.microsoft.com/en-us/cpp/cpp/stdcall */
#define MALLINCAM_APIV(x)   __declspec(dllexport)   x   __cdecl
#elif !defined(MALLINCAM_NOIMPORTS)
#define MALLINCAM_API(x)    __declspec(dllimport)   x   __stdcall
#define MALLINCAM_APIV(x)   __declspec(dllimport)   x   __cdecl
#else
#define MALLINCAM_API(x)    x   __stdcall
#define MALLINCAM_APIV(x)   x   __cdecl
#endif
#else   /* Linux or macOS */
#define MALLINCAM_API(x)    x
#define MALLINCAM_APIV(x)   x
#if (!defined(HRESULT)) && (!defined(__COREFOUNDATION_CFPLUGINCOM__)) /* CFPlugInCOM.h */
#define HRESULT int
#endif
#ifndef SUCCEEDED
#define SUCCEEDED(hr)   (((HRESULT)(hr)) >= 0)
#endif
#ifndef FAILED
#define FAILED(hr)      (((HRESULT)(hr)) < 0)
#endif
#ifndef __stdcall
#define __stdcall
#endif
#endif

#ifndef TDIBWIDTHBYTES
#define TDIBWIDTHBYTES(bits)  ((unsigned)(((bits) + 31) & (~31)) / 8)
#endif

/********************************************************************************/
/* HRESULT: error code                                                          */
/* Please note that the return value >= 0 means success                         */
/* (especially S_FALSE is also successful, indicating that the internal value and the value set by the user is equivalent, which means "no operation"). */
/* Therefore, the SUCCEEDED and FAILED macros should generally be used to determine whether the return value is successful or failed. */
/* (Unless there are special needs, do not use "==S_OK" or "==0" to judge the return value) */
/*                                                                              */
/* #define SUCCEEDED(hr)   (((HRESULT)(hr)) >= 0)                               */
/* #define FAILED(hr)      (((HRESULT)(hr)) < 0)                                */
/*                                                                              */
/********************************************************************************/
#if defined(MALLINCAM_HRESULT_ERRORCODE_NEEDED)
#define S_OK                (HRESULT)(0x00000000) /* Success */
#define S_FALSE             (HRESULT)(0x00000001) /* Yet another success */ /* Remark: Different from S_OK, such as internal values and user-set values have coincided, equivalent to noop */
#define E_UNEXPECTED        (HRESULT)(0x8000ffff) /* Catastrophic failure */ /* Remark: Generally indicates that the conditions are not met, such as calling put_Option setting some options that do not support modification when the camera is running, and so on */
#define E_NOTIMPL           (HRESULT)(0x80004001) /* Not supported or not implemented */ /* Remark: This feature is not supported on this model of camera */
#define E_NOINTERFACE       (HRESULT)(0x80004002)
#define E_ACCESSDENIED      (HRESULT)(0x80070005) /* Permission denied */ /* Remark: The program on Linux does not have permission to open the USB device, please enable udev rules file or run as root */
#define E_OUTOFMEMORY       (HRESULT)(0x8007000e) /* Out of memory */
#define E_INVALIDARG        (HRESULT)(0x80070057) /* One or more arguments are not valid */
#define E_POINTER           (HRESULT)(0x80004003) /* Pointer that is not valid */ /* Remark: Pointer is NULL */
#define E_FAIL              (HRESULT)(0x80004005) /* Generic failure */
#define E_WRONG_THREAD      (HRESULT)(0x8001010e) /* Call function in the wrong thread */
#define E_GEN_FAILURE       (HRESULT)(0x8007001f) /* Device not functioning */ /* Remark: It is generally caused by hardware errors, such as cable problems, USB port problems, poor contact, camera hardware damage, etc */
#define E_BUSY              (HRESULT)(0x800700aa) /* The requested resource is in use */ /* Remark: The camera is already in use, such as duplicated opening/starting the camera, or being used by other application, etc */
#define E_PENDING           (HRESULT)(0x8000000a) /* The data necessary to complete this operation is not yet available */ /* Remark: No data is available at this time */
#define E_TIMEOUT           (HRESULT)(0x8001011f) /* This operation returned because the timeout period expired */
#define E_UNREACH           (HRESULT)(0x80072743) /* Network is unreachable */
#endif

/* handle */
typedef struct Mallincam_t { int unused; } *HMallincam;

#define MALLINCAM_MAX                       128
                                         
#define MALLINCAM_FLAG_CMOS                 0x00000001  /* cmos sensor */
#define MALLINCAM_FLAG_CCD_PROGRESSIVE      0x00000002  /* progressive ccd sensor */
#define MALLINCAM_FLAG_CCD_INTERLACED       0x00000004  /* interlaced ccd sensor */
#define MALLINCAM_FLAG_ROI_HARDWARE         0x00000008  /* support hardware ROI */
#define MALLINCAM_FLAG_MONO                 0x00000010  /* monochromatic */
#define MALLINCAM_FLAG_BINSKIP_SUPPORTED    0x00000020  /* support bin/skip mode, see Mallincam_put_Mode and Mallincam_get_Mode */
#define MALLINCAM_FLAG_USB30                0x00000040  /* usb3.0 */
#define MALLINCAM_FLAG_TEC                  0x00000080  /* Thermoelectric Cooler */
#define MALLINCAM_FLAG_USB30_OVER_USB20     0x00000100  /* usb3.0 camera connected to usb2.0 port */
#define MALLINCAM_FLAG_ST4                  0x00000200  /* ST4 port */
#define MALLINCAM_FLAG_GETTEMPERATURE       0x00000400  /* support to get the temperature of the sensor */
#define MALLINCAM_FLAG_HIGH_FULLWELL        0x00000800  /* high fullwell capacity */
#define MALLINCAM_FLAG_RAW10                0x00001000  /* pixel format, RAW 10bits */
#define MALLINCAM_FLAG_RAW12                0x00002000  /* pixel format, RAW 12bits */
#define MALLINCAM_FLAG_RAW14                0x00004000  /* pixel format, RAW 14bits */
#define MALLINCAM_FLAG_RAW16                0x00008000  /* pixel format, RAW 16bits */
#define MALLINCAM_FLAG_FAN                  0x00010000  /* cooling fan */
#define MALLINCAM_FLAG_TEC_ONOFF            0x00020000  /* Thermoelectric Cooler can be turn on or off, support to set the target temperature of TEC */
#define MALLINCAM_FLAG_ISP                  0x00040000  /* ISP (Image Signal Processing) chip */
#define MALLINCAM_FLAG_TRIGGER_SOFTWARE     0x00080000  /* support software trigger */
#define MALLINCAM_FLAG_TRIGGER_EXTERNAL     0x00100000  /* support external trigger */
#define MALLINCAM_FLAG_TRIGGER_SINGLE       0x00200000  /* only support trigger single: one trigger, one image */
#define MALLINCAM_FLAG_BLACKLEVEL           0x00400000  /* support set and get the black level */
#define MALLINCAM_FLAG_AUTO_FOCUS           0x00800000  /* support auto focus */
#define MALLINCAM_FLAG_BUFFER               0x01000000  /* frame buffer */
#define MALLINCAM_FLAG_DDR                  0x02000000  /* use very large capacity DDR (Double Data Rate SDRAM) for frame buffer. The capacity is not less than one full frame */
#define MALLINCAM_FLAG_CG                   0x04000000  /* Conversion Gain: HCG, LCG */
#define MALLINCAM_FLAG_YUV411               0x08000000  /* pixel format, yuv411 */
#define MALLINCAM_FLAG_VUYY                 0x10000000  /* pixel format, yuv422, VUYY */
#define MALLINCAM_FLAG_YUV444               0x20000000  /* pixel format, yuv444 */
#define MALLINCAM_FLAG_RGB888               0x40000000  /* pixel format, RGB888 */
#define MALLINCAM_FLAG_RAW8                 0x80000000  /* pixel format, RAW 8 bits */
#define MALLINCAM_FLAG_GMCY8                0x0000000100000000  /* pixel format, GMCY, 8bits */
#define MALLINCAM_FLAG_GMCY12               0x0000000200000000  /* pixel format, GMCY, 12bits */
#define MALLINCAM_FLAG_UYVY                 0x0000000400000000  /* pixel format, yuv422, UYVY */
#define MALLINCAM_FLAG_CGHDR                0x0000000800000000  /* Conversion Gain: HCG, LCG, HDR */
#define MALLINCAM_FLAG_GLOBALSHUTTER        0x0000001000000000  /* global shutter */
#define MALLINCAM_FLAG_FOCUSMOTOR           0x0000002000000000  /* support focus motor */
#define MALLINCAM_FLAG_PRECISE_FRAMERATE    0x0000004000000000  /* support precise framerate & bandwidth, see MALLINCAM_OPTION_PRECISE_FRAMERATE & MALLINCAM_OPTION_BANDWIDTH */
#define MALLINCAM_FLAG_HEAT                 0x0000008000000000  /* support heat to prevent fogging up */
#define MALLINCAM_FLAG_LOW_NOISE            0x0000010000000000  /* support low noise mode (Higher signal noise ratio, lower frame rate) */
#define MALLINCAM_FLAG_LEVELRANGE_HARDWARE  0x0000020000000000  /* hardware level range, put(get)_LevelRangeV2 */
#define MALLINCAM_FLAG_EVENT_HARDWARE       0x0000040000000000  /* hardware event, such as exposure start & stop */
#define MALLINCAM_FLAG_LIGHTSOURCE          0x0000080000000000  /* embedded light source */
#define MALLINCAM_FLAG_FILTERWHEEL          0x0000100000000000  /* astro filter wheel */
#define MALLINCAM_FLAG_GIGE                 0x0000200000000000  /* 1 Gigabit GigE */
#define MALLINCAM_FLAG_10GIGE               0x0000400000000000  /* 10 Gigabit GigE */
#define MALLINCAM_FLAG_5GIGE                0x0000800000000000  /* 5 Gigabit GigE */
#define MALLINCAM_FLAG_25GIGE               0x0001000000000000  /* 2.5 Gigabit GigE */
#define MALLINCAM_FLAG_AUTOFOCUSER          0x0002000000000000  /* astro auto focuser */
#define MALLINCAM_FLAG_LIGHT_SOURCE         0x0004000000000000  /* stand alone light source */
#define MALLINCAM_FLAG_CAMERALINK           0x0008000000000000  /* camera link */
#define MALLINCAM_FLAG_CXP                  0x0010000000000000  /* CXP: CoaXPress */
#define MALLINCAM_FLAG_RAW12PACK            0x0020000000000000  /* pixel format, RAW 12bits packed */
#define MALLINCAM_FLAG_SELFTRIGGER          0x0040000000000000  /* self trigger */
#define MALLINCAM_FLAG_RAW11                0x0080000000000000  /* pixel format, RAW 11bits */
#define MALLINCAM_FLAG_GHOPTO               0x0100000000000000  /* ghopto sensor */
#define MALLINCAM_FLAG_RAW10PACK            0x0200000000000000  /* pixel format, RAW 10bits packed */
#define MALLINCAM_FLAG_USB32                0x0400000000000000  /* usb3.2 */
#define MALLINCAM_FLAG_USB32_OVER_USB30     0x0800000000000000  /* usb3.2 camera connected to usb3.0 port */

#define MALLINCAM_EXPOGAIN_DEF              100     /* exposure gain, default value */
#define MALLINCAM_EXPOGAIN_MIN              100     /* exposure gain, minimum value */
#define MALLINCAM_TEMP_DEF                  6503    /* color temperature, default value */
#define MALLINCAM_TEMP_MIN                  2000    /* color temperature, minimum value */
#define MALLINCAM_TEMP_MAX                  15000   /* color temperature, maximum value */
#define MALLINCAM_TINT_DEF                  1000    /* tint */
#define MALLINCAM_TINT_MIN                  200     /* tint */
#define MALLINCAM_TINT_MAX                  2500    /* tint */
#define MALLINCAM_HUE_DEF                   0       /* hue */
#define MALLINCAM_HUE_MIN                   (-180)  /* hue */
#define MALLINCAM_HUE_MAX                   180     /* hue */
#define MALLINCAM_SATURATION_DEF            128     /* saturation */
#define MALLINCAM_SATURATION_MIN            0       /* saturation */
#define MALLINCAM_SATURATION_MAX            255     /* saturation */
#define MALLINCAM_BRIGHTNESS_DEF            0       /* brightness */
#define MALLINCAM_BRIGHTNESS_MIN            (-255)  /* brightness */
#define MALLINCAM_BRIGHTNESS_MAX            255     /* brightness */
#define MALLINCAM_CONTRAST_DEF              0       /* contrast */
#define MALLINCAM_CONTRAST_MIN              (-255)  /* contrast */
#define MALLINCAM_CONTRAST_MAX              255     /* contrast */
#define MALLINCAM_GAMMA_DEF                 100     /* gamma */
#define MALLINCAM_GAMMA_MIN                 20      /* gamma */
#define MALLINCAM_GAMMA_MAX                 180     /* gamma */
#define MALLINCAM_AETARGET_DEF              120     /* target of auto exposure */
#define MALLINCAM_AETARGET_MIN              16      /* target of auto exposure */
#define MALLINCAM_AETARGET_MAX              220     /* target of auto exposure */
#define MALLINCAM_WBGAIN_DEF                0       /* white balance gain */
#define MALLINCAM_WBGAIN_MIN                (-127)  /* white balance gain */
#define MALLINCAM_WBGAIN_MAX                127     /* white balance gain */
#define MALLINCAM_BLACKLEVEL_MIN            0       /* minimum black level */
#define MALLINCAM_BLACKLEVEL8_MAX           31              /* maximum black level for bitdepth = 8 */
#define MALLINCAM_BLACKLEVEL10_MAX          (31 * 4)        /* maximum black level for bitdepth = 10 */
#define MALLINCAM_BLACKLEVEL11_MAX          (31 * 8)        /* maximum black level for bitdepth = 11 */
#define MALLINCAM_BLACKLEVEL12_MAX          (31 * 16)       /* maximum black level for bitdepth = 12 */
#define MALLINCAM_BLACKLEVEL14_MAX          (31 * 64)       /* maximum black level for bitdepth = 14 */
#define MALLINCAM_BLACKLEVEL16_MAX          (31 * 256)      /* maximum black level for bitdepth = 16 */
#define MALLINCAM_SHARPENING_STRENGTH_DEF   0       /* sharpening strength */
#define MALLINCAM_SHARPENING_STRENGTH_MIN   0       /* sharpening strength */
#define MALLINCAM_SHARPENING_STRENGTH_MAX   500     /* sharpening strength */
#define MALLINCAM_SHARPENING_RADIUS_DEF     2       /* sharpening radius */
#define MALLINCAM_SHARPENING_RADIUS_MIN     1       /* sharpening radius */
#define MALLINCAM_SHARPENING_RADIUS_MAX     10      /* sharpening radius */
#define MALLINCAM_SHARPENING_THRESHOLD_DEF  0       /* sharpening threshold */
#define MALLINCAM_SHARPENING_THRESHOLD_MIN  0       /* sharpening threshold */
#define MALLINCAM_SHARPENING_THRESHOLD_MAX  255     /* sharpening threshold */
#define MALLINCAM_AUTOEXPO_THRESHOLD_DEF    5       /* auto exposure threshold */
#define MALLINCAM_AUTOEXPO_THRESHOLD_MIN    2       /* auto exposure threshold */
#define MALLINCAM_AUTOEXPO_THRESHOLD_MAX    15      /* auto exposure threshold */
#define MALLINCAM_AUTOEXPO_THLD_TRIGGER_DEF 5       /* auto exposure trigger threshold */
#define MALLINCAM_AUTOEXPO_THLD_TRIGGER_MIN 2       /* auto exposure trigger threshold */
#define MALLINCAM_AUTOEXPO_THLD_TRIGGER_MAX 64      /* auto exposure trigger threshold */
#define MALLINCAM_AUTOEXPO_DAMP_DEF         0       /* auto exposure damping coefficient: thousandths */
#define MALLINCAM_AUTOEXPO_DAMP_MIN         0       /* auto exposure damping coefficient: thousandths */
#define MALLINCAM_AUTOEXPO_DAMP_MAX         1000    /* auto exposure damping coefficient: thousandths */
#define MALLINCAM_BANDWIDTH_DEF             100     /* bandwidth */
#define MALLINCAM_BANDWIDTH_MIN             1       /* bandwidth */
#define MALLINCAM_BANDWIDTH_MAX             100     /* bandwidth */
#define MALLINCAM_DENOISE_DEF               0       /* denoise */
#define MALLINCAM_DENOISE_MIN               0       /* denoise */
#define MALLINCAM_DENOISE_MAX               100     /* denoise */
#define MALLINCAM_HEARTBEAT_MIN             100     /* millisecond */
#define MALLINCAM_HEARTBEAT_MAX             10000   /* millisecond */
#define MALLINCAM_AE_PERCENT_MIN            0       /* auto exposure percent; 0 or 100 => full roi average, means "disabled" */
#define MALLINCAM_AE_PERCENT_MAX            100
#define MALLINCAM_AE_PERCENT_DEF            1       /* auto exposure percent: enabled, percentage = 1% */
#define MALLINCAM_NOPACKET_TIMEOUT_MIN      500     /* no packet timeout minimum: 500ms */
#define MALLINCAM_NOFRAME_TIMEOUT_MIN       500     /* no frame timeout minimum: 500ms */
#define MALLINCAM_DYNAMIC_DEFECT_T1_MIN     0       /* dynamic defect pixel correction, dead pixel ratio: the smaller the dead ratio is, the more stringent the conditions for processing dead pixels are, and fewer pixels will be processed */
#define MALLINCAM_DYNAMIC_DEFECT_T1_MAX     100     /* means: 1.0 */
#define MALLINCAM_DYNAMIC_DEFECT_T1_DEF     90      /* means: 0.9 */
#define MALLINCAM_DYNAMIC_DEFECT_T2_MIN     0       /* dynamic defect pixel correction, hot pixel ratio: the smaller the hot ratio is, the more stringent the conditions for processing hot pixels are, and fewer pixels will be processed */
#define MALLINCAM_DYNAMIC_DEFECT_T2_MAX     100
#define MALLINCAM_DYNAMIC_DEFECT_T2_DEF     90
#define MALLINCAM_HDR_K_MIN                 1       /* HDR synthesize */
#define MALLINCAM_HDR_K_MAX                 25500
#define MALLINCAM_HDR_B_MIN                 0
#define MALLINCAM_HDR_B_MAX                 65535
#define MALLINCAM_HDR_THRESHOLD_MIN         0
#define MALLINCAM_HDR_THRESHOLD_MAX         4094
#define MALLINCAM_CDS_MIN                   0       /* Correlated Double Sampling */
#define MALLINCAM_CDS_MAX                   100

typedef struct {
    unsigned    width;
    unsigned    height;
} MallincamResolution;

/* In Windows platform, we always use UNICODE wchar_t */
/* In Linux or macOS, we use char */

typedef struct {
#if defined(_WIN32)
    const wchar_t*      name;        /* model name, in Windows, we use unicode */
#else
    const char*         name;        /* model name */
#endif
    unsigned long long  flag;        /* MALLINCAM_FLAG_xxx, 64 bits */
    unsigned            maxspeed;    /* number of speed level, same as Mallincam_get_MaxSpeed(), speed range = [0, maxspeed], closed interval */
    unsigned            preview;     /* number of preview resolution, same as Mallincam_get_ResolutionNumber() */
    unsigned            still;       /* number of still resolution, same as Mallincam_get_StillResolutionNumber() */
    unsigned            maxfanspeed; /* maximum fan speed, fan speed range = [0, max], closed interval */
    unsigned            ioctrol;     /* number of input/output control */
    float               xpixsz;      /* physical pixel size in micrometer */
    float               ypixsz;      /* physical pixel size in micrometer */
    MallincamResolution   res[16];
} MallincamModelV2; /* device model v2 */

typedef struct {
#if defined(_WIN32)
    wchar_t               displayname[64];    /* display name: model name or user-defined name (if any and Mallincam_EnumWithName) */
    wchar_t               id[64];             /* unique and opaque id of a connected camera, for Mallincam_Open */
#else
    char                  displayname[64];    /* display name: model name or user-defined name (if any and Mallincam_EnumWithName) */
    char                  id[64];             /* unique and opaque id of a connected camera, for Mallincam_Open */
#endif
    const MallincamModelV2* model;
} MallincamDeviceV2; /* device instance for enumerating */

/*
    get the version of this dll/so/dylib, which is: 59.28981.20250715
*/
#if defined(_WIN32)
MALLINCAM_API(const wchar_t*)   Mallincam_Version();
#else
MALLINCAM_API(const char*)      Mallincam_Version();
#endif

/*
    enumerate the cameras connected to the computer, return the number of enumerated.

    MallincamDeviceV2 arr[MALLINCAM_MAX];
    unsigned cnt = Mallincam_EnumV2(arr);
    for (unsigned i = 0; i < cnt; ++i)
        ...

    if arr == NULL, then, only the number is returned.
    Mallincam_Enum is obsolete.
*/
MALLINCAM_API(unsigned) Mallincam_EnumV2(MallincamDeviceV2 arr[MALLINCAM_MAX]);

/* use the camId of MallincamDeviceV2, which is enumerated by Mallincam_EnumV2.
    if camId is NULL, Mallincam_Open will open the first enumerated camera.
    For USB, GigE or PCIe camera, the camId can the camId can also be specified as (case sensitive):
        (a) "sn:xxxxxxxxxxxx" (such as sn:ZP250212241204105)
        (b) "name:xxxxxx" (such as name: Camera1)
    Moreover, for GigE camera, the camId can also be specified as (case sensitive):
        (a) "ip:xxx.xxx.xxx.xxx" (such as ip:192.168.1.100) or
        (b) "mac:xxxxxxxxxxxx" (such as mac:d05f64ffff23)
    For the issue of opening the camera on Android, please refer to the documentation
*/
#if defined(_WIN32)
MALLINCAM_API(HMallincam) Mallincam_Open(const wchar_t* camId);
#else
MALLINCAM_API(HMallincam) Mallincam_Open(const char* camId);
#endif

/*
    the same with Mallincam_Open, but use the index as the parameter. such as:
    index == 0, open the first camera,
    index == 1, open the second camera,
    etc
*/
MALLINCAM_API(HMallincam) Mallincam_OpenByIndex(unsigned index);

/* close the handle. After it is closed, never use the handle any more. */
MALLINCAM_API(void)     Mallincam_Close(HMallincam h);

#define MALLINCAM_EVENT_EXPOSURE          0x0001    /* exposure time or gain changed */
#define MALLINCAM_EVENT_TEMPTINT          0x0002    /* white balance changed, Temp/Tint mode */
#define MALLINCAM_EVENT_IMAGE             0x0004    /* live image arrived, use Mallincam_PullImageXXXX to get this image */
#define MALLINCAM_EVENT_STILLIMAGE        0x0005    /* snap (still) frame arrived, use Mallincam_PullStillImageXXXX to get this frame */
#define MALLINCAM_EVENT_WBGAIN            0x0006    /* white balance changed, RGB Gain mode */
#define MALLINCAM_EVENT_TRIGGERFAIL       0x0007    /* trigger failed */
#define MALLINCAM_EVENT_BLACK             0x0008    /* black balance changed */
#define MALLINCAM_EVENT_FFC               0x0009    /* flat field correction status changed */
#define MALLINCAM_EVENT_DFC               0x000a    /* dark field correction status changed */
#define MALLINCAM_EVENT_ROI               0x000b    /* roi changed */
#define MALLINCAM_EVENT_LEVELRANGE        0x000c    /* level range changed */
#define MALLINCAM_EVENT_AUTOEXPO_CONV     0x000d    /* auto exposure convergence */
#define MALLINCAM_EVENT_AUTOEXPO_CONVFAIL 0x000e    /* auto exposure once mode convergence failed */
#define MALLINCAM_EVENT_FPNC              0x000f    /* fix pattern noise correction status changed */
#define MALLINCAM_EVENT_ERROR             0x0080    /* generic error */
#define MALLINCAM_EVENT_DISCONNECTED      0x0081    /* camera disconnected */
#define MALLINCAM_EVENT_NOFRAMETIMEOUT    0x0082    /* no frame timeout error */
#define MALLINCAM_EVENT_FOCUSPOS          0x0084    /* focus positon */
#define MALLINCAM_EVENT_NOPACKETTIMEOUT   0x0085    /* no packet timeout */
#define MALLINCAM_EVENT_EXPO_START        0x4000    /* hardware event: exposure start */
#define MALLINCAM_EVENT_EXPO_STOP         0x4001    /* hardware event: exposure stop */
#define MALLINCAM_EVENT_TRIGGER_ALLOW     0x4002    /* hardware event: next trigger allow */
#define MALLINCAM_EVENT_HEARTBEAT         0x4003    /* hardware event: heartbeat, can be used to monitor whether the camera is alive */
#define MALLINCAM_EVENT_TRIGGER_IN        0x4004    /* hardware event: trigger in */
#define MALLINCAM_EVENT_FACTORY           0x8001    /* restore factory settings */

#if defined(_WIN32)
MALLINCAM_API(HRESULT)  Mallincam_StartPullModeWithWndMsg(HMallincam h, HWND hWnd, UINT nMsg);
#endif

/* Do NOT call Mallincam_Close, Mallincam_Stop in this callback context, it deadlocks. */
/* Do NOT call Mallincam_put_Option with MALLINCAM_OPTION_TRIGGER, MALLINCAM_OPTION_BITDEPTH, MALLINCAM_OPTION_PIXEL_FORMAT, MALLINCAM_OPTION_BINNING, MALLINCAM_OPTION_ROTATE, it will fail with error code E_WRONG_THREAD */
typedef void (__stdcall* PMALLINCAM_EVENT_CALLBACK)(unsigned nEvent, void* ctxEvent);
MALLINCAM_API(HRESULT)  Mallincam_StartPullModeWithCallback(HMallincam h, PMALLINCAM_EVENT_CALLBACK funEvent, void* ctxEvent);

#define MALLINCAM_FRAMEINFO_FLAG_SEQ                0x00000001 /* frame sequence number */
#define MALLINCAM_FRAMEINFO_FLAG_TIMESTAMP          0x00000002 /* timestamp */
#define MALLINCAM_FRAMEINFO_FLAG_EXPOTIME           0x00000004 /* exposure time */
#define MALLINCAM_FRAMEINFO_FLAG_EXPOGAIN           0x00000008 /* exposure gain */
#define MALLINCAM_FRAMEINFO_FLAG_BLACKLEVEL         0x00000010 /* black level */
#define MALLINCAM_FRAMEINFO_FLAG_SHUTTERSEQ         0x00000020 /* sequence shutter counter */
#define MALLINCAM_FRAMEINFO_FLAG_GPS                0x00000040 /* GPS */
#define MALLINCAM_FRAMEINFO_FLAG_AUTOFOCUS          0x00000080 /* auto focus: uLum & uFV */
#define MALLINCAM_FRAMEINFO_FLAG_COUNT              0x00000100 /* timecount, framecount, tricount */
#define MALLINCAM_FRAMEINFO_FLAG_MECHANICALSHUTTER  0x00000200 /* Mechanical shutter: closed */
#define MALLINCAM_FRAMEINFO_FLAG_STILL              0x00008000 /* still image */
#define MALLINCAM_FRAMEINFO_FLAG_CG                 0x00010000 /* Conversion Gain: High */

typedef struct {
    unsigned            width;
    unsigned            height;
    unsigned            flag;       /* MALLINCAM_FRAMEINFO_FLAG_xxxx */
    unsigned            seq;        /* frame sequence number */
    unsigned long long  timestamp;  /* microsecond */
    unsigned            shutterseq; /* sequence shutter counter */
    unsigned            expotime;   /* exposure time */
    unsigned short      expogain;   /* exposure gain */
    unsigned short      blacklevel; /* black level */
} MallincamFrameInfoV3;

typedef struct {
    unsigned long long utcstart;    /* exposure start time: nanosecond since epoch (00:00:00 UTC on Thursday, 1 January 1970, see https://en.wikipedia.org/wiki/Unix_time) */
    unsigned long long utcend;      /* exposure end time */
    int                longitude;   /* millionth of a degree, 0.000001 degree */
    int                latitude;
    int                altitude;    /* millimeter */
    unsigned short     satellite;   /* number of satellite */
    unsigned short     reserved;    /* not used */
} MallincamGps;

typedef struct {
    MallincamFrameInfoV3 v3;
    unsigned reserved; /* not used */
    unsigned uLum;
    unsigned long long uFV;
    unsigned long long timecount;
    unsigned framecount, tricount;
    MallincamGps gps;
} MallincamFrameInfoV4;

/*
    nWaitMS: The timeout interval, in milliseconds. If a nonzero value is specified, the function waits until the image is ok or the interval elapses.
             If nWaitMS is zero, the function does not enter a wait state if the image is not available; it always returns immediately; this is equal to Mallincam_PullImageV4.
    bStill: to pull still image, set to 1, otherwise 0
    bits: 24 (RGB24), 32 (RGB32), 48 (RGB48), 8 (Grey), 16 (Grey), 64 (RGB64).
          In RAW mode, this parameter is ignored.
          bits = 0 means using default bits base on MALLINCAM_OPTION_RGB.
          When bits and MALLINCAM_OPTION_RGB are inconsistent, format conversion will have to be performed, resulting in loss of efficiency.
          See the following bits and MALLINCAM_OPTION_RGB correspondence table:
            ----------------------------------------------------------------------------------------------------------------------
            | MALLINCAM_OPTION_RGB |   0 (RGB24)   |   1 (RGB48)   |   2 (RGB32)   |   3 (Grey8)   |  4 (Grey16)   |   5 (RGB64)   |
            |--------------------|---------------|---------------|---------------|---------------|---------------|---------------|
            | bits = 0           |      24       |       48      |      32       |       8       |       16      |       64      |
            |--------------------|---------------|---------------|---------------|---------------|---------------|---------------|
            | bits = 24          |      24       |       NA      | Convert to 24 | Convert to 24 |       NA      |       NA      |
            |--------------------|---------------|---------------|---------------|---------------|---------------|---------------|
            | bits = 32          | Convert to 32 |       NA      |       32      | Convert to 32 |       NA      |       NA      |
            |--------------------|---------------|---------------|---------------|---------------|---------------|---------------|
            | bits = 48          |      NA       |       48      |       NA      |       NA      | Convert to 48 | Convert to 48 |
            |--------------------|---------------|---------------|---------------|---------------|---------------|---------------|
            | bits = 8           | Convert to 8  |       NA      | Convert to 8  |       8       |       NA      |       NA      |
            |--------------------|---------------|---------------|---------------|---------------|---------------|---------------|
            | bits = 16          |      NA       | Convert to 16 |       NA      |       NA      |       16      | Convert to 16 |
            |--------------------|---------------|---------------|---------------|---------------|---------------|---------------|
            | bits = 64          |      NA       | Convert to 64 |       NA      |       NA      | Convert to 64 |       64      |
            |--------------------|---------------|---------------|---------------|---------------|---------------|---------------|

    rowPitch: The distance from one row to the next row. rowPitch = 0 means using the default row pitch. rowPitch = -1 means zero padding, see below:
            ----------------------------------------------------------------------------------------------
            | format                             | 0 means default row pitch     | -1 means zero padding |
            |------------------------------------|-------------------------------|-----------------------|
            | RGB       | RGB24                  | TDIBWIDTHBYTES(24 * Width)    | Width * 3             |
            |           | RGB32                  | Width * 4                     | Width * 4             |
            |           | RGB48                  | TDIBWIDTHBYTES(48 * Width)    | Width * 6             |
            |           | GREY8                  | TDIBWIDTHBYTES(8 * Width)     | Width                 |
            |           | GREY16                 | TDIBWIDTHBYTES(16 * Width)    | Width * 2             |
            |           | RGB64                  | Width * 8                     | Width * 8             |
            |-----------|------------------------|-------------------------------|-----------------------|
            | RAW       | 8bits Mode             | Width                         | Width                 |
            |           | 10/12/14/16bits Mode   | Width * 2                     | Width * 2             |
            |-----------|------------------------|-------------------------------|-----------------------|
*/
MALLINCAM_API(HRESULT)  Mallincam_PullImageV4(HMallincam h, void* pImageData, int bStill, int bits, int rowPitch, MallincamFrameInfoV4* pInfo);
MALLINCAM_API(HRESULT)  Mallincam_WaitImageV4(HMallincam h, unsigned nWaitMS, void* pImageData, int bStill, int bits, int rowPitch, MallincamFrameInfoV4* pInfo);
MALLINCAM_API(HRESULT)  Mallincam_PullImageV3(HMallincam h, void* pImageData, int bStill, int bits, int rowPitch, MallincamFrameInfoV3* pInfo);
MALLINCAM_API(HRESULT)  Mallincam_WaitImageV3(HMallincam h, unsigned nWaitMS, void* pImageData, int bStill, int bits, int rowPitch, MallincamFrameInfoV3* pInfo);

typedef struct {
    unsigned            width;
    unsigned            height;
    unsigned            flag;       /* MALLINCAM_FRAMEINFO_FLAG_xxxx */
    unsigned            seq;        /* frame sequence number */
    unsigned long long  timestamp;  /* microsecond */
} MallincamFrameInfoV2;

MALLINCAM_API(HRESULT)  Mallincam_PullImageV2(HMallincam h, void* pImageData, int bits, MallincamFrameInfoV2* pInfo);
MALLINCAM_API(HRESULT)  Mallincam_PullStillImageV2(HMallincam h, void* pImageData, int bits, MallincamFrameInfoV2* pInfo);
MALLINCAM_API(HRESULT)  Mallincam_PullImageWithRowPitchV2(HMallincam h, void* pImageData, int bits, int rowPitch, MallincamFrameInfoV2* pInfo);
MALLINCAM_API(HRESULT)  Mallincam_PullStillImageWithRowPitchV2(HMallincam h, void* pImageData, int bits, int rowPitch, MallincamFrameInfoV2* pInfo);

MALLINCAM_API(HRESULT)  Mallincam_PullImage(HMallincam h, void* pImageData, int bits, unsigned* pnWidth, unsigned* pnHeight);
MALLINCAM_API(HRESULT)  Mallincam_PullStillImage(HMallincam h, void* pImageData, int bits, unsigned* pnWidth, unsigned* pnHeight);
MALLINCAM_API(HRESULT)  Mallincam_PullImageWithRowPitch(HMallincam h, void* pImageData, int bits, int rowPitch, unsigned* pnWidth, unsigned* pnHeight);
MALLINCAM_API(HRESULT)  Mallincam_PullStillImageWithRowPitch(HMallincam h, void* pImageData, int bits, int rowPitch, unsigned* pnWidth, unsigned* pnHeight);

/*
    (NULL == pData) means something error
    ctxData is the callback context which is passed by Mallincam_StartPushModeV3
    bSnap: TRUE if Mallincam_Snap

    funData is callbacked by an internal thread of mallincam.dll, so please pay attention to multithread problem.
    Do NOT call Mallincam_Close, Mallincam_Stop in this callback context, it deadlocks.
*/
typedef void (__stdcall* PMALLINCAM_DATA_CALLBACK_V4)(const void* pData, const MallincamFrameInfoV3* pInfo, int bSnap, void* ctxData);
MALLINCAM_API(HRESULT)  Mallincam_StartPushModeV4(HMallincam h, PMALLINCAM_DATA_CALLBACK_V4 funData, void* ctxData, PMALLINCAM_EVENT_CALLBACK funEvent, void* ctxEvent);

typedef void (__stdcall* PMALLINCAM_DATA_CALLBACK_V3)(const void* pData, const MallincamFrameInfoV2* pInfo, int bSnap, void* ctxData);
MALLINCAM_API(HRESULT)  Mallincam_StartPushModeV3(HMallincam h, PMALLINCAM_DATA_CALLBACK_V3 funData, void* ctxData, PMALLINCAM_EVENT_CALLBACK funEvent, void* ctxEvent);

MALLINCAM_API(HRESULT)  Mallincam_Stop(HMallincam h);
MALLINCAM_API(HRESULT)  Mallincam_Pause(HMallincam h, int bPause); /* 1 => pause, 0 => continue */

/*  for pull mode: MALLINCAM_EVENT_STILLIMAGE, and then Mallincam_PullStillImageXXXX/Mallincam_PullImageV4
    for push mode: the snapped image will be return by PMALLINCAM_DATA_CALLBACK(V2/V3), with the parameter 'bSnap' set to 'TRUE'
    nResolutionIndex = 0xffffffff means use the cureent preview resolution
*/
MALLINCAM_API(HRESULT)  Mallincam_Snap(HMallincam h, unsigned nResolutionIndex);  /* still image snap */
MALLINCAM_API(HRESULT)  Mallincam_SnapN(HMallincam h, unsigned nResolutionIndex, unsigned nNumber);  /* multiple still image snap */
MALLINCAM_API(HRESULT)  Mallincam_SnapR(HMallincam h, unsigned nResolutionIndex, unsigned nNumber);  /* multiple RAW still image snap */
/*
    soft trigger:
    nNumber:    0xffff:     trigger continuously
                0:          cancel trigger, see MALLINCAM_OPTION_TRIGGER_CANCEL_MODE
                others:     number of images to be triggered
*/
MALLINCAM_API(HRESULT)  Mallincam_Trigger(HMallincam h, unsigned short nNumber);

/*
    trigger synchronously
    nWaitMS:    0:              by default, exposure * 102% + 4000 milliseconds
                0xffffffff:     wait infinite
                other:          milliseconds to wait
*/
MALLINCAM_API(HRESULT)  Mallincam_TriggerSyncV4(HMallincam h, unsigned nWaitMS, void* pImageData, int bits, int rowPitch, MallincamFrameInfoV4* pInfo);
MALLINCAM_API(HRESULT)  Mallincam_TriggerSync(HMallincam h, unsigned nWaitMS, void* pImageData, int bits, int rowPitch, MallincamFrameInfoV3* pInfo);

/*
    put_Size, put_eSize, can be used to set the video output resolution BEFORE Mallincam_StartXXXX.
    put_Size use width and height parameters, put_eSize use the index parameter.
    for example, UCMOS03100KPA support the following resolutions:
            index 0:    2048,   1536
            index 1:    1024,   768
            index 2:    680,    510
    so, we can use put_Size(h, 1024, 768) or put_eSize(h, 1). Both have the same effect.
*/
MALLINCAM_API(HRESULT)  Mallincam_put_Size(HMallincam h, int nWidth, int nHeight);
MALLINCAM_API(HRESULT)  Mallincam_get_Size(HMallincam h, int* pWidth, int* pHeight);
MALLINCAM_API(HRESULT)  Mallincam_put_eSize(HMallincam h, unsigned nResolutionIndex);
MALLINCAM_API(HRESULT)  Mallincam_get_eSize(HMallincam h, unsigned* pnResolutionIndex);

/*
    final image size after ROI, rotate, binning
*/
MALLINCAM_API(HRESULT)  Mallincam_get_FinalSize(HMallincam h, int* pWidth, int* pHeight);

MALLINCAM_API(HRESULT)  Mallincam_get_ResolutionNumber(HMallincam h);
MALLINCAM_API(HRESULT)  Mallincam_get_Resolution(HMallincam h, unsigned nResolutionIndex, int* pWidth, int* pHeight);
/*
    numerator/denominator, such as: 1/1, 1/2, 1/3
*/
MALLINCAM_API(HRESULT)  Mallincam_get_ResolutionRatio(HMallincam h, unsigned nResolutionIndex, int* pNumerator, int* pDenominator);
MALLINCAM_API(HRESULT)  Mallincam_get_Field(HMallincam h);

/*
see: http://www.siliconimaging.com/RGB%20Bayer.htm
FourCC:
    MAKEFOURCC('G', 'B', 'R', 'G')
    MAKEFOURCC('R', 'G', 'G', 'B')
    MAKEFOURCC('B', 'G', 'G', 'R')
    MAKEFOURCC('G', 'R', 'B', 'G')
    MAKEFOURCC('Y', 'Y', 'Y', 'Y'), monochromatic sensor
    MAKEFOURCC('Y', '4', '1', '1'), yuv411
    MAKEFOURCC('V', 'U', 'Y', 'Y'), yuv422
    MAKEFOURCC('U', 'Y', 'V', 'Y'), yuv422
    MAKEFOURCC('Y', '4', '4', '4'), yuv444
    MAKEFOURCC('R', 'G', 'B', '8'), RGB888

#ifndef MAKEFOURCC
#define MAKEFOURCC(a, b, c, d) ((unsigned)(unsigned char)(a) | ((unsigned)(unsigned char)(b) << 8) | ((unsigned)(unsigned char)(c) << 16) | ((unsigned)(unsigned char)(d) << 24))
#endif
*/
MALLINCAM_API(HRESULT)  Mallincam_get_RawFormat(HMallincam h, unsigned* pFourCC, unsigned* pBitsPerPixel);

/*
    ------------------------------------------------------------------|
    | Parameter               |   Range       |   Default             |
    |-----------------------------------------------------------------|
    | Auto Exposure Target    |   10~220      |   120                 |
    | Exposure Gain           |   100~        |   100                 |
    | Temp                    |   1000~25000  |   6503                |
    | Tint                    |   100~2500    |   1000                |
    | LevelRange              |   0~255       |   Low = 0, High = 255 |
    | Contrast                |   -255~255    |   0                   |
    | Hue                     |   -180~180    |   0                   |
    | Saturation              |   0~255       |   128                 |
    | Brightness              |   -255~255    |   0                   |
    | Gamma                   |   20~180      |   100                 |
    | WBGain                  |   -127~127    |   0                   |
    ------------------------------------------------------------------|
*/

#ifndef __MALLINCAM_CALLBACK_DEFINED__
#define __MALLINCAM_CALLBACK_DEFINED__
typedef void (__stdcall* PIMALLINCAM_EXPOSURE_CALLBACK)(void* ctxExpo);                                 /* auto exposure */
typedef void (__stdcall* PIMALLINCAM_WHITEBALANCE_CALLBACK)(const int aGain[3], void* ctxWB);           /* once white balance, RGB Gain mode */
typedef void (__stdcall* PIMALLINCAM_BLACKBALANCE_CALLBACK)(const unsigned short aSub[3], void* ctxBB); /* once black balance */
typedef void (__stdcall* PIMALLINCAM_TEMPTINT_CALLBACK)(const int nTemp, const int nTint, void* ctxTT); /* once white balance, Temp/Tint Mode */
typedef void (__stdcall* PIMALLINCAM_HISTOGRAM_CALLBACK)(const float aHistY[256], const float aHistR[256], const float aHistG[256], const float aHistB[256], void* ctxHistogram);
typedef void (__stdcall* PIMALLINCAM_CHROME_CALLBACK)(void* ctxChrome);
typedef void (__stdcall* PIMALLINCAM_PROGRESS)(int percent, void* ctxProgress);
#endif

/*
* nFlag & 0x00008000: mono or color
* nFlag & 0x0f: bitdepth
* so the size of aHist is:
    int arraySize = 1 << (nFlag & 0x0f);
    if ((nFlag & 0x00008000) == 0)
        arraySize *= 3;
*/
typedef void (__stdcall* PIMALLINCAM_HISTOGRAM_CALLBACKV2)(const unsigned* aHist, unsigned nFlag, void* ctxHistogramV2);

/*
* bAutoExposure:
*   0: disable auto exposure
*   1: auto exposure continue mode
*   2: auto exposure once mode
*/
MALLINCAM_API(HRESULT)  Mallincam_get_AutoExpoEnable(HMallincam h, int* bAutoExposure);
MALLINCAM_API(HRESULT)  Mallincam_put_AutoExpoEnable(HMallincam h, int bAutoExposure);

MALLINCAM_API(HRESULT)  Mallincam_get_AutoExpoTarget(HMallincam h, unsigned short* Target);
MALLINCAM_API(HRESULT)  Mallincam_put_AutoExpoTarget(HMallincam h, unsigned short Target);

/* set the maximum/minimal auto exposure time and agin. The default maximum auto exposure time is 350ms */
MALLINCAM_API(HRESULT)  Mallincam_put_AutoExpoRange(HMallincam h, unsigned maxTime, unsigned minTime, unsigned short maxGain, unsigned short minGain);
MALLINCAM_API(HRESULT)  Mallincam_get_AutoExpoRange(HMallincam h, unsigned* maxTime, unsigned* minTime, unsigned short* maxGain, unsigned short* minGain);
MALLINCAM_API(HRESULT)  Mallincam_put_MaxAutoExpoTimeAGain(HMallincam h, unsigned maxTime, unsigned short maxGain);
MALLINCAM_API(HRESULT)  Mallincam_get_MaxAutoExpoTimeAGain(HMallincam h, unsigned* maxTime, unsigned short* maxGain);
MALLINCAM_API(HRESULT)  Mallincam_put_MinAutoExpoTimeAGain(HMallincam h, unsigned minTime, unsigned short minGain);
MALLINCAM_API(HRESULT)  Mallincam_get_MinAutoExpoTimeAGain(HMallincam h, unsigned* minTime, unsigned short* minGain);

MALLINCAM_API(HRESULT)  Mallincam_get_ExpoTime(HMallincam h, unsigned* Time); /* in microseconds */
MALLINCAM_API(HRESULT)  Mallincam_put_ExpoTime(HMallincam h, unsigned Time); /* in microseconds */
MALLINCAM_API(HRESULT)  Mallincam_get_RealExpoTime(HMallincam h, unsigned* Time); /* actual exposure time */
MALLINCAM_API(HRESULT)  Mallincam_get_ExpTimeRange(HMallincam h, unsigned* nMin, unsigned* nMax, unsigned* nDef);

MALLINCAM_API(HRESULT)  Mallincam_get_ExpoAGain(HMallincam h, unsigned short* Gain); /* percent, such as 300 */
MALLINCAM_API(HRESULT)  Mallincam_put_ExpoAGain(HMallincam h, unsigned short Gain); /* percent */
MALLINCAM_API(HRESULT)  Mallincam_get_ExpoAGainRange(HMallincam h, unsigned short* nMin, unsigned short* nMax, unsigned short* nDef);

/* Auto White Balance "Once", Temp/Tint Mode */
MALLINCAM_API(HRESULT)  Mallincam_AwbOnce(HMallincam h, PIMALLINCAM_TEMPTINT_CALLBACK funTT, void* ctxTT); /* auto white balance "once". This function must be called AFTER Mallincam_StartXXXX */

/* Auto White Balance "Once", RGB Gain Mode */
MALLINCAM_API(HRESULT)  Mallincam_AwbInit(HMallincam h, PIMALLINCAM_WHITEBALANCE_CALLBACK funWB, void* ctxWB);

/* White Balance, Temp/Tint mode */
MALLINCAM_API(HRESULT)  Mallincam_put_TempTint(HMallincam h, int nTemp, int nTint);
MALLINCAM_API(HRESULT)  Mallincam_get_TempTint(HMallincam h, int* nTemp, int* nTint);

/* White Balance, RGB Gain mode */
MALLINCAM_API(HRESULT)  Mallincam_put_WhiteBalanceGain(HMallincam h, int aGain[3]);
MALLINCAM_API(HRESULT)  Mallincam_get_WhiteBalanceGain(HMallincam h, int aGain[3]);

/* Black Balance */
MALLINCAM_API(HRESULT)  Mallincam_AbbOnce(HMallincam h, PIMALLINCAM_BLACKBALANCE_CALLBACK funBB, void* ctxBB); /* auto black balance "once". This function must be called AFTER Mallincam_StartXXXX */
MALLINCAM_API(HRESULT)  Mallincam_put_BlackBalance(HMallincam h, unsigned short aSub[3]);
MALLINCAM_API(HRESULT)  Mallincam_get_BlackBalance(HMallincam h, unsigned short aSub[3]);

/* Flat Field Correction */
MALLINCAM_API(HRESULT)  Mallincam_FfcOnce(HMallincam h);
#if defined(_WIN32)
MALLINCAM_API(HRESULT)  Mallincam_FfcExport(HMallincam h, const wchar_t* filePath);
MALLINCAM_API(HRESULT)  Mallincam_FfcImport(HMallincam h, const wchar_t* filePath);
#else
MALLINCAM_API(HRESULT)  Mallincam_FfcExport(HMallincam h, const char* filePath);
MALLINCAM_API(HRESULT)  Mallincam_FfcImport(HMallincam h, const char* filePath);
#endif
MALLINCAM_API(HRESULT)  Mallincam_FfcFile(const void* data[], unsigned num, unsigned width, unsigned height, unsigned fourcc, unsigned bits, const char* outfile);

/* Dark Field Correction */
MALLINCAM_API(HRESULT)  Mallincam_DfcOnce(HMallincam h);

#if defined(_WIN32)
MALLINCAM_API(HRESULT)  Mallincam_DfcExport(HMallincam h, const wchar_t* filePath);
MALLINCAM_API(HRESULT)  Mallincam_DfcImport(HMallincam h, const wchar_t* filePath);
#else
MALLINCAM_API(HRESULT)  Mallincam_DfcExport(HMallincam h, const char* filePath);
MALLINCAM_API(HRESULT)  Mallincam_DfcImport(HMallincam h, const char* filePath);
#endif

/* Fix Pattern Noise Correction */
MALLINCAM_API(HRESULT)  Mallincam_FpncOnce(HMallincam h);

#if defined(_WIN32)
MALLINCAM_API(HRESULT)  Mallincam_FpncExport(HMallincam h, const wchar_t* filePath);
MALLINCAM_API(HRESULT)  Mallincam_FpncImport(HMallincam h, const wchar_t* filePath);
#else
MALLINCAM_API(HRESULT)  Mallincam_FpncExport(HMallincam h, const char* filePath);
MALLINCAM_API(HRESULT)  Mallincam_FpncImport(HMallincam h, const char* filePath);
#endif

MALLINCAM_API(HRESULT)  Mallincam_put_Hue(HMallincam h, int Hue);
MALLINCAM_API(HRESULT)  Mallincam_get_Hue(HMallincam h, int* Hue);
MALLINCAM_API(HRESULT)  Mallincam_put_Saturation(HMallincam h, int Saturation);
MALLINCAM_API(HRESULT)  Mallincam_get_Saturation(HMallincam h, int* Saturation);
MALLINCAM_API(HRESULT)  Mallincam_put_Brightness(HMallincam h, int Brightness);
MALLINCAM_API(HRESULT)  Mallincam_get_Brightness(HMallincam h, int* Brightness);
MALLINCAM_API(HRESULT)  Mallincam_get_Contrast(HMallincam h, int* Contrast);
MALLINCAM_API(HRESULT)  Mallincam_put_Contrast(HMallincam h, int Contrast);
MALLINCAM_API(HRESULT)  Mallincam_get_Gamma(HMallincam h, int* Gamma); /* percent */
MALLINCAM_API(HRESULT)  Mallincam_put_Gamma(HMallincam h, int Gamma);  /* percent */

MALLINCAM_API(HRESULT)  Mallincam_get_Chrome(HMallincam h, int* bChrome);  /* 1 => monochromatic mode, 0 => color mode */
MALLINCAM_API(HRESULT)  Mallincam_put_Chrome(HMallincam h, int bChrome);

MALLINCAM_API(HRESULT)  Mallincam_get_VFlip(HMallincam h, int* bVFlip);  /* vertical flip */
MALLINCAM_API(HRESULT)  Mallincam_put_VFlip(HMallincam h, int bVFlip);
MALLINCAM_API(HRESULT)  Mallincam_get_HFlip(HMallincam h, int* bHFlip);
MALLINCAM_API(HRESULT)  Mallincam_put_HFlip(HMallincam h, int bHFlip); /* horizontal flip */

MALLINCAM_API(HRESULT)  Mallincam_get_Negative(HMallincam h, int* bNegative);  /* negative film */
MALLINCAM_API(HRESULT)  Mallincam_put_Negative(HMallincam h, int bNegative);

MALLINCAM_API(HRESULT)  Mallincam_put_Speed(HMallincam h, unsigned short nSpeed);
MALLINCAM_API(HRESULT)  Mallincam_get_Speed(HMallincam h, unsigned short* pSpeed);
MALLINCAM_API(HRESULT)  Mallincam_get_MaxSpeed(HMallincam h); /* get the maximum speed, see "Frame Speed Level", the speed range = [0, max], closed interval */

MALLINCAM_API(HRESULT)  Mallincam_get_FanMaxSpeed(HMallincam h); /* get the maximum fan speed, the fan speed range = [0, max], closed interval */

MALLINCAM_API(HRESULT)  Mallincam_get_MaxBitDepth(HMallincam h); /* get the max bitdepth of this camera, such as 8, 10, 12, 14, 16 */

/* power supply of lighting:
        0 => 60HZ AC
        1 => 50Hz AC
        2 => DC
*/
MALLINCAM_API(HRESULT)  Mallincam_put_HZ(HMallincam h, int nHZ);
MALLINCAM_API(HRESULT)  Mallincam_get_HZ(HMallincam h, int* nHZ);

MALLINCAM_API(HRESULT)  Mallincam_put_Mode(HMallincam h, int bSkip); /* skip or bin */
MALLINCAM_API(HRESULT)  Mallincam_get_Mode(HMallincam h, int* bSkip); /* If the model don't support bin/skip mode, return E_NOTIMPL */

#if !defined(_WIN32)
#ifndef __RECT_DEFINED__
#define __RECT_DEFINED__
typedef struct {
    int left;
    int top;
    int right;
    int bottom;
} RECT, *PRECT;
#endif
#endif

MALLINCAM_API(HRESULT)  Mallincam_put_AWBAuxRect(HMallincam h, const RECT* pAuxRect); /* auto white balance ROI */
MALLINCAM_API(HRESULT)  Mallincam_get_AWBAuxRect(HMallincam h, RECT* pAuxRect);
MALLINCAM_API(HRESULT)  Mallincam_put_AEAuxRect(HMallincam h, const RECT* pAuxRect);  /* auto exposure ROI */
MALLINCAM_API(HRESULT)  Mallincam_get_AEAuxRect(HMallincam h, RECT* pAuxRect);

MALLINCAM_API(HRESULT)  Mallincam_put_ABBAuxRect(HMallincam h, const RECT* pAuxRect); /* auto black balance ROI */
MALLINCAM_API(HRESULT)  Mallincam_get_ABBAuxRect(HMallincam h, RECT* pAuxRect);

/*
    S_FALSE:    color mode
    S_OK:       mono mode, such as EXCCD00300KMA and UHCCD01400KMA
*/
MALLINCAM_API(HRESULT)  Mallincam_get_MonoMode(HMallincam h);

MALLINCAM_API(HRESULT)  Mallincam_get_StillResolutionNumber(HMallincam h);
MALLINCAM_API(HRESULT)  Mallincam_get_StillResolution(HMallincam h, unsigned nResolutionIndex, int* pWidth, int* pHeight);

/*  0: no realtime
          stop grab frame when frame buffer deque is full, until the frames in the queue are pulled away and the queue is not full
    1: realtime
          use minimum frame buffer. When new frame arrive, drop all the pending frame regardless of whether the frame buffer is full.
          If DDR present, also limit the DDR frame buffer to only one frame.
    2: soft realtime
          Drop the oldest frame when the queue is full and then enqueue the new frame
    default: 0
*/
MALLINCAM_API(HRESULT)  Mallincam_put_RealTime(HMallincam h, int val);
MALLINCAM_API(HRESULT)  Mallincam_get_RealTime(HMallincam h, int* val);

/* discard the current internal frame cache.
    If DDR present, also discard the frames in the DDR.
    Mallincam_Flush is obsolete, recommend using Mallincam_put_Option(h, MALLINCAM_OPTION_FLUSH, 3)
*/
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_Flush(HMallincam h);

/* get the temperature of the sensor, in 0.1 degrees Celsius (32 means 3.2 degrees Celsius, -35 means -3.5 degree Celsius)
    return E_NOTIMPL if not supported
*/
MALLINCAM_API(HRESULT)  Mallincam_get_Temperature(HMallincam h, short* pTemperature);

/* set the target temperature of the sensor or TEC, in 0.1 degrees Celsius (32 means 3.2 degrees Celsius, -35 means -3.5 degree Celsius)
    set "-2730" or below means using the default value of this model
    return E_NOTIMPL if not supported
*/
MALLINCAM_API(HRESULT)  Mallincam_put_Temperature(HMallincam h, short nTemperature);

/*
    get the revision
*/
MALLINCAM_API(HRESULT)  Mallincam_get_Revision(HMallincam h, unsigned short* pRevision);

/*
    get the serial number which is always 32 chars which is zero-terminated such as "TP110826145730ABCD1234FEDC56787"
*/
MALLINCAM_API(HRESULT)  Mallincam_get_SerialNumber(HMallincam h, char sn[32]);

/*
    get the camera firmware version, such as: 3.2.1.20140922
*/
MALLINCAM_API(HRESULT)  Mallincam_get_FwVersion(HMallincam h, char fwver[16]);

/*
    get the camera hardware version, such as: 3.12
*/
MALLINCAM_API(HRESULT)  Mallincam_get_HwVersion(HMallincam h, char hwver[16]);

/*
    get the production date, such as: 20150327, YYYYMMDD, (YYYY: year, MM: month, DD: day)
*/
MALLINCAM_API(HRESULT)  Mallincam_get_ProductionDate(HMallincam h, char pdate[10]);

/*
    get the FPGA version, such as: 1.13
*/
MALLINCAM_API(HRESULT)  Mallincam_get_FpgaVersion(HMallincam h, char fpgaver[16]);

/*
    get the sensor pixel size, such as: 2.4um x 2.4um
*/
MALLINCAM_API(HRESULT)  Mallincam_get_PixelSize(HMallincam h, unsigned nResolutionIndex, float* x, float* y);

/* software level range */
MALLINCAM_API(HRESULT)  Mallincam_put_LevelRange(HMallincam h, unsigned short aLow[4], unsigned short aHigh[4]);
MALLINCAM_API(HRESULT)  Mallincam_get_LevelRange(HMallincam h, unsigned short aLow[4], unsigned short aHigh[4]);

/* hardware level range mode */
#define MALLINCAM_LEVELRANGE_MANUAL       0x0000  /* manual */
#define MALLINCAM_LEVELRANGE_ONCE         0x0001  /* once */
#define MALLINCAM_LEVELRANGE_CONTINUE     0x0002  /* continue */
#define MALLINCAM_LEVELRANGE_ROI          0xffff  /* update roi rect only */
MALLINCAM_API(HRESULT)  Mallincam_put_LevelRangeV2(HMallincam h, unsigned short mode, const RECT* pRoiRect, unsigned short aLow[4], unsigned short aHigh[4]);
MALLINCAM_API(HRESULT)  Mallincam_get_LevelRangeV2(HMallincam h, unsigned short* pMode, RECT* pRoiRect, unsigned short aLow[4], unsigned short aHigh[4]);

/*
    The following functions must be called AFTER Mallincam_StartPushMode or Mallincam_StartPullModeWithWndMsg or Mallincam_StartPullModeWithCallback
*/
MALLINCAM_API(HRESULT)  Mallincam_LevelRangeAuto(HMallincam h);  /* software level range */
MALLINCAM_API(HRESULT)  Mallincam_GetHistogram(HMallincam h, PIMALLINCAM_HISTOGRAM_CALLBACK funHistogram, void* ctxHistogram);
MALLINCAM_API(HRESULT)  Mallincam_GetHistogramV2(HMallincam h, PIMALLINCAM_HISTOGRAM_CALLBACKV2 funHistogramV2, void* ctxHistogramV2);

/* led state:
    iLed: Led index, (0, 1, 2, ...)
    iState: 1 => Ever bright; 2 => Flashing; other => Off
    iPeriod: Flashing Period (>= 500ms)
*/
MALLINCAM_API(HRESULT)  Mallincam_put_LEDState(HMallincam h, unsigned short iLed, unsigned short iState, unsigned short iPeriod);

MALLINCAM_API(HRESULT)  Mallincam_write_EEPROM(HMallincam h, unsigned addr, const unsigned char* pBuffer, unsigned nBufferLen);
MALLINCAM_API(HRESULT)  Mallincam_read_EEPROM(HMallincam h, unsigned addr, unsigned char* pBuffer, unsigned nBufferLen);

MALLINCAM_API(HRESULT)  Mallincam_read_Pipe(HMallincam h, unsigned pipeId, void* pBuffer, unsigned nBufferLen);
MALLINCAM_API(HRESULT)  Mallincam_write_Pipe(HMallincam h, unsigned pipeId, const void* pBuffer, unsigned nBufferLen);
MALLINCAM_API(HRESULT)  Mallincam_feed_Pipe(HMallincam h, unsigned pipeId);

MALLINCAM_API(HRESULT)  Mallincam_put_Option(HMallincam h, unsigned iOption, int iValue);
MALLINCAM_API(HRESULT)  Mallincam_get_Option(HMallincam h, unsigned iOption, int* piValue);

/* [RW] = Read/Write; [RO] = Read Only; [WO] = Write Only */
#define MALLINCAM_OPTION_NOFRAME_TIMEOUT        0x01       /* [RW] no frame timeout: 0 => disable, positive value (>= MALLINCAM_NOFRAME_TIMEOUT_MIN) => timeout milliseconds. default: disable */
#define MALLINCAM_OPTION_THREAD_PRIORITY        0x02       /* [RW] set the priority of the internal thread which grab data from the usb device.
                                                             Win: iValue: 0 => THREAD_PRIORITY_NORMAL; 1 => THREAD_PRIORITY_ABOVE_NORMAL; 2 => THREAD_PRIORITY_HIGHEST; 3 => THREAD_PRIORITY_TIME_CRITICAL; default: 1; see: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreadpriority
                                                             Linux & macOS: The high 16 bits for the scheduling policy, and the low 16 bits for the priority; see: https://linux.die.net/man/3/pthread_setschedparam
                                                         */
#define MALLINCAM_OPTION_PROCESSMODE            0x03       /* [RW] obsolete & useless, noop. 0 = better image quality, more cpu usage. this is the default value; 1 = lower image quality, less cpu usage */
#define MALLINCAM_OPTION_RAW                    0x04       /* [RW]
                                                            0: RGB mode
                                                            1: RAW mode, read the CMOS or CCD raw data
                                                            -1: RAW mode, the difference from 1 is the execution of FFC, DFC, FPNC, black balance, and white balance
                                                            default value: 0
                                                         */
#define MALLINCAM_OPTION_HISTOGRAM              0x05       /* [RW] 0 = only one, 1 = continue mode */
#define MALLINCAM_OPTION_BITDEPTH               0x06       /* [RW] 0 = 8 bits mode, 1 = 16 bits mode, subset of MALLINCAM_OPTION_PIXEL_FORMAT */
#define MALLINCAM_OPTION_FAN                    0x07       /* [RW] 0 = turn off the cooling fan, [1, max] = fan speed, , set to "-1" means to use default fan speed */
#define MALLINCAM_OPTION_TEC                    0x08       /* [RW] 0 = turn off the thermoelectric cooler, 1 = turn on the thermoelectric cooler */
#define MALLINCAM_OPTION_LINEAR                 0x09       /* [RW] 0 = turn off the builtin linear tone mapping, 1 = turn on the builtin linear tone mapping, default value: 1 */
#define MALLINCAM_OPTION_CURVE                  0x0a       /* [RW] 0 = turn off the builtin curve tone mapping, 1 = turn on the builtin polynomial curve tone mapping, 2 = logarithmic curve tone mapping, default value: 2 */
#define MALLINCAM_OPTION_TRIGGER                0x0b       /* [RW] 0 = video mode, 1 = software or simulated trigger mode, 2 = external trigger mode, 3 = external + software trigger, 4 = self trigger, default value = 0 */
#define MALLINCAM_OPTION_RGB                    0x0c       /* [RW] 0 => RGB24; 1 => enable RGB48 format when bitdepth > 8; 2 => RGB32; 3 => 8 Bits Grey (only for mono camera); 4 => 16 Bits Grey (only for mono camera when bitdepth > 8); 5 => 64(RGB64) */
#define MALLINCAM_OPTION_COLORMATIX             0x0d       /* [RW] enable or disable the builtin color matrix, default value: 1 */
#define MALLINCAM_OPTION_WBGAIN                 0x0e       /* [RW] enable or disable the builtin white balance gain, default value: 1 */
#define MALLINCAM_OPTION_TECTARGET              0x0f       /* [RW] get or set the target temperature of the thermoelectric cooler, in 0.1 degree Celsius. For example, 125 means 12.5 degree Celsius, -35 means -3.5 degree Celsius. Set "-2730" or below means using the default for that model */
#define MALLINCAM_OPTION_AUTOEXP_POLICY         0x10       /* [RW] auto exposure policy:
                                                             0: Exposure Only
                                                             1: Exposure Preferred
                                                             2: Gain Only
                                                             3: Gain Preferred
                                                             default value: 1
                                                         */
#define MALLINCAM_OPTION_FRAMERATE              0x11       /* [RW] limit the frame rate, the default value 0 means no limit */
#define MALLINCAM_OPTION_DEMOSAIC               0x12       /* [RW] demosaic method for both video and still image: BILINEAR = 0, VNG(Variable Number of Gradients) = 1, PPG(Patterned Pixel Grouping) = 2, AHD(Adaptive Homogeneity Directed) = 3, EA(Edge Aware) = 4, see https://en.wikipedia.org/wiki/Demosaicing
                                                              In terms of CPU usage, EA is the lowest, followed by BILINEAR, and the others are higher.
                                                              default value: 0
                                                         */
#define MALLINCAM_OPTION_DEMOSAIC_VIDEO         0x13       /* [RW] demosaic method for video */
#define MALLINCAM_OPTION_DEMOSAIC_STILL         0x14       /* [RW] demosaic method for still image */
#define MALLINCAM_OPTION_BLACKLEVEL             0x15       /* [RW] black level */
#define MALLINCAM_OPTION_MULTITHREAD            0x16       /* [RW] multithread image processing */
#define MALLINCAM_OPTION_BINNING                0x17       /* [RW] binning
                                                                0x01: (no binning)
                                                                n: (saturating add, n*n), 0x02(2*2), 0x03(3*3), 0x04(4*4), 0x05(5*5), 0x06(6*6), 0x07(7*7), 0x08(8*8). The Bitdepth of the data remains unchanged.
                                                                0x40 | n: (unsaturated add, n*n, works only in RAW mode), 0x42(2*2), 0x43(3*3), 0x44(4*4), 0x45(5*5), 0x46(6*6), 0x47(7*7), 0x48(8*8). The Bitdepth of the data is increased. For example, the original data with bitdepth of 12 will increase the bitdepth by 2 bits and become 14 after 2*2 binning.
                                                                0x80 | n: (average, n*n), 0x82(2*2), 0x83(3*3), 0x84(4*4), 0x85(5*5), 0x86(6*6), 0x87(7*7), 0x88(8*8). The Bitdepth of the data remains unchanged.
                                                            The final image size is rounded down to an even number, such as 640/3 to get 212
                                                         */
#define MALLINCAM_OPTION_ROTATE                 0x18       /* [RW] rotate clockwise: 0, 90, 180, 270 */
#define MALLINCAM_OPTION_CG                     0x19       /* [RW] Conversion Gain:
                                                                0 = LCG
                                                                1 = HCG
                                                                2 = HDR (for camera with flag MALLINCAM_FLAG_CGHDR)
                                                                2 = MCG (for camera with flag MALLINCAM_FLAG_GHOPTO)
                                                         */
#define MALLINCAM_OPTION_PIXEL_FORMAT           0x1a       /* [RW] pixel format, MALLINCAM_PIXELFORMAT_xxxx */
#define MALLINCAM_OPTION_FFC                    0x1b       /* [RW] flat field correction
                                                             set:
                                                                  0: disable
                                                                  1: enable
                                                                 -1: reset
                                                                 (0xff000000 | n): set the average number to n, [1~255]
                                                             get:
                                                                  (val & 0xff): 0 => disable, 1 => enable, 2 => inited
                                                                  ((val & 0xff00) >> 8): sequence
                                                                  ((val & 0xff0000) >> 16): average number
                                                         */
#define MALLINCAM_OPTION_DDR_DEPTH              0x1c       /* [RW] the number of the frames that DDR can cache
                                                                 1: DDR cache only one frame
                                                                 0: Auto:
                                                                         => one for video mode when auto exposure is enabled
                                                                         => full capacity for others
                                                                -1: DDR can cache frames to full capacity
                                                         */
#define MALLINCAM_OPTION_DFC                    0x1d       /* [RW] dark field correction
                                                             set:
                                                                 0: disable
                                                                 1: enable
                                                                -1: reset
                                                                 (0xff000000 | n): set the average number to n, [1~255]
                                                             get:
                                                                 (val & 0xff): 0 => disable, 1 => enable, 2 => inited
                                                                 ((val & 0xff00) >> 8): sequence
                                                                 ((val & 0xff0000) >> 16): average number
                                                         */
#define MALLINCAM_OPTION_SHARPENING             0x1e       /* [RW] Sharpening: (threshold << 24) | (radius << 16) | strength)
                                                             strength: [0, 500], default: 0 (disable)
                                                             radius: [1, 10]
                                                             threshold: [0, 255]
                                                         */
#define MALLINCAM_OPTION_FACTORY                0x1f       /* [WO] restore the factory settings */
#define MALLINCAM_OPTION_TEC_VOLTAGE            0x20       /* [RO] get the current TEC voltage in 0.1V, 59 mean 5.9V; readonly */
#define MALLINCAM_OPTION_TEC_VOLTAGE_MAX        0x21       /* [RW] TEC maximum voltage in 0.1V */
#define MALLINCAM_OPTION_DEVICE_RESET           0x22       /* [WO] reset usb device, simulate a replug */
#define MALLINCAM_OPTION_UPSIDE_DOWN            0x23       /* [RW] upsize down:
                                                             1: yes
                                                             0: no
                                                             default: 1 (win), 0 (linux/macos)
                                                         */
#define MALLINCAM_OPTION_FOCUSPOS               0x24       /* [RW] focus positon */
#define MALLINCAM_OPTION_AFMODE                 0x25       /* [RW] auto focus mode, see MallincamAFMode */
#define MALLINCAM_OPTION_AFSTATUS               0x27       /* [RO] auto focus status, see MallincamAFStaus */
#define MALLINCAM_OPTION_TESTPATTERN            0x28       /* [RW] test pattern:
                                                            0: off
                                                            3: monochrome diagonal stripes
                                                            5: monochrome vertical stripes
                                                            7: monochrome horizontal stripes
                                                            9: chromatic diagonal stripes
                                                         */
#define MALLINCAM_OPTION_AUTOEXP_THRESHOLD      0x29       /* [RW] threshold of auto exposure, default value: 5, range = [2, 15] */
#define MALLINCAM_OPTION_BYTEORDER              0x2a       /* [RW] Byte order, BGR or RGB: 0 => RGB, 1 => BGR, default value: 1(Win), 0(macOS, Linux, Android) */
#define MALLINCAM_OPTION_NOPACKET_TIMEOUT       0x2b       /* [RW] no packet timeout: 0 => disable, positive value (>= MALLINCAM_NOPACKET_TIMEOUT_MIN) => timeout milliseconds. default: disable */
#define MALLINCAM_OPTION_MAX_PRECISE_FRAMERATE  0x2c       /* [RO] get the precise frame rate maximum value in 0.1 fps, such as 115 means 11.5 fps */
#define MALLINCAM_OPTION_PRECISE_FRAMERATE      0x2d       /* [RW] precise frame rate current value in 0.1 fps. use MALLINCAM_OPTION_MAX_PRECISE_FRAMERATE, MALLINCAM_OPTION_MIN_PRECISE_FRAMERATE to get the range. if the set value is out of range, E_INVALIDARG will be returned */
#define MALLINCAM_OPTION_BANDWIDTH              0x2e       /* [RW] bandwidth, [1-100]% */
#define MALLINCAM_OPTION_RELOAD                 0x2f       /* [RW] reload the last frame in trigger mode */
#define MALLINCAM_OPTION_CALLBACK_THREAD        0x30       /* [RW] dedicated thread for callback: 0 => disable, 1 => enable, default: 0 */
#define MALLINCAM_OPTION_FRONTEND_DEQUE_LENGTH  0x31       /* [RW] frontend (raw) frame buffer deque length, range: [2, 1024], default: 4
                                                            All the memory will be pre-allocated when the camera starts, so, please attention to memory usage
                                                         */
#define MALLINCAM_OPTION_FRAME_DEQUE_LENGTH     0x31       /* [RW] alias of MALLINCAM_OPTION_FRONTEND_DEQUE_LENGTH */
#define MALLINCAM_OPTION_MIN_PRECISE_FRAMERATE  0x32       /* [RO] get the precise frame rate minimum value in 0.1 fps, such as 15 means 1.5 fps */
#define MALLINCAM_OPTION_SEQUENCER_ONOFF        0x33       /* [RW] sequencer trigger: on/off */
#define MALLINCAM_OPTION_SEQUENCER_NUMBER       0x34       /* [RW] sequencer trigger: number, range = [1, 255] */
#define MALLINCAM_OPTION_SEQUENCER_EXPOTIME     0x01000000 /* [RW] sequencer trigger: exposure time, iOption = MALLINCAM_OPTION_SEQUENCER_EXPOTIME | index, iValue = exposure time
                                                             For example, to set the exposure time of the third group to 50ms, call:
                                                                Mallincam_put_Option(MALLINCAM_OPTION_SEQUENCER_EXPOTIME | 3, 50000)
                                                         */
#define MALLINCAM_OPTION_SEQUENCER_EXPOGAIN     0x02000000 /* [RW] sequencer trigger: exposure gain, iOption = MALLINCAM_OPTION_SEQUENCER_EXPOGAIN | index, iValue = gain */
#define MALLINCAM_OPTION_DENOISE                0x35       /* [RW] denoise, strength range: [0, 100], 0 means disable */
#define MALLINCAM_OPTION_HEAT_MAX               0x36       /* [RO] get maximum level: heat to prevent fogging up */
#define MALLINCAM_OPTION_HEAT                   0x37       /* [RW] heat to prevent fogging up */
#define MALLINCAM_OPTION_LOW_NOISE              0x38       /* [RW] low noise mode (Higher signal noise ratio, lower frame rate): 1 => enable */
#define MALLINCAM_OPTION_POWER                  0x39       /* [RO] get power consumption, unit: milliwatt */
#define MALLINCAM_OPTION_GLOBAL_RESET_MODE      0x3a       /* [RW] global reset mode */
#define MALLINCAM_OPTION_OPEN_ERRORCODE         0x3b       /* [RO] get the open camera error code */
#define MALLINCAM_OPTION_FLUSH                  0x3d       /* [WO]
                                                            1 = hard flush, discard frames cached by camera DDR (if any)
                                                            2 = soft flush, discard frames cached by mallincam.dll (if any)
                                                            3 = both flush
                                                            Mallincam_Flush means 'both flush'
                                                            return the number of soft flushed frames if successful, HRESULT if failed
                                                         */
#define MALLINCAM_OPTION_NUMBER_DROP_FRAME      0x3e       /* [RO] get the number of frames that have been grabbed from the USB but dropped by the software */
#define MALLINCAM_OPTION_DUMP_CFG               0x3f       /* [RW] 0 = when camera is stopped, do not dump configuration automatically
                                                            1 = when camera is stopped, dump configuration automatically
                                                            -1 = explicitly dump configuration once
                                                            default: 1
                                                         */
#define MALLINCAM_OPTION_DEFECT_PIXEL           0x40       /* [RW] Defect Pixel Correction: 0 => disable, 1 => enable; default: 1 */
#define MALLINCAM_OPTION_BACKEND_DEQUE_LENGTH   0x41       /* [RW] backend (pipelined) frame buffer deque length (Only available in pull mode), range: [2, 1024], default: 3
                                                            All the memory will be pre-allocated when the camera starts, so, please attention to memory usage
                                                         */
#define MALLINCAM_OPTION_LIGHTSOURCE_MAX        0x42       /* [RO] get the light source range, [0 ~ max] */
#define MALLINCAM_OPTION_LIGHTSOURCE            0x43       /* [RW] light source */
#define MALLINCAM_OPTION_HEARTBEAT              0x44       /* [RW] Heartbeat interval in millisecond, range = [MALLINCAM_HEARTBEAT_MIN, MALLINCAM_HEARTBEAT_MAX], 0 = disable, default: disable */
#define MALLINCAM_OPTION_FRONTEND_DEQUE_CURRENT 0x45       /* [RO] get the current number in frontend deque */
#define MALLINCAM_OPTION_BACKEND_DEQUE_CURRENT  0x46       /* [RO] get the current number in backend deque */
#define MALLINCAM_OPTION_EVENT_HARDWARE         0x04000000 /* [RW] enable or disable hardware event: 0 => disable, 1 => enable; default: disable
                                                                (1) iOption = MALLINCAM_OPTION_EVENT_HARDWARE, master switch for notification of all hardware events
                                                                (2) iOption = MALLINCAM_OPTION_EVENT_HARDWARE | (event type), a specific type of sub-switch
                                                            Only if both the master switch and the sub-switch of a particular type remain on are actually enabled for that type of event notification.
                                                         */
#define MALLINCAM_OPTION_PACKET_NUMBER          0x47       /* [RO] get the received packet number */
#define MALLINCAM_OPTION_FILTERWHEEL_SLOT       0x48       /* [RW] filter wheel slot number */
#define MALLINCAM_OPTION_FILTERWHEEL_POSITION   0x49       /* [RW] filter wheel position:
                                                                set:
                                                                    -1: reset
                                                                    val & 0xff: position between 0 and N-1, where N is the number of filter slots
                                                                    (val >> 8) & 0x1: direction, 0 => clockwise spinning, 1 => auto direction spinning
                                                                get:
                                                                     -1: in motion
                                                                     val: position arrived
                                                         */
#define MALLINCAM_OPTION_AUTOEXPOSURE_PERCENT   0x4a       /* [RW] auto exposure percent to average:
                                                                1~99: peak percent average
                                                                0 or 100: full roi average, means "disabled"
                                                         */
#define MALLINCAM_OPTION_ANTI_SHUTTER_EFFECT    0x4b       /* [RW] anti shutter effect: 1 => enable, 0 => disable; default: 0 */
#define MALLINCAM_OPTION_CHAMBER_HT             0x4c       /* [RO] get chamber humidity & temperature:
                                                                high 16 bits: humidity, in 0.1%, such as: 325 means humidity is 32.5%
                                                                low 16 bits: temperature, in 0.1 degrees Celsius, such as: 32 means 3.2 degrees Celsius
                                                         */
#define MALLINCAM_OPTION_ENV_HT                 0x4d       /* [RO] get environment humidity & temperature */
#define MALLINCAM_OPTION_EXPOSURE_PRE_DELAY     0x4e       /* [RW] exposure signal pre-delay, microsecond */
#define MALLINCAM_OPTION_EXPOSURE_POST_DELAY    0x4f       /* [RW] exposure signal post-delay, microsecond */
#define MALLINCAM_OPTION_AUTOEXPO_CONV          0x50       /* [RO] get auto exposure convergence status: 1(YES) or 0(NO), -1(PENDING) */
#define MALLINCAM_OPTION_AUTOEXPO_TRIGGER       0x51       /* [RW] auto exposure on trigger mode: 0 => disable, 1 => enable; default: 0 */
#define MALLINCAM_OPTION_LINE_PRE_DELAY         0x52       /* [RW] specified line signal pre-delay, microsecond */
#define MALLINCAM_OPTION_LINE_POST_DELAY        0x53       /* [RW] specified line signal post-delay, microsecond */
#define MALLINCAM_OPTION_TEC_VOLTAGE_MAX_RANGE  0x54       /* [RO] get the tec maximum voltage range:
                                                                high 16 bits: max
                                                                low 16 bits: min
                                                         */
#define MALLINCAM_OPTION_HIGH_FULLWELL          0x55       /* [RW] high fullwell capacity: 0 => disable, 1 => enable */
#define MALLINCAM_OPTION_DYNAMIC_DEFECT         0x56       /* [RW] dynamic defect pixel correction:
                                                                dead pixel ratio, t1: (high 16 bits): [0, 100], means: [0.0, 1.0]
                                                                hot pixel ratio, t2: (low 16 bits): [0, 100], means: [0.0, 1.0]
                                                         */
#define MALLINCAM_OPTION_HDR_KB                 0x57       /* [RW] HDR synthesize
                                                                K (high 16 bits): [1, 25500]
                                                                B (low 16 bits): [0, 65535]
                                                                0xffffffff => set to default
                                                         */
#define MALLINCAM_OPTION_HDR_THRESHOLD          0x58       /* [RW] HDR synthesize
                                                                threshold: [1, 4094]
                                                                0xffffffff => set to default
                                                         */
#define MALLINCAM_OPTION_GIGETIMEOUT            0x5a       /* [RW] For GigE cameras, the application periodically sends heartbeat signals to the camera to keep the connection to the camera alive.
                                                            If the camera doesn't receive heartbeat signals within the time period specified by the heartbeat timeout counter, the camera resets the connection.
                                                            When the application is stopped by the debugger, the application cannot send the heartbeat signals
                                                                0 => auto: when the camera is opened, enable if no debugger is present or disable if debugger is present
                                                                1 => enable
                                                                2 => disable
                                                                default: auto
                                                         */
#define MALLINCAM_OPTION_EEPROM_SIZE            0x5b       /* [RO] get EEPROM size */
#define MALLINCAM_OPTION_OVERCLOCK_MAX          0x5c       /* [RO] get overclock range: [0, max] */
#define MALLINCAM_OPTION_OVERCLOCK              0x5d       /* [RW] overclock, default: 0 */
#define MALLINCAM_OPTION_RESET_SENSOR           0x5e       /* [WO] reset sensor */
#define MALLINCAM_OPTION_ISP                    0x5f       /* [RW] Enable hardware ISP: 0 => auto (disable in RAW mode, otherwise enable), 1 => enable, -1 => disable; default: 0 */
#define MALLINCAM_OPTION_AUTOEXP_EXPOTIME_DAMP  0x60       /* [RW] Auto exposure damping coefficient: time (thousandths). The larger the damping coefficient, the smoother and slower the exposure time changes */
#define MALLINCAM_OPTION_AUTOEXP_GAIN_DAMP      0x61       /* [RW] Auto exposure damping coefficient: gain (thousandths). The larger the damping coefficient, the smoother and slower the gain changes */
#define MALLINCAM_OPTION_MOTOR_NUMBER           0x62       /* [RW] range: [1, 20] */
#define MALLINCAM_OPTION_MOTOR_POS              0x10000000 /* [RW] range: [1, 702] */
#define MALLINCAM_OPTION_PSEUDO_COLOR_START     0x63       /* [RW] Pseudo: start color, BGR format */
#define MALLINCAM_OPTION_PSEUDO_COLOR_END       0x64       /* [RW] Pseudo: end color, BGR format */
#define MALLINCAM_OPTION_PSEUDO_COLOR_ENABLE    0x65       /* [RW] Pseudo: -1 => custom: use startcolor & endcolor to generate the colormap
                                                                    0 => disable
                                                                    1 => spot
                                                                    2 => spring
                                                                    3 => summer
                                                                    4 => autumn
                                                                    5 => winter
                                                                    6 => bone
                                                                    7 => jet
                                                                    8 => rainbow
                                                                    9 => deepgreen
                                                                    10 => ocean
                                                                    11 => cool
                                                                    12 => hsv
                                                                    13 => pink
                                                                    14 => hot
                                                                    15 => parula
                                                                    16 => magma
                                                                    17 => inferno
                                                                    18 => plasma
                                                                    19 => viridis
                                                                    20 => cividis
                                                                    21 => twilight
                                                                    22 => twilight_shifted
                                                                    23 => turbo
                                                                    24 => red
                                                                    25 => green
                                                                    26 => blue
                                                         */
#define MALLINCAM_OPTION_LOW_POWERCONSUMPTION   0x66       /* [RW] Low Power Consumption: 0 => disable, 1 => enable */
#define MALLINCAM_OPTION_FPNC                   0x67       /* [RW] Fix Pattern Noise Correction
                                                             set:
                                                                 0: disable
                                                                 1: enable
                                                                -1: reset
                                                                 (0xff000000 | n): set the average number to n, [1~255]
                                                             get:
                                                                 (val & 0xff): 0 => disable, 1 => enable, 2 => inited
                                                                 ((val & 0xff00) >> 8): sequence
                                                                 ((val & 0xff0000) >> 16): average number
                                                         */
#define MALLINCAM_OPTION_OVEREXP_POLICY         0x68       /* [RW] Auto exposure over exposure policy: when overexposed,
                                                                0 => directly reduce the exposure time/gain to the minimum value; or
                                                                1 => reduce exposure time/gain in proportion to current and target brightness.
                                                                n(n>1) => first adjust the exposure time to (maximum automatic exposure time * maximum automatic exposure gain) * n / 1000, and then adjust according to the strategy of 1
                                                            The advantage of policy 0 is that the convergence speed is faster, but there is black screen.
                                                            Policy 1 avoids the black screen, but the convergence speed is slower.
                                                            Default: 0
                                                         */
#define MALLINCAM_OPTION_READOUT_MODE           0x69       /* [RW] Readout mode: 0 = IWR (Integrate While Read), 1 = ITR (Integrate Then Read)
                                                              The working modes of the detector readout circuit can be divided into two types: ITR and IWR. Using the IWR readout mode can greatly increase the frame rate. In the ITR mode, the integration of the (n+1)th frame starts after all the data of the nth frame are read out, while in the IWR mode, the data of the nth frame is read out at the same time when the (n+1)th frame is integrated
                                                         */
#define MALLINCAM_OPTION_TAILLIGHT              0x6a       /* [RW] Turn on/off tail Led light: 0 => off, 1 => on; default: on */
#define MALLINCAM_OPTION_LENSSTATE              0x6b       /* [RW] Load/Save lens state to EEPROM: 0 => load, 1 => save */
#define MALLINCAM_OPTION_AWB_CONTINUOUS         0x6c       /* [RW] Auto White Balance: continuous mode
                                                                0:  disable (default)
                                                                n>0: every n millisecond(s)
                                                                n<0: every -n frame
                                                         */
#define MALLINCAM_OPTION_TECTARGET_RANGE        0x6d       /* [RO] TEC target range: min(low 16 bits) = (short)(val & 0xffff), max(high 16 bits) = (short)((val >> 16) & 0xffff) */
#define MALLINCAM_OPTION_CDS                    0x6e       /* [RW] Correlated Double Sampling: 0~100 */
#define MALLINCAM_OPTION_LOW_POWER_EXPOTIME     0x6f       /* [RW] Low Power Consumption: Enable if exposure time is greater than the set value */
#define MALLINCAM_OPTION_ZERO_OFFSET            0x70       /* [RW] Sensor output offset to zero: 0 => disable, 1 => eanble; default: 0 */
#define MALLINCAM_OPTION_GVCP_TIMEOUT           0x71       /* [RW] GVCP Timeout: millisecond, range = [3, 75], default: 15
                                                              Unless in very special circumstances, generally no modification is required, just use the default value
                                                         */
#define MALLINCAM_OPTION_GVCP_RETRY             0x72       /* [RW] GVCP Retry: range = [2, 8], default: 4
                                                              Unless in very special circumstances, generally no modification is required, just use the default value
                                                         */
#define MALLINCAM_OPTION_GVSP_WAIT_PERCENT      0x73       /* [RW] GVSP wait percent: range = [0, 100], default = (trigger mode: 100, realtime: 0, other: 1) */
#define MALLINCAM_OPTION_RESET_SEQ_TIMESTAMP    0x74       /* [WO] Reset to 0: 1 => seq; 2 => timestamp; 3 => both */
#define MALLINCAM_OPTION_TRIGGER_CANCEL_MODE    0x75       /* [RW] Trigger cancel mode: 0 => no frame, 1 => output frame; default: 0 */
#define MALLINCAM_OPTION_MECHANICALSHUTTER      0x76       /* [RW] Mechanical shutter: 0 => open, 1 => close; default: 0 */
#define MALLINCAM_OPTION_LINE_TIME              0x77       /* [RO] Line-time of sensor in nanosecond */
#define MALLINCAM_OPTION_ZERO_PADDING           0x78       /* [RW] Zero padding: 0 => high, 1 => low; default: 0 */
#define MALLINCAM_OPTION_UPTIME                 0x79       /* [RO] device uptime in millisecond */
#define MALLINCAM_OPTION_BITRANGE               0x7a       /* [RW] Bit range: [0, 8] */
#define MALLINCAM_OPTION_MODE_SEQ_TIMESTAMP     0x7b       /* [RW] Mode of seq & timestamp: 0 => reset to 0 automatically; 1 => never reset automatically; default: 0 */
#define TOUPCAP_OPTION_TIMED_TRIGGER_NUM      0x7c       /* [RW] Timed trigger number */
#define MALLINCAM_OPTION_TIMED_TRIGGER_LOW      0x20000000 /* [RW] Timed trigger: lower 32 bits of 64-bit integer, nanosecond since epoch (00:00:00 UTC on Thursday, 1 January 1970, see https://en.wikipedia.org/wiki/Unix_time) */
#define MALLINCAM_OPTION_TIMED_TRIGGER_HIGH     0x40000000 /* [RW] Timed trigger: high 32 bits. The lower 32 bits must be set first, followed by the higher 32 bits */
#define MALLINCAM_OPTION_AUTOEXP_THLD_TRIGGER   0x7d       /* [RW] trigger threshold of auto exposure */
#define MALLINCAM_OPTION_LANE                   0x7e       /* [RW] */
#define MALLINCAM_OPTION_VOLTAGEBIAS            0x7f       /* [RW] Voltage bias */
#define MALLINCAM_OPTION_VOLTAGEBIAS_RANGE      0x80       /* [RO] Voltage bias range: min (low 16bits), max (high 16bits) */
#define MALLINCAM_OPTION_READ_TIME              0x81       /* [RO] */
#define MALLINCAM_OPTION_ALL_EXPO_TIME          0x82       /* [RO] */
#define MALLINCAM_OPTION_CAMERA_PERIOD          0x83       /* [RO] */
#define MALLINCAM_OPTION_FRONTEND_FULL          0x84       /* [RO] get the number of frontend deque full */
#define MALLINCAM_OPTION_BACKEND_FULL           0x85       /* [RO] get the number of backend deque full */
#define MALLINCAM_OPTION_GPS                    0x86       /* [RO] gps status: 0 => not supported; -1 => gps device offline; 1 => gps device online */
#define MALLINCAM_OPTION_LINE_LENGTH            0x87       /* [RW] Line length in pixel clock */
#define MALLINCAM_OPTION_SCAN_DIRECTION         0x88       /* [RW] Scan direction: 0 (forward), 1(reverse), 2(alternate) */

/* pixel format */
#define MALLINCAM_PIXELFORMAT_RAW8              0x00
#define MALLINCAM_PIXELFORMAT_RAW10             0x01
#define MALLINCAM_PIXELFORMAT_RAW12             0x02
#define MALLINCAM_PIXELFORMAT_RAW14             0x03
#define MALLINCAM_PIXELFORMAT_RAW16             0x04
#define MALLINCAM_PIXELFORMAT_YUV411            0x05
#define MALLINCAM_PIXELFORMAT_VUYY              0x06
#define MALLINCAM_PIXELFORMAT_YUV444            0x07
#define MALLINCAM_PIXELFORMAT_RGB888            0x08
#define MALLINCAM_PIXELFORMAT_GMCY8             0x09   /* map to RGGB 8 bits */
#define MALLINCAM_PIXELFORMAT_GMCY12            0x0a   /* map to RGGB 12 bits */
#define MALLINCAM_PIXELFORMAT_UYVY              0x0b
#define MALLINCAM_PIXELFORMAT_RAW12PACK         0x0c
#define MALLINCAM_PIXELFORMAT_RAW11             0x0d
#define MALLINCAM_PIXELFORMAT_HDR8HL            0x0e   /* HDR, Bitdepth: 8, Conversion Gain: High + Low */
#define MALLINCAM_PIXELFORMAT_HDR10HL           0x0f   /* HDR, Bitdepth: 10, Conversion Gain: High + Low */
#define MALLINCAM_PIXELFORMAT_HDR11HL           0x10   /* HDR, Bitdepth: 11, Conversion Gain: High + Low */
#define MALLINCAM_PIXELFORMAT_HDR12HL           0x11   /* HDR, Bitdepth: 12, Conversion Gain: High + Low */
#define MALLINCAM_PIXELFORMAT_HDR14HL           0x12   /* HDR, Bitdepth: 14, Conversion Gain: High + Low */
#define MALLINCAM_PIXELFORMAT_RAW10PACK         0x13

/*
* cmd: input
*    -1:       query the number
*    0~number: query the nth pixel format
* pixelFormat: output, MALLINCAM_PIXELFORMAT_xxxx
*/
MALLINCAM_API(HRESULT)     Mallincam_get_PixelFormatSupport(HMallincam h, char cmd, int* pixelFormat);

/*
* pixelFormat: MALLINCAM_PIXELFORMAT_XXXX
*/
MALLINCAM_API(const char*) Mallincam_get_PixelFormatName(int pixelFormat);

/*
    xOffset, yOffset, xWidth, yHeight: must be even numbers
*/
MALLINCAM_API(HRESULT)  Mallincam_put_Roi(HMallincam h, unsigned xOffset, unsigned yOffset, unsigned xWidth, unsigned yHeight);
MALLINCAM_API(HRESULT)  Mallincam_get_Roi(HMallincam h, unsigned* pxOffset, unsigned* pyOffset, unsigned* pxWidth, unsigned* pyHeight);

/* multiple Roi */
MALLINCAM_API(HRESULT)  Mallincam_put_RoiN(HMallincam h, unsigned xOffset[], unsigned yOffset[], unsigned xWidth[], unsigned yHeight[], unsigned Num);

/* Hardware Binning
* Value: 1x1, 2x2, etc
* Method: Average, Add, Skip
*/
MALLINCAM_API(HRESULT)  Mallincam_put_Binning(HMallincam h, const char* pValue, const char* pMethod);
MALLINCAM_API(HRESULT)  Mallincam_get_Binning(HMallincam h, const char** ppValue, const char** ppMethod);
MALLINCAM_API(HRESULT)  Mallincam_get_BinningNumber(HMallincam h);
MALLINCAM_API(HRESULT)  Mallincam_get_BinningValue(HMallincam h, unsigned index, const char** ppValue);
MALLINCAM_API(HRESULT)  Mallincam_get_BinningMethod(HMallincam h, unsigned index, const char** ppMethod);

MALLINCAM_API(HRESULT)  Mallincam_put_XY(HMallincam h, int x, int y);

#define MALLINCAM_IOCONTROLTYPE_GET_SUPPORTEDMODE            0x01 /* 0x01 => Input, 0x02 => Output, (0x01 | 0x02) => support both Input and Output */
#define MALLINCAM_IOCONTROLTYPE_GET_GPIODIR                  0x03 /* 0x00 => Input, 0x01 => Output */
#define MALLINCAM_IOCONTROLTYPE_SET_GPIODIR                  0x04
#define MALLINCAM_IOCONTROLTYPE_GET_FORMAT                   0x05 /*
                                                                    0x00 => not connected
                                                                    0x01 => Tri-state: Tri-state mode (Not driven)
                                                                    0x02 => TTL: TTL level signals
                                                                    0x03 => LVDS: LVDS level signals
                                                                    0x04 => RS422: RS422 level signals
                                                                    0x05 => Opto-coupled
                                                                */
#define MALLINCAM_IOCONTROLTYPE_SET_FORMAT                   0x06
#define MALLINCAM_IOCONTROLTYPE_GET_OUTPUTINVERTER           0x07 /* boolean, only support output signal */
#define MALLINCAM_IOCONTROLTYPE_SET_OUTPUTINVERTER           0x08
#define MALLINCAM_IOCONTROLTYPE_GET_INPUTACTIVATION          0x09 /* 0x00 => Rising edge, 0x01 => Falling edge, 0x02 => Level high, 0x03 => Level low */
#define MALLINCAM_IOCONTROLTYPE_SET_INPUTACTIVATION          0x0a
#define MALLINCAM_IOCONTROLTYPE_GET_DEBOUNCERTIME            0x0b /* debouncer time in microseconds, range: [0, 20000] */
#define MALLINCAM_IOCONTROLTYPE_SET_DEBOUNCERTIME            0x0c
#define MALLINCAM_IOCONTROLTYPE_GET_TRIGGERSOURCE            0x0d /*
                                                                   0x00 => Opto-isolated input
                                                                   0x01 => GPIO0
                                                                   0x02 => GPIO1
                                                                   0x03 => Counter
                                                                   0x04 => PWM
                                                                   0x05 => Software
                                                                */
#define MALLINCAM_IOCONTROLTYPE_SET_TRIGGERSOURCE            0x0e
#define MALLINCAM_IOCONTROLTYPE_GET_TRIGGERDELAY             0x0f /* Trigger delay time in microseconds, range: [0, 5000000] */
#define MALLINCAM_IOCONTROLTYPE_SET_TRIGGERDELAY             0x10
#define MALLINCAM_IOCONTROLTYPE_GET_BURSTCOUNTER             0x11 /* Burst Counter, range: [1 ~ 65535] */
#define MALLINCAM_IOCONTROLTYPE_SET_BURSTCOUNTER             0x12
#define MALLINCAM_IOCONTROLTYPE_GET_COUNTERSOURCE            0x13 /* 0x00 => Opto-isolated input, 0x01 => GPIO0, 0x02 => GPIO1 */
#define MALLINCAM_IOCONTROLTYPE_SET_COUNTERSOURCE            0x14
#define MALLINCAM_IOCONTROLTYPE_GET_COUNTERVALUE             0x15 /* Counter Value, range: [1 ~ 65535] */
#define MALLINCAM_IOCONTROLTYPE_SET_COUNTERVALUE             0x16
#define MALLINCAM_IOCONTROLTYPE_SET_RESETCOUNTER             0x18
#define MALLINCAM_IOCONTROLTYPE_GET_PWM_FREQ                 0x19 /* PWM Frequency */
#define MALLINCAM_IOCONTROLTYPE_SET_PWM_FREQ                 0x1a
#define MALLINCAM_IOCONTROLTYPE_GET_PWM_DUTYRATIO            0x1b /* PWM Duty Ratio */
#define MALLINCAM_IOCONTROLTYPE_SET_PWM_DUTYRATIO            0x1c
#define MALLINCAM_IOCONTROLTYPE_GET_PWMSOURCE                0x1d /* PWM Source: 0x00 => Opto-isolated input, 0x01 => GPIO0, 0x02 => GPIO1 */
#define MALLINCAM_IOCONTROLTYPE_SET_PWMSOURCE                0x1e
#define MALLINCAM_IOCONTROLTYPE_GET_OUTPUTMODE               0x1f /*
                                                                   0x00 => Frame Trigger Wait
                                                                   0x01 => Exposure Active
                                                                   0x02 => Strobe
                                                                   0x03 => User output
                                                                   0x04 => Counter Output
                                                                   0x05 => Timer Output
                                                                */
#define MALLINCAM_IOCONTROLTYPE_SET_OUTPUTMODE               0x20
#define MALLINCAM_IOCONTROLTYPE_GET_STROBEDELAYMODE          0x21 /* boolean, 0 => pre-delay, 1 => delay; compared to exposure active signal */
#define MALLINCAM_IOCONTROLTYPE_SET_STROBEDELAYMODE          0x22
#define MALLINCAM_IOCONTROLTYPE_GET_STROBEDELAYTIME          0x23 /* Strobe delay or pre-delay time in microseconds, range: [0, 5000000] */
#define MALLINCAM_IOCONTROLTYPE_SET_STROBEDELAYTIME          0x24
#define MALLINCAM_IOCONTROLTYPE_GET_STROBEDURATION           0x25 /* Strobe duration time in microseconds, range: [0, 5000000] */
#define MALLINCAM_IOCONTROLTYPE_SET_STROBEDURATION           0x26
#define MALLINCAM_IOCONTROLTYPE_GET_USERVALUE                0x27 /*
                                                                   bit0 => Opto-isolated output
                                                                   bit1 => GPIO0 output
                                                                   bit2 => GPIO1 output
                                                                */
#define MALLINCAM_IOCONTROLTYPE_SET_USERVALUE                0x28
#define MALLINCAM_IOCONTROLTYPE_GET_UART_ENABLE              0x29 /* enable: 1 => on; 0 => off */
#define MALLINCAM_IOCONTROLTYPE_SET_UART_ENABLE              0x2a
#define MALLINCAM_IOCONTROLTYPE_GET_UART_BAUDRATE            0x2b /* baud rate: 0 => 9600; 1 => 19200; 2 => 38400; 3 => 57600; 4 => 115200 */
#define MALLINCAM_IOCONTROLTYPE_SET_UART_BAUDRATE            0x2c
#define MALLINCAM_IOCONTROLTYPE_GET_UART_LINEMODE            0x2d /* line mode: 0 => TX(GPIO_0)/RX(GPIO_1); 1 => TX(GPIO_1)/RX(GPIO_0) */
#define MALLINCAM_IOCONTROLTYPE_SET_UART_LINEMODE            0x2e
#define MALLINCAM_IOCONTROLTYPE_GET_EXPO_ACTIVE_MODE         0x2f /* exposure time signal: 0 => specified line, 1 => common exposure time */
#define MALLINCAM_IOCONTROLTYPE_SET_EXPO_ACTIVE_MODE         0x30
#define MALLINCAM_IOCONTROLTYPE_GET_EXPO_START_LINE          0x31 /* exposure start line, default: 0 */
#define MALLINCAM_IOCONTROLTYPE_SET_EXPO_START_LINE          0x32
#define MALLINCAM_IOCONTROLTYPE_GET_EXPO_END_LINE            0x33 /* exposure end line, default: 0
                                                                   end line must be no less than start line
                                                                */
#define MALLINCAM_IOCONTROLTYPE_SET_EXPO_END_LINE            0x34
#define MALLINCAM_IOCONTROLTYPE_GET_EXEVT_ACTIVE_MODE        0x35 /* exposure event: 0 => specified line, 1 => common exposure time */
#define MALLINCAM_IOCONTROLTYPE_SET_EXEVT_ACTIVE_MODE        0x36
#define MALLINCAM_IOCONTROLTYPE_GET_OUTPUTCOUNTERVALUE       0x37 /* Output Counter Value, range: [0 ~ 65535] */
#define MALLINCAM_IOCONTROLTYPE_SET_OUTPUTCOUNTERVALUE       0x38
#define MALLINCAM_IOCONTROLTYPE_SET_OUTPUT_PAUSE             0x3a /* Output pause: 1 => puase, 0 => unpause */
#define MALLINCAM_IOCONTROLTYPE_GET_INPUT_STATE              0x3b /* Input state: 0 (low level) or 1 (high level) */
#define MALLINCAM_IOCONTROLTYPE_GET_USER_PULSE_HIGH          0x3d /* User pulse high level time: us */
#define MALLINCAM_IOCONTROLTYPE_SET_USER_PULSE_HIGH          0x3e
#define MALLINCAM_IOCONTROLTYPE_GET_USER_PULSE_LOW           0x3f /* User pulse low level time: us */
#define MALLINCAM_IOCONTROLTYPE_SET_USER_PULSE_LOW           0x40
#define MALLINCAM_IOCONTROLTYPE_GET_USER_PULSE_NUMBER        0x41 /* User pulse number: default 0 */
#define MALLINCAM_IOCONTROLTYPE_SET_USER_PULSE_NUMBER        0x42
#define MALLINCAM_IOCONTROLTYPE_GET_EXTERNAL_TRIGGER_NUMBER  0x43 /* External trigger number */
#define MALLINCAM_IOCONTROLTYPE_GET_DEBOUNCER_TRIGGER_NUMBER 0x45 /* Trigger signal number after debounce */
#define MALLINCAM_IOCONTROLTYPE_GET_EFFECTIVE_TRIGGER_NUMBER 0x47 /* Effective trigger signal number */

#define MALLINCAM_IOCONTROL_DELAYTIME_MAX                    (5 * 1000 * 1000)

/*
  ioLine:
    0 => Opto-isolated input
    1 => Opto-isolated output
    2 => GPIO0
    3 => GPIO1
*/
MALLINCAM_API(HRESULT)  Mallincam_IoControl(HMallincam h, unsigned ioLine, unsigned nType, int outVal, int* inVal);

#ifndef __MALLINCAMSELFTRIGGER_DEFINED__
#define __MALLINCAMSELFTRIGGER_DEFINED__
typedef struct {
    unsigned sensingLeft, sensingTop, sensingWidth, sensingHeight; /* Sensing Area */
    unsigned hThreshold, lThreshold; /* threshold High side, threshold Low side */
    unsigned expoTime; /* Exposure Time */
    unsigned short expoGain; /* Exposure Gain */
    unsigned short hCount, lCount; /* Count threshold High side, Count threshold Low side, thousandths of Sensing Area */
    unsigned short reserved;
} MallincamSelfTrigger;
#endif
MALLINCAM_API(HRESULT)  Mallincam_put_SelfTrigger(HMallincam h, const MallincamSelfTrigger* pSt);
MALLINCAM_API(HRESULT)  Mallincam_get_SelfTrigger(HMallincam h, MallincamSelfTrigger* pSt);

#define MALLINCAM_FLASH_SIZE      0x00    /* query total size */
#define MALLINCAM_FLASH_EBLOCK    0x01    /* query erase block size */
#define MALLINCAM_FLASH_RWBLOCK   0x02    /* query read/write block size */
#define MALLINCAM_FLASH_STATUS    0x03    /* query status */
#define MALLINCAM_FLASH_READ      0x04    /* read */
#define MALLINCAM_FLASH_WRITE     0x05    /* write */
#define MALLINCAM_FLASH_ERASE     0x06    /* erase */
/* Flash:
 action = MALLINCAM_FLASH_XXXX: read, write, erase, query total size, query read/write block size, query erase block size
 addr = address
 see democpp
*/
MALLINCAM_API(HRESULT)  Mallincam_rwc_Flash(HMallincam h, unsigned action, unsigned addr, unsigned len, void* pData);

MALLINCAM_API(HRESULT)  Mallincam_write_UART(HMallincam h, const unsigned char* pData, unsigned nDataLen);
MALLINCAM_API(HRESULT)  Mallincam_read_UART(HMallincam h, unsigned char* pBuffer, unsigned nBufferLen);

/* Initialize support for GigE cameras. If online/offline notifications are not required, the callback function can be set to NULL */
typedef void (__stdcall* PMALLINCAM_HOTPLUG)(void* ctxHotPlug);
MALLINCAM_API(HRESULT)  Mallincam_GigeEnable(PMALLINCAM_HOTPLUG funHotPlug, void* ctxHotPlug);

/* Initialize support for PCIe cameras. If online/offline notifications are not required, the callback function can be set to NULL */
MALLINCAM_API(HRESULT)  Mallincam_PciEnable(PMALLINCAM_HOTPLUG funHotPlug, void* ctxHotPlug);

/* Initialize support for *.cti cameras. If online/offline notifications are not required, the callback function can be set to NULL
* (1) ctiPath = NULL means all *.cti in GENICAM_GENTL64_PATH/GENICAM_GENTL32_PATH
* or
* (2) ctiPath[] = {
            "/full/path/to/file.cti"
            ...
            NULLL
        }
*/
#if defined(_WIN32)
MALLINCAM_API(HRESULT)  Mallincam_CtiEnable(PMALLINCAM_HOTPLUG funHotPlug, void* ctxHotPlug, const wchar_t* ctiPath[]);
#else
MALLINCAM_API(HRESULT)  Mallincam_CtiEnable(PMALLINCAM_HOTPLUG funHotPlug, void* ctxHotPlug, const char* ctiPath[]);
#endif

/*
 filePath:
    "*": export to EEPROM
    "0x????" or "0X????": export to EEPROM specified address
    file path: export to file in ini format
*/
MALLINCAM_API(HRESULT)  Mallincam_export_Cfg(HMallincam h, const char* filePath);

/*
This function is only available on macOS and Linux, it's unnecessary on Windows & Android. To process the device plug in / pull out:
  (1) On Windows, please refer to the MSDN
       (a) Device Management, https://docs.microsoft.com/en-us/windows/win32/devio/device-management
       (b) Detecting Media Insertion or Removal, https://docs.microsoft.com/en-us/windows/win32/devio/detecting-media-insertion-or-removal
  (2) On Android, please refer to https://developer.android.com/guide/topics/connectivity/usb/host
  (3) On Linux / macOS, please call this function to register the callback function.
      When the device is inserted or pulled out, you will be notified by the callback funcion, and then call Mallincam_EnumV2(...) again to enum the cameras.
  (4) On macOS, IONotificationPortCreate series APIs can also be used as an alternative.
Recommendation: for better rubustness, when notify of device insertion arrives, don't open handle of this device immediately, but open it after delaying a short time (e.g., 200 milliseconds).
*/
#if !defined(_WIN32) && !defined(__ANDROID__)
MALLINCAM_API(void)   Mallincam_HotPlug(PMALLINCAM_HOTPLUG funHotPlug, void* ctxHotPlug);
#endif

MALLINCAM_API(unsigned) Mallincam_EnumWithName(MallincamDeviceV2 pti[MALLINCAM_MAX]);
MALLINCAM_API(HRESULT)  Mallincam_set_Name(HMallincam h, const char* name);
MALLINCAM_API(HRESULT)  Mallincam_query_Name(HMallincam h, char name[64]);
#if defined(_WIN32)
MALLINCAM_API(HRESULT)  Mallincam_put_Name(const wchar_t* camId, const char* name);
MALLINCAM_API(HRESULT)  Mallincam_get_Name(const wchar_t* camId, char name[64]);
#else
MALLINCAM_API(HRESULT)  Mallincam_put_Name(const char* camId, const char* name);
MALLINCAM_API(HRESULT)  Mallincam_get_Name(const char* camId, char name[64]);
#endif

typedef struct {
    unsigned short lensID;
    unsigned char  lensType;
    unsigned char  statusAfmf;      /* LENS_AF = 0x00,  LENS_MF = 0x80 */

    unsigned short maxFocalLength;
    unsigned short curFocalLength;
    unsigned short minFocalLength;

    short          farFM;           /* focus motor, absolute value */
    short          curFM;           /* current focus motor */
    short          nearFM;

    unsigned short maxFocusDistance;
    unsigned short minFocusDistance;

    char           curAM;
    unsigned char  maxAM;           /* maximum Aperture, mimimum F# */
    unsigned char  minAM;           /* mimimum Aperture, maximum F# */
    unsigned char  posAM;           /* used for set aperture motor to posAM, it is an index */
    int            posFM;           /* used for set focus motor to posFM */

    unsigned       sizeFN;
    const char**   arrayFN;
    const char*    lensName;        /* lens Name */
} MallincamLensInfo;

MALLINCAM_API(HRESULT)  Mallincam_get_LensInfo(HMallincam h, MallincamLensInfo* pInfo);

typedef enum
{
    MallincamAFMode_CALIBRATE = 0x0,/* lens calibration mode */
    MallincamAFMode_MANUAL    = 0x1,/* manual focus mode */
    MallincamAFMode_ONCE      = 0x2,/* onepush focus mode */
    MallincamAFMode_AUTO      = 0x3,/* autofocus mode */
    MallincamAFMode_NONE      = 0x4,/* no active selection of focus mode */
    MallincamAFMode_IDLE      = 0x5,
    MallincamAFMode_UNUSED    = 0xffffffff
} MallincamAFMode;

typedef enum
{
    MallincamAFStatus_NA           = 0x0,/* Not available */
    MallincamAFStatus_PEAKPOINT    = 0x1,/* Focus completed, find the focus position */
    MallincamAFStatus_DEFOCUS      = 0x2,/* End of focus, defocus */
    MallincamAFStatus_NEAR         = 0x3,/* Focusing ended, object too close */
    MallincamAFStatus_FAR          = 0x4,/* Focusing ended, object too far */
    MallincamAFStatus_ROICHANGED   = 0x5,/* Focusing ends, roi changes */
    MallincamAFStatus_SCENECHANGED = 0x6,/* Focusing ends, scene changes */
    MallincamAFStatus_MODECHANGED  = 0x7,/* The end of focusing and the change in focusing mode is usually determined by the user moderator */
    MallincamAFStatus_UNFINISH     = 0x8,/* The focus is not complete. At the beginning of focusing, it will be set as incomplete */
    MallincamAfStatus_UNUSED       = 0xffffffff
} MallincamAFStatus;/* Focus Status */

typedef struct {
    MallincamAFMode    AF_Mode;
    MallincamAFStatus  AF_Status;
    unsigned char    AF_LensAP_Update_Flag;  /* mark for whether the lens aperture is calibrated */
    unsigned char    Reserved[3];
} MallincamAFState;

MALLINCAM_API(HRESULT)  Mallincam_get_AFState(HMallincam h, MallincamAFState* pState);

MALLINCAM_API(HRESULT)  Mallincam_put_AFMode(HMallincam h, MallincamAFMode mode, int bFixedWD, unsigned uiNear, unsigned uiFar);
MALLINCAM_API(HRESULT)  Mallincam_put_AFRoi(HMallincam h, unsigned xOffset, unsigned yOffset, unsigned xWidth, unsigned yHeight);
MALLINCAM_API(HRESULT)  Mallincam_get_AFRoi(HMallincam h, unsigned* pxOffset, unsigned* pyOffset, unsigned* pxWidth, unsigned* pyHeight);
MALLINCAM_API(HRESULT)  Mallincam_put_AFAperture(HMallincam h, int iAperture);
MALLINCAM_API(HRESULT)  Mallincam_put_AFFMPos(HMallincam h, int iFMPos);

/*  simulate replug:
    return > 0, the number of device has been replug
    return = 0, no device found
    return E_ACCESSDENIED if without UAC Administrator privileges
    for each device found, it will take about 3 seconds
*/
#if defined(_WIN32)
MALLINCAM_API(HRESULT) Mallincam_Replug(const wchar_t* camId);
MALLINCAM_API(HRESULT) Mallincam_Enable(const wchar_t* camId, int enable); /* 1 => enable, 0 => disable */
#else
MALLINCAM_API(HRESULT) Mallincam_Replug(const char* camId);
MALLINCAM_API(HRESULT) Mallincam_Enable(const char* camId, int enable); /* 1 => enable, 0 => disable */
#endif

MALLINCAM_API(const MallincamModelV2**) Mallincam_all_Model(); /* return all supported USB model array */
MALLINCAM_API(const MallincamModelV2*) Mallincam_query_Model(HMallincam h);
MALLINCAM_API(const MallincamModelV2*) Mallincam_get_Model(unsigned short idVendor, unsigned short idProduct);

/* firmware update:
    camId: camera ID
    filePath: ufw file full path
    funProgress, ctx: progress percent callback
Please do not unplug the camera or lost power during the upgrade process, this is very very important.
Once an unplugging or power outage occurs during the upgrade process, the camera will no longer be available and can only be returned to the factory for repair.
*/
#if defined(_WIN32)
MALLINCAM_API(HRESULT)  Mallincam_Update(const wchar_t* camId, const wchar_t* filePath, PIMALLINCAM_PROGRESS funProgress, void* ctxProgress);
#else
MALLINCAM_API(HRESULT)  Mallincam_Update(const char* camId, const char* filePath, PIMALLINCAM_PROGRESS funProgress, void* ctxProgress);
#endif

MALLINCAM_API(HRESULT)  Mallincam_put_Linear(HMallincam h, const unsigned char* v8, const unsigned short* v16); /* v8, v16 pointer must remains valid while camera running */
MALLINCAM_API(HRESULT)  Mallincam_put_Curve(HMallincam h, const unsigned char* v8, const unsigned short* v16); /* v8, v16 pointer must remains valid while camera running */
MALLINCAM_API(HRESULT)  Mallincam_put_ColorMatrix(HMallincam h, const double v[9]); /* null => revert to model default */
MALLINCAM_API(HRESULT)  Mallincam_put_InitWBGain(HMallincam h, const unsigned short v[3]); /* null => revert to model default */

/*
    get the actual frame rate of the camera at the most recent time (about a few seconds):
    framerate (fps) = nFrame * 1000.0 / nTime
*/
MALLINCAM_API(HRESULT)  Mallincam_get_FrameRate(HMallincam h, unsigned* nFrame, unsigned* nTime, unsigned* nTotalFrame);

/* AAF: Astro Auto Focuser */
#define MALLINCAM_AAF_SETPOSITION     0x01
#define MALLINCAM_AAF_GETPOSITION     0x02
#define MALLINCAM_AAF_SETZERO         0x03
#define MALLINCAM_AAF_SETDIRECTION    0x05
#define MALLINCAM_AAF_GETDIRECTION    0x06
#define MALLINCAM_AAF_SETMAXINCREMENT 0x07
#define MALLINCAM_AAF_GETMAXINCREMENT 0x08
#define MALLINCAM_AAF_SETFINE         0x09
#define MALLINCAM_AAF_GETFINE         0x0a
#define MALLINCAM_AAF_SETCOARSE       0x0b
#define MALLINCAM_AAF_GETCOARSE       0x0c
#define MALLINCAM_AAF_SETBUZZER       0x0d
#define MALLINCAM_AAF_GETBUZZER       0x0e
#define MALLINCAM_AAF_SETBACKLASH     0x0f
#define MALLINCAM_AAF_GETBACKLASH     0x10
#define MALLINCAM_AAF_GETAMBIENTTEMP  0x12
#define MALLINCAM_AAF_GETTEMP         0x14  /* in 0.1 degrees Celsius, such as: 32 means 3.2 degrees Celsius */
#define MALLINCAM_AAF_ISMOVING        0x16
#define MALLINCAM_AAF_HALT            0x17
#define MALLINCAM_AAF_SETMAXSTEP      0x1b
#define MALLINCAM_AAF_GETMAXSTEP      0x1c
#define MALLINCAM_AAF_GETSTEPSIZE     0x1e
#define MALLINCAM_AAF_RANGEMIN        0xfd  /* Range: min value */
#define MALLINCAM_AAF_RANGEMAX        0xfe  /* Range: max value */
#define MALLINCAM_AAF_RANGEDEF        0xff  /* Range: default value */
MALLINCAM_API(HRESULT) Mallincam_AAF(HMallincam h, int action, int outVal, int* inVal);

/* astronomy: for ST4 guide, please see: ASCOM Platform Help ICameraV2.
    nDirect: 0 = North, 1 = South, 2 = East, 3 = West, 4 = Stop
    nDuration: in milliseconds
*/
MALLINCAM_API(HRESULT)  Mallincam_ST4PlusGuide(HMallincam h, unsigned nDirect, unsigned nDuration);

/* S_OK: ST4 pulse guiding
   S_FALSE: ST4 not pulse guiding
*/
MALLINCAM_API(HRESULT)  Mallincam_ST4PlusGuideState(HMallincam h);

MALLINCAM_API(HRESULT)  Mallincam_Gain2TempTint(const int gain[3], int* temp, int* tint);
MALLINCAM_API(void)     Mallincam_TempTint2Gain(const int temp, const int tint, int gain[3]);
/*
    calculate the clarity factor:
    pImageData: pointer to the image data
    bits: 8(Grey), 16(Grey), 24(RGB24), 32(RGB32), 48(RGB48), 64(RGB64)
    nImgWidth, nImgHeight: the image width and height
    xOffset, yOffset, xWidth, yHeight: the Roi used to calculate. If not specified, use 1/5 * 1/5 rectangle in the center
    return < 0.0 when error
*/
MALLINCAM_API(double)   Mallincam_calc_ClarityFactor(const void* pImageData, int bits, unsigned nImgWidth, unsigned nImgHeight);
MALLINCAM_API(double)   Mallincam_calc_ClarityFactorV2(const void* pImageData, int bits, unsigned nImgWidth, unsigned nImgHeight, unsigned xOffset, unsigned yOffset, unsigned xWidth, unsigned yHeight);

/*
    nBitCount: output bitmap bit count
    when nBitDepth == 8:
        nBitCount must be 24 or 32
    when nBitDepth > 8
        nBitCount:  24 => RGB24
                    32 => RGB32
                    48 => RGB48
                    64 => RGB64
*/
MALLINCAM_API(void)     Mallincam_deBayerV2(unsigned nFourCC, int nW, int nH, const void* pRaw, void* pRGB, unsigned char nBitDepth, unsigned char nBitCount);


#ifndef __MALLINCAMFOCUSMOTOR_DEFINED__
#define __MALLINCAMFOCUSMOTOR_DEFINED__
typedef struct {
    int imax;    /* maximum auto focus sensor board positon */
    int imin;    /* minimum auto focus sensor board positon */
    int idef;    /* conjugate calibration positon */
    int imaxabs; /* maximum absolute auto focus sensor board positon, micrometer */
    int iminabs; /* maximum absolute auto focus sensor board positon, micrometer */
    int zoneh;   /* zone horizontal */
    int zonev;   /* zone vertical */
} MallincamFocusMotor;
#endif

MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_get_FocusMotor(HMallincam h, MallincamFocusMotor* pFocusMotor);

/*
* raw image process
* step:
*  'F': very beginning
*  'B': just before black balance
*  'D': just before demosaic
 */
typedef void (__stdcall* PMALLINCAM_PROCESS_CALLBACK)(char step, char bStill, unsigned nFourCC, int nW, int nH, void* pRaw, unsigned char pixelFormat, void* ctxProcess);
MALLINCAM_API(HRESULT)  Mallincam_put_Process(HMallincam h, PMALLINCAM_PROCESS_CALLBACK funProcess, void* ctxProcess);

/* debayer: raw to RGB */
typedef void (__stdcall* PMALLINCAM_DEMOSAIC_CALLBACK)(unsigned nFourCC, int nW, int nH, const void* pRaw, void* pRGB, unsigned char nBitDepth, void* ctxDemosaic);
MALLINCAM_API(HRESULT)  Mallincam_put_Demosaic(HMallincam h, PMALLINCAM_DEMOSAIC_CALLBACK funDemosaic, void* ctxDemosaic);

/*
    obsolete, please use MallincamModelV2
*/
typedef struct {
#if defined(_WIN32)
    const wchar_t*      name;       /* model name, in Windows, we use unicode */
#else
    const char*         name;       /* model name */
#endif
    unsigned            flag;       /* MALLINCAM_FLAG_xxx */
    unsigned            maxspeed;   /* number of speed level, same as Mallincam_get_MaxSpeed(), the speed range = [0, maxspeed], closed interval */
    unsigned            preview;    /* number of preview resolution, same as Mallincam_get_ResolutionNumber() */
    unsigned            still;      /* number of still resolution, same as Mallincam_get_StillResolutionNumber() */
    MallincamResolution   res[16];
} MallincamModel; /* camera model */

/*
    obsolete, please use Mallincam_deBayerV2
*/
MALLINCAM_DEPRECATED
MALLINCAM_API(void)     Mallincam_deBayer(unsigned nFourCC, int nW, int nH, const void* pRaw, void* pRGB, unsigned char nBitDepth);

/*
    obsolete, please use MallincamDeviceV2
*/
typedef struct {
#if defined(_WIN32)
    wchar_t             displayname[64];    /* display name */
    wchar_t             id[64];             /* unique and opaque id of a connected camera, for Mallincam_Open */
#else
    char                displayname[64];    /* display name */
    char                id[64];             /* unique and opaque id of a connected camera, for Mallincam_Open */
#endif
    const MallincamModel* model;
} MallincamDevice; /* camera instance for enumerating */

/*
    obsolete, please use Mallincam_EnumV2
*/
MALLINCAM_DEPRECATED
MALLINCAM_API(unsigned) Mallincam_Enum(MallincamDevice arr[MALLINCAM_MAX]);

typedef PMALLINCAM_DATA_CALLBACK_V3 PMALLINCAM_DATA_CALLBACK_V2;
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_StartPushModeV2(HMallincam h, PMALLINCAM_DATA_CALLBACK_V2 funData, void* ctxData);

#if !defined(_WIN32)
#ifndef __BITMAPINFOHEADER_DEFINED__
#define __BITMAPINFOHEADER_DEFINED__
typedef struct {
    unsigned        biSize;
    int             biWidth;
    int             biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned        biCompression;
    unsigned        biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned        biClrUsed;
    unsigned        biClrImportant;
} BITMAPINFOHEADER;
#endif
#endif

typedef void (__stdcall* PMALLINCAM_DATA_CALLBACK)(const void* pData, const BITMAPINFOHEADER* pHeader, int bSnap, void* ctxData);
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_StartPushMode(HMallincam h, PMALLINCAM_DATA_CALLBACK funData, void* ctxData);

MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_put_ExpoCallback(HMallincam h, PIMALLINCAM_EXPOSURE_CALLBACK funExpo, void* ctxExpo);
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_put_ChromeCallback(HMallincam h, PIMALLINCAM_CHROME_CALLBACK funChrome, void* ctxChrome);

/* Mallincam_FfcOnePush is obsolete, recommend using Mallincam_FfcOnce. */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_FfcOnePush(HMallincam h);

/* Mallincam_DfcOnePush is obsolete, recommend using Mallincam_DfcOnce. */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_DfcOnePush(HMallincam h);

/* Mallincam_AwbOnePush is obsolete, recommend using Mallincam_AwbOnce. */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_AwbOnePush(HMallincam h, PIMALLINCAM_TEMPTINT_CALLBACK funTT, void* ctxTT);

/* Mallincam_AbbOnePush is obsolete, recommend using Mallincam_AbbOnce. */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_AbbOnePush(HMallincam h, PIMALLINCAM_BLACKBALANCE_CALLBACK funBB, void* ctxBB);

#if defined(_WIN32)
/* Mallincam_put_TempTintInit is obsolete, recommend using Mallincam_AwbOnce */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_put_TempTintInit(HMallincam h, PIMALLINCAM_TEMPTINT_CALLBACK funTT, void* ctxTT);

/* ProcessMode: obsolete & useless, noop */
#ifndef __MALLINCAM_PROCESSMODE_DEFINED__
#define __MALLINCAM_PROCESSMODE_DEFINED__
#define MALLINCAM_PROCESSMODE_FULL        0x00    /* better image quality, more cpu usage. this is the default value */
#define MALLINCAM_PROCESSMODE_FAST        0x01    /* lower image quality, less cpu usage */
#endif
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_put_ProcessMode(HMallincam h, unsigned nProcessMode);
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_get_ProcessMode(HMallincam h, unsigned* pnProcessMode);
#endif

/* obsolete, recommend using Mallincam_put_Roi and Mallincam_get_Roi */
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_put_RoiMode(HMallincam h, int bRoiMode, int xOffset, int yOffset);
MALLINCAM_DEPRECATED
MALLINCAM_API(HRESULT)  Mallincam_get_RoiMode(HMallincam h, int* pbRoiMode, int* pxOffset, int* pyOffset);

/* obsolete:
     ------------------------------------------------------------|
     | Parameter         |   Range       |   Default             |
     |-----------------------------------------------------------|
     | VidgetAmount      |   -100~100    |   0                   |
     | VignetMidPoint    |   0~100       |   50                  |
     -------------------------------------------------------------
*/
MALLINCAM_API(HRESULT)  Mallincam_put_VignetEnable(HMallincam h, int bEnable);
MALLINCAM_API(HRESULT)  Mallincam_get_VignetEnable(HMallincam h, int* bEnable);
MALLINCAM_API(HRESULT)  Mallincam_put_VignetAmountInt(HMallincam h, int nAmount);
MALLINCAM_API(HRESULT)  Mallincam_get_VignetAmountInt(HMallincam h, int* nAmount);
MALLINCAM_API(HRESULT)  Mallincam_put_VignetMidPointInt(HMallincam h, int nMidPoint);
MALLINCAM_API(HRESULT)  Mallincam_get_VignetMidPointInt(HMallincam h, int* nMidPoint);

/* obsolete flags */
#define MALLINCAM_FLAG_BITDEPTH10    MALLINCAM_FLAG_RAW10  /* pixel format, RAW 10bits */
#define MALLINCAM_FLAG_BITDEPTH12    MALLINCAM_FLAG_RAW12  /* pixel format, RAW 12bits */
#define MALLINCAM_FLAG_BITDEPTH14    MALLINCAM_FLAG_RAW14  /* pixel format, RAW 14bits */
#define MALLINCAM_FLAG_BITDEPTH16    MALLINCAM_FLAG_RAW16  /* pixel format, RAW 16bits */

MALLINCAM_API(HRESULT)  Mallincam_log_File(const
#if defined(_WIN32)
                                       wchar_t*
#else
                                       char*
#endif
                                       filePath);
MALLINCAM_API(HRESULT)  Mallincam_log_Level(unsigned level); /* 0 => none; 1 => error; 2 => debug; 3 => verbose */
MALLINCAM_API(void)     Mallincam_log_str(unsigned level, const char* str);
MALLINCAM_APIV(void)    Mallincam_log(unsigned level, const char* format, ...);

#if defined(MALLINCAM_LOG)
#define MALLINCAM_LOG_NONE(format, ...)	  Mallincam_log(0, format, ##__VA_ARGS__)
#define MALLINCAM_LOG_ERROR(format, ...)	  Mallincam_log(1, format, ##__VA_ARGS__)
#define MALLINCAM_LOG_DEBUG(format, ...)	  Mallincam_log(2, format, ##__VA_ARGS__)
#define MALLINCAM_LOG_VERBOSE(format, ...)  Mallincam_log(3, format, ##__VA_ARGS__)
/* for example: MALLINCAM_LOG_DEBUG("%s: blahblah, x = %d, y = %f", __func__ x, y); */
#endif

#if defined(_WIN32)
#pragma pack(pop)
#endif

#ifdef __cplusplus
}
#endif

#endif
