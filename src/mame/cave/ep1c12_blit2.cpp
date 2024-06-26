// license:BSD-3-Clause
// copyright-holders:David Haywood
#include "emu.h"

#define REALLY_SIMPLE 0
/* X-Flipped, Non-Tinted, Transparent */
#define FLIPX 1
#define TINT 0
#define TRANSPARENT 1

#include "ep1c12.h"

/* Special Case */
#define BLENDED 0
#define FUNCNAME draw_sprite_f1_ti0_plain
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef BLENDED

/* Regular Cases*/
#define BLENDED 1

#define _SMODE 0
#define _DMODE 0
#define FUNCNAME draw_sprite_f1_ti0_tr1_s0_d0
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 1
#define _DMODE 0
#define FUNCNAME draw_sprite_f1_ti0_tr1_s1_d0
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 2
#define _DMODE 0
#define FUNCNAME draw_sprite_f1_ti0_tr1_s2_d0
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 3
#define _DMODE 0
#define FUNCNAME draw_sprite_f1_ti0_tr1_s3_d0
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 4
#define _DMODE 0
#define FUNCNAME draw_sprite_f1_ti0_tr1_s4_d0
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 5
#define _DMODE 0
#define FUNCNAME draw_sprite_f1_ti0_tr1_s5_d0
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 6
#define _DMODE 0
#define FUNCNAME draw_sprite_f1_ti0_tr1_s6_d0
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 7
#define _DMODE 0
#define FUNCNAME draw_sprite_f1_ti0_tr1_s7_d0
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

///////


#define _SMODE 0
#define _DMODE 1
#define FUNCNAME draw_sprite_f1_ti0_tr1_s0_d1
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 1
#define _DMODE 1
#define FUNCNAME draw_sprite_f1_ti0_tr1_s1_d1
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 2
#define _DMODE 1
#define FUNCNAME draw_sprite_f1_ti0_tr1_s2_d1
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 3
#define _DMODE 1
#define FUNCNAME draw_sprite_f1_ti0_tr1_s3_d1
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 4
#define _DMODE 1
#define FUNCNAME draw_sprite_f1_ti0_tr1_s4_d1
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 5
#define _DMODE 1
#define FUNCNAME draw_sprite_f1_ti0_tr1_s5_d1
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 6
#define _DMODE 1
#define FUNCNAME draw_sprite_f1_ti0_tr1_s6_d1
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 7
#define _DMODE 1
#define FUNCNAME draw_sprite_f1_ti0_tr1_s7_d1
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

////


#define _SMODE 0
#define _DMODE 2
#define FUNCNAME draw_sprite_f1_ti0_tr1_s0_d2
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 1
#define _DMODE 2
#define FUNCNAME draw_sprite_f1_ti0_tr1_s1_d2
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 2
#define _DMODE 2
#define FUNCNAME draw_sprite_f1_ti0_tr1_s2_d2
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 3
#define _DMODE 2
#define FUNCNAME draw_sprite_f1_ti0_tr1_s3_d2
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 4
#define _DMODE 2
#define FUNCNAME draw_sprite_f1_ti0_tr1_s4_d2
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 5
#define _DMODE 2
#define FUNCNAME draw_sprite_f1_ti0_tr1_s5_d2
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 6
#define _DMODE 2
#define FUNCNAME draw_sprite_f1_ti0_tr1_s6_d2
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 7
#define _DMODE 2
#define FUNCNAME draw_sprite_f1_ti0_tr1_s7_d2
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

///


#define _SMODE 0
#define _DMODE 3
#define FUNCNAME draw_sprite_f1_ti0_tr1_s0_d3
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 1
#define _DMODE 3
#define FUNCNAME draw_sprite_f1_ti0_tr1_s1_d3
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 2
#define _DMODE 3
#define FUNCNAME draw_sprite_f1_ti0_tr1_s2_d3
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 3
#define _DMODE 3
#define FUNCNAME draw_sprite_f1_ti0_tr1_s3_d3
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 4
#define _DMODE 3
#define FUNCNAME draw_sprite_f1_ti0_tr1_s4_d3
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 5
#define _DMODE 3
#define FUNCNAME draw_sprite_f1_ti0_tr1_s5_d3
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 6
#define _DMODE 3
#define FUNCNAME draw_sprite_f1_ti0_tr1_s6_d3
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 7
#define _DMODE 3
#define FUNCNAME draw_sprite_f1_ti0_tr1_s7_d3
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

///


#define _SMODE 0
#define _DMODE 4
#define FUNCNAME draw_sprite_f1_ti0_tr1_s0_d4
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 1
#define _DMODE 4
#define FUNCNAME draw_sprite_f1_ti0_tr1_s1_d4
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 2
#define _DMODE 4
#define FUNCNAME draw_sprite_f1_ti0_tr1_s2_d4
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 3
#define _DMODE 4
#define FUNCNAME draw_sprite_f1_ti0_tr1_s3_d4
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 4
#define _DMODE 4
#define FUNCNAME draw_sprite_f1_ti0_tr1_s4_d4
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 5
#define _DMODE 4
#define FUNCNAME draw_sprite_f1_ti0_tr1_s5_d4
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 6
#define _DMODE 4
#define FUNCNAME draw_sprite_f1_ti0_tr1_s6_d4
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 7
#define _DMODE 4
#define FUNCNAME draw_sprite_f1_ti0_tr1_s7_d4
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

///

#define _SMODE 0
#define _DMODE 5
#define FUNCNAME draw_sprite_f1_ti0_tr1_s0_d5
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 1
#define _DMODE 5
#define FUNCNAME draw_sprite_f1_ti0_tr1_s1_d5
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 2
#define _DMODE 5
#define FUNCNAME draw_sprite_f1_ti0_tr1_s2_d5
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 3
#define _DMODE 5
#define FUNCNAME draw_sprite_f1_ti0_tr1_s3_d5
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 4
#define _DMODE 5
#define FUNCNAME draw_sprite_f1_ti0_tr1_s4_d5
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 5
#define _DMODE 5
#define FUNCNAME draw_sprite_f1_ti0_tr1_s5_d5
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 6
#define _DMODE 5
#define FUNCNAME draw_sprite_f1_ti0_tr1_s6_d5
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 7
#define _DMODE 5
#define FUNCNAME draw_sprite_f1_ti0_tr1_s7_d5
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

///

#define _SMODE 0
#define _DMODE 6
#define FUNCNAME draw_sprite_f1_ti0_tr1_s0_d6
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 1
#define _DMODE 6
#define FUNCNAME draw_sprite_f1_ti0_tr1_s1_d6
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 2
#define _DMODE 6
#define FUNCNAME draw_sprite_f1_ti0_tr1_s2_d6
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 3
#define _DMODE 6
#define FUNCNAME draw_sprite_f1_ti0_tr1_s3_d6
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 4
#define _DMODE 6
#define FUNCNAME draw_sprite_f1_ti0_tr1_s4_d6
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 5
#define _DMODE 6
#define FUNCNAME draw_sprite_f1_ti0_tr1_s5_d6
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 6
#define _DMODE 6
#define FUNCNAME draw_sprite_f1_ti0_tr1_s6_d6
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 7
#define _DMODE 6
#define FUNCNAME draw_sprite_f1_ti0_tr1_s7_d6
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

///


#define _SMODE 0
#define _DMODE 7
#define FUNCNAME draw_sprite_f1_ti0_tr1_s0_d7
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 1
#define _DMODE 7
#define FUNCNAME draw_sprite_f1_ti0_tr1_s1_d7
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 2
#define _DMODE 7
#define FUNCNAME draw_sprite_f1_ti0_tr1_s2_d7
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 3
#define _DMODE 7
#define FUNCNAME draw_sprite_f1_ti0_tr1_s3_d7
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 4
#define _DMODE 7
#define FUNCNAME draw_sprite_f1_ti0_tr1_s4_d7
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 5
#define _DMODE 7
#define FUNCNAME draw_sprite_f1_ti0_tr1_s5_d7
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 6
#define _DMODE 7
#define FUNCNAME draw_sprite_f1_ti0_tr1_s6_d7
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#define _SMODE 7
#define _DMODE 7
#define FUNCNAME draw_sprite_f1_ti0_tr1_s7_d7
#include "ep1c12in.ipp"
#undef FUNCNAME
#undef _SMODE
#undef _DMODE

#undef BLENDED

#undef FLIPX
#undef TINT
#undef TRANSPARENT
#undef REALLY_SIMPLE
