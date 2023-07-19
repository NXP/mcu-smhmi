/*
Copyright 2021 by Retune DSP
Copyright 2022 NXP

NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef RDSP_TYPES_H_
#define RDSP_TYPES_H_

#include "RdspPlatforms.h"

#ifdef __cplusplus
extern "C" {
#endif

//	rdsp types				RDSP_USES_IEEE754_CODE			RDSP_USES_AFLOAT_CODE
//-------------------------------------------------------------------------------
// rdsp_float				float							fr32
// rdsp_dB:					float							fr32
// rdsp_floatx2:			float[2]						vr64
// rdsp_complex:			float[2]						vr64
// rdsp_double:				double							vr64
//-------------------------------------------------------------------------------


#if RDSP_USES_AFLOAT_CODE == 1
// Only HMD1A floating point - no HIFI3
typedef fr32 rdsp_float;
typedef fr32 rdsp_dB;
typedef vr64 rdsp_floatx2;
typedef vr64 rdsp_complex;
typedef vr64 rdsp_double;

#elif defined(HIFI4)

typedef xtfloat rdsp_float;
typedef xtfloat rdsp_dB;
typedef xtfloatx2 rdsp_floatx2;
typedef xtfloatx2 rdsp_complex;
typedef xtfloatx2 rdsp_double;

#elif defined(FUSIONDSP)

typedef xtfloat rdsp_float;
typedef xtfloat rdsp_dB;
typedef xtfloatx2 rdsp_floatx2;
typedef xtfloatx2 rdsp_complex;
typedef xtfloatx2 rdsp_double;

#else

typedef float rdsp_float;
typedef float rdsp_dB;
typedef float rdsp_floatx2[2];
typedef float rdsp_complex[2];
typedef double rdsp_double;

#endif

typedef rdsp_float rdsp_xyz_t[3];

#ifdef __cplusplus
}
#endif

#endif // RDSP_TYPES_H_
