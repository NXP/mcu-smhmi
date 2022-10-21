/*
 * Copyright 2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

.section .rodata
.align 4

.global oob_demo_en_begin
.global oob_demo_cn_begin
.global oob_demo_de_begin
.global oob_demo_fr_begin


oob_demo_en_begin:
.incbin "../local_voice/oob_demo_en/oob_demo_en_pack_WithMapID.bin"
oob_demo_en_end:

oob_demo_cn_begin:
.incbin "../local_voice/oob_demo_cn/oob_demo_cn_pack_WithMapID.bin"
oob_demo_cn_end:

oob_demo_de_begin:
.incbin "../local_voice/oob_demo_de/oob_demo_de_pack_WithMapID.bin"
oob_demo_de_end:

oob_demo_fr_begin:
.incbin "../local_voice/oob_demo_fr/oob_demo_fr_pack_WithMapID.bin"
oob_demo_fr_end:

