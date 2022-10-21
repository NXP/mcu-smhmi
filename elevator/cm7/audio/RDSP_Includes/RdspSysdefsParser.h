/*
 * Copyright (c) 2021 by Retune DSP. This code is the confidential and
 * proprietary property of Retune DSP and the possession or use of this
 * file requires a written license from Retune DSP.
 *
 * Contact information: info@retune-dsp.com
 *                      www.retune-dsp.com
 */


#ifndef RDSP_SYSDEFS_PARSER_H
#define RDSP_SYSDEFS_PARSER_H

typedef struct RETUNE_VOICESEEKER_plugin_s RETUNE_VOICESEEKERLIGHT_plugin_t;

void load_user_sysdefs_xml(RETUNE_VOICESEEKERLIGHT_plugin_t* APluginInit, const char* Auser_sysdefs_fn);

#endif // RDSP_SYSDEFS_PARSER_H

