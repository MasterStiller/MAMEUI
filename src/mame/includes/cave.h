// license:BSD-3-Clause
// copyright-holders:Luca Elia
#ifndef MAME_INCLUDES_CAVE_H
#define MAME_INCLUDES_CAVE_H

#pragma once

/***************************************************************************

    Cave hardware

***************************************************************************/

#include "machine/eepromser.h"
#include "machine/gen_latch.h"
#include "machine/timer.h"
#include "sound/okim6295.h"
#include "video/tmap038.h"
#include "emupal.h"
#include "screen.h"
#include "tilemap.h"

class cave_state : public driver_device
{
public:
	cave_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_videoregs(*this, "videoregs.%u", 0)
		, m_spriteram(*this, "spriteram.%u", 0)
		, m_io_in0(*this, "IN0")
		, m_io_bet(*this, "BET")
		, m_touch_x(*this, "TOUCH%u_X", 1U)
		, m_touch_y(*this, "TOUCH%u_Y", 1U)
		, m_spriteregion(*this, "sprites%u", 0)
		, m_tileregion(*this, "layer%u", 0)
		, m_okiregion(*this, "oki%u", 1)
		, m_z80region(*this, "audiocpu")
		, m_z80bank(*this, "z80bank")
		, m_okibank_lo(*this, "oki%u_banklo", 1)
		, m_okibank_hi(*this, "oki%u_bankhi", 1)
		, m_maincpu(*this, "maincpu")
		, m_audiocpu(*this, "audiocpu")
		, m_oki(*this, "oki%u", 1)
		, m_int_timer(*this, "int_timer")
		, m_int_timer_left(*this, "int_timer_left")
		, m_int_timer_right(*this, "int_timer_right")
		, m_eeprom(*this, "eeprom")
		, m_gfxdecode(*this, "gfxdecode.%u", 0U)
		, m_spr_gfxdecode(*this, "spr_gfxdecode.%u", 0U)
		, m_screen(*this, "screen.%u", 0U)
		, m_palette(*this, "palette.%u", 0U)
		, m_tilemap(*this, "tilemap.%u", 0U)
		, m_soundlatch(*this, "soundlatch")
		, m_startup(*this, "startup")
		, m_led_outputs(*this, "led%u", 0U)
	{ }

	DECLARE_READ_LINE_MEMBER(korokoro_hopper_r);
	DECLARE_READ_LINE_MEMBER(tjumpman_hopper_r);
	DECLARE_READ_LINE_MEMBER(paccarn_bet4_r);
	DECLARE_READ_LINE_MEMBER(paccarn_bet8_r);

	void init_uopoko();
	void init_donpachi();
	void init_mazinger();
	void init_gaia();
	void init_pwrinst2();
	void init_ddonpach();
	void init_agallet();
	void init_hotdogst();
	void init_tjumpman();
	void init_korokoro();
	void init_esprade();
	void init_pwrinst2j();
	void init_guwange();
	void init_feversos();
	void init_sailormn();
	void init_dfeveron();
	void init_metmqstr();
	void init_ppsatan();

	void uopoko(machine_config &config);
	void sailormn(machine_config &config);
	void paceight(machine_config &config);
	void pacslot(machine_config &config);
	void paccarn(machine_config &config);
	void hotdogst(machine_config &config);
	void crusherm(machine_config &config);
	void donpachi(machine_config &config);
	void tekkencw(machine_config &config);
	void korokoro(machine_config &config);
	void esprade(machine_config &config);
	void mazinger(machine_config &config);
	void tjumpman(machine_config &config);
	void tekkenbs(machine_config &config);
	void gaia(machine_config &config);
	void metmqstr(machine_config &config);
	void ppsatan(machine_config &config);
	void guwange(machine_config &config);
	void dfeveron(machine_config &config);
	void ddonpach(machine_config &config);
	void pwrinst2(machine_config &config);

protected:
	virtual void device_post_load() override;

private:

	enum
	{
		TYPE_ZOOM = 0,
		TYPE_NOZOOM = 1,
		TYPE_ISPWRINST2 = 2
	};

	void (cave_state::*m_get_sprite_info)(int chip);
	void (cave_state::*m_sprite_draw)(int chip, int priority);

	void add_base_config(machine_config &config, int layer);
	void add_ymz(machine_config &config);

	u16 irq_cause_r(offs_t offset);
	u8 soundflags_r();
	u16 soundflags_ack_r();
	void sound_cmd_w(u16 data);
	u8 soundlatch_lo_r();
	u8 soundlatch_hi_r();
	u16 soundlatch_ack_r();
	void soundlatch_ack_w(u8 data);
	void gaia_coin_w(u8 data);
	u16 donpachi_videoregs_r(offs_t offset);
	template<int Chip> void videoregs_w(offs_t offset, u16 data, u16 mem_mask);
	void korokoro_leds_w(offs_t offset, u16 data, u16 mem_mask);
	template<int Chip> void pwrinst2_vctrl_w(offs_t offset, u16 data, u16 mem_mask);
	u16 sailormn_input0_r();
	void tjumpman_leds_w(u8 data);
	void pacslot_leds_w(u8 data);
	template<int Mask> void z80_rombank_w(u8 data);
	template<int Mask> void oki1_bank_w(u8 data);
	template<int Mask> void oki2_bank_w(u8 data);
	void eeprom_w(u8 data);
	void sailormn_eeprom_w(u8 data);
	void hotdogst_eeprom_w(u8 data);
	void guwange_eeprom_w(u8 data);
	void metmqstr_eeprom_w(u8 data);
	void korokoro_eeprom_w(offs_t offset, u16 data, u16 mem_mask);
	u16 pwrinst2_eeprom_r();
	void tjumpman_eeprom_w(u8 data);
	void ppsatan_eeprom_w(offs_t offset, u16 data, u16 mem_mask);
	void ppsatan_io_mux_w(offs_t offset, u16 data, u16 mem_mask);
	template<int Player> u16 ppsatan_touch_r();
	void ppsatan_out_w(offs_t offset, u16 data, u16 mem_mask);
	void sailormn_get_banked_code(bool tiledim, u32 &color, u32 &pri, u32 &code);
	DECLARE_MACHINE_RESET(sailormn);
	DECLARE_VIDEO_START(spr_4bpp);
	DECLARE_VIDEO_START(spr_8bpp);
	DECLARE_VIDEO_START(ppsatan);
	void cave_palette(palette_device &palette);
	void dfeveron_palette(palette_device &palette);
	void korokoro_palette(palette_device &palette);
	void mazinger_palette(palette_device &palette);
	void pwrinst2_palette(palette_device &palette);
	void sailormn_palette(palette_device &palette);
	void ppsatan_palette(palette_device &palette);
	u32 screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	u32 screen_update_ppsatan_core (screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect, int chip);
	u32 screen_update_ppsatan_top  (screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	u32 screen_update_ppsatan_left (screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	u32 screen_update_ppsatan_right(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	INTERRUPT_GEN_MEMBER(interrupt);
	INTERRUPT_GEN_MEMBER(interrupt_ppsatan);
	TIMER_CALLBACK_MEMBER(vblank_end);
	TIMER_DEVICE_CALLBACK_MEMBER(vblank_start);
	TIMER_DEVICE_CALLBACK_MEMBER(vblank_start_left);
	TIMER_DEVICE_CALLBACK_MEMBER(vblank_start_right);
	TIMER_DEVICE_CALLBACK_MEMBER(timer_lev2_cb);
	TIMER_DEVICE_CALLBACK_MEMBER(sailormn_startup);
	void get_sprite_info(int chip);
	void sailormn_tilebank_w(int bank);
	DECLARE_WRITE_LINE_MEMBER(sound_irq_gen);
	void update_irq_state();
	void unpack_sprites(int chip);
	void sailormn_unpack_tiles(int chip);

	virtual void machine_start() override;
	virtual void machine_reset() override;

	/* memory pointers */
	optional_shared_ptr_array<u16, 4> m_videoregs;
	optional_shared_ptr_array<u16, 4> m_spriteram;

	optional_ioport m_io_in0;
	optional_ioport m_io_bet;
	optional_ioport_array<2> m_touch_x;
	optional_ioport_array<2> m_touch_y;

	/* memory regions */
	optional_memory_region_array<4> m_spriteregion;
	optional_memory_region_array<4> m_tileregion;
	optional_memory_region_array<2> m_okiregion;
	optional_memory_region          m_z80region;
	optional_memory_bank            m_z80bank;
	optional_memory_bank_array<2>   m_okibank_lo;
	optional_memory_bank_array<2>   m_okibank_hi;

	/* video-related */
	enum
	{
		MAX_PRIORITY        = 4,
		MAX_SPRITE_NUM      = 0x400
	};

	struct sprite_cave
	{
		sprite_cave() { }

		int priority = 0, flags = 0;

		const u8 *pen_data = nullptr;  /* points to top left corner of tile data */
		int line_offset = 0;

		pen_t base_pen = 0;
		int tile_width = 0, tile_height = 0;
		int total_width = 0, total_height = 0;  /* in screen coordinates */
		int x = 0, y = 0, xcount0 = 0, ycount0 = 0;
		int zoomx_re = 0, zoomy_re = 0;
	};

	struct
	{
		int clip_left = 0, clip_right = 0, clip_top = 0, clip_bottom = 0;
		u8  *baseaddr = 0U;
		int line_offset = 0;
		u8  *baseaddr_zbuf =0U;
		int line_offset_zbuf = 0;
	} m_blit;

	std::unique_ptr<sprite_cave []> m_sprite[4]{};
	sprite_cave *m_sprite_table[4][MAX_PRIORITY][MAX_SPRITE_NUM + 1]{};

	bitmap_ind16 m_sprite_zbuf[4]{};
	u16       m_sprite_zbuf_baseval = 0U;

	std::unique_ptr<u8[]> m_sprite_gfx[4]{};
	offs_t                m_sprite_gfx_mask[4]{};

	int       m_num_sprites[4]{};

	int       m_spriteram_bank[4]{};
	int       m_spriteram_bank_delay[4]{};

	int       m_layers_offs_x = 0;
	int       m_layers_offs_y = 0;
	int       m_row_effect_offs_n = 0;
	int       m_row_effect_offs_f = 0;
	u16       m_background_pen[4]{};

	int       m_spritetype[2]{};
	int       m_kludge = 0;
	emu_timer *m_vblank_end_timer = nullptr;

	u16       m_sprite_granularity = 0U;
	u32       m_max_sprite_clk[4]{}; // max usable clock for sprites

	/* misc */
	int       m_time_vblank_irq = 0;
	u8        m_irq_level = 0U;
	u8        m_vblank_irq = 0U;
	u8        m_sound_irq = 0U;
	u8        m_unknown_irq = 0U;
	u8        m_agallet_vblank_irq = 0U;

	/* sound related */
	int       m_soundbuf_wptr = 0;
	int       m_soundbuf_rptr = 0;
	u8        m_soundbuf_data[32]{};
	bool      m_soundbuf_empty = false;
	//u8      m_sound_flag[2]{};

	/* game specific */
	// sailormn
	int       m_sailormn_tilebank = 0;
	// korokoro
	u16       m_leds[2]{};
	int       m_hopper = 0;
	// ppsatan
	u16       m_ppsatan_io_mux = 0U;

	/* devices */
	required_device<cpu_device> m_maincpu;
	optional_device<cpu_device> m_audiocpu;
	optional_device_array<okim6295_device, 2> m_oki;
	required_device<timer_device> m_int_timer;
	optional_device<timer_device> m_int_timer_left;
	optional_device<timer_device> m_int_timer_right;
	optional_device<eeprom_serial_93cxx_device> m_eeprom;
	optional_device_array<gfxdecode_device, 4> m_gfxdecode;
	optional_device_array<gfxdecode_device, 4> m_spr_gfxdecode;
	optional_device_array<screen_device, 4> m_screen;
	optional_device_array<palette_device, 4> m_palette;
	optional_device_array<tilemap038_device, 4> m_tilemap;
	optional_device<generic_latch_16_device> m_soundlatch;
	optional_device<timer_device> m_startup;
	output_finder<9> m_led_outputs;

	int m_rasflag = 0;
	int m_old_rasflag = 0;

	inline void tilemap_draw(int chip, screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect, u32 flags, u32 priority, u32 priority2, int GFX);
	void set_pens(int chip);
	void vh_start(u16 sprcol_granularity);
	void get_sprite_info_cave(int chip);
	void get_sprite_info_donpachi(int chip);
	void sprite_init();
	void sprite_check(int chip, int screen_no, screen_device &screen, const rectangle &clip);
	void do_blit_zoom32(int chip, const sprite_cave *sprite);
	void do_blit_zoom32_zb(int chip, const sprite_cave *sprite);
	void do_blit_32(int chip, const sprite_cave *sprite);
	void do_blit_32_zb(int chip, const sprite_cave *sprite);
	void sprite_draw_cave(int chip, int priority);
	void sprite_draw_cave_zbuf(int chip, int priority);
	void sprite_draw_donpachi(int chip, int priority);
	void sprite_draw_donpachi_zbuf(int chip, int priority);
	void init_cave();
	void init_z80_bank();
	void init_oki_bank(int chip);
	void show_leds();

	void crusherm_map(address_map &map);
	void ddonpach_map(address_map &map);
	void dfeveron_map(address_map &map);
	void donpachi_map(address_map &map);
	void esprade_map(address_map &map);
	void gaia_map(address_map &map);
	void guwange_map(address_map &map);
	void hotdogst_map(address_map &map);
	void hotdogst_sound_map(address_map &map);
	void hotdogst_sound_portmap(address_map &map);
	void korokoro_map(address_map &map);
	void mazinger_map(address_map &map);
	void mazinger_sound_map(address_map &map);
	void mazinger_sound_portmap(address_map &map);
	void metmqstr_map(address_map &map);
	void metmqstr_sound_map(address_map &map);
	void metmqstr_sound_portmap(address_map &map);
	void oki2_map(address_map &map);
	void oki_map(address_map &map);
	void paccarn_map(address_map &map);
	void paceight_map(address_map &map);
	void pacslot_map(address_map &map);
	void ppsatan_map(address_map &map);
	void pwrinst2_map(address_map &map);
	void pwrinst2_sound_map(address_map &map);
	void pwrinst2_sound_portmap(address_map &map);
	void sailormn_map(address_map &map);
	void sailormn_sound_map(address_map &map);
	void sailormn_sound_portmap(address_map &map);
	void tekkenbs_map(address_map &map);
	void tekkencw_map(address_map &map);
	void tjumpman_map(address_map &map);
	void uopoko_map(address_map &map);
};

#endif // MAME_INCLUDES_CAVE_H
