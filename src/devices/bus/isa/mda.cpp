// license:BSD-3-Clause
// copyright-holders:Wilbert Pol, Miodrag Milanovic
/***************************************************************************

  Monochrome Display Adapter (MDA) section

***************************************************************************/

#include "emu.h"
#include "mda.h"

#include "screen.h"


#define LOG_READ    (1U << 1)
#define LOG_SETUP   (1U << 2)
#define LOG_ROW     (1U << 3)
#define LOG_MODE    (1U << 4)
#define LOG_CHRG    (1U << 5)
#define LOG_STAT    (1U << 6)

//#define VERBOSE (LOG_MODE|LOG_STAT)
//#define LOG_OUTPUT_STREAM std::cout

#include "logmacro.h"

#define LOGR(...)     LOGMASKED(LOG_READ,  __VA_ARGS__)
#define LOGSETUP(...) LOGMASKED(LOG_SETUP, __VA_ARGS__)
#define LOGROW(...)   LOGMASKED(LOG_ROW,   __VA_ARGS__)
#define LOGMODE(...)  LOGMASKED(LOG_MODE,  __VA_ARGS__)
#define LOGCHRG(...)  LOGMASKED(LOG_CHRG,  __VA_ARGS__)
#define LOGSTAT(...)  LOGMASKED(LOG_STAT,  __VA_ARGS__)

#ifdef _MSC_VER
#define FUNCNAME __func__
#else
#define FUNCNAME __PRETTY_FUNCTION__
#endif

#define MDA_SCREEN_NAME "mda_screen"
#define MC6845_NAME "mc6845"

/*
  Hercules video card
 */
#define HERCULES_SCREEN_NAME "hercules_screen"
#define MDA_CLOCK   XTAL(16'257'000)

static const unsigned char mda_palette[4][3] =
{
	{ 0x00,0x00,0x00 },
	{ 0x00,0x55,0x00 },
	{ 0x00,0xaa,0x00 },
	{ 0x00,0xff,0x00 }
};

enum
{
	MDA_TEXT_INTEN = 0,
	MDA_TEXT_BLINK,
	HERCULES_GFX_BLINK,
	MDA_LOWRES_TEXT_INTEN,
	MDA_LOWRES_TEXT_BLINK
};

/* F4 Character Displayer */
static const gfx_layout pc_16_charlayout =
{
	8, 16,                  /* 8 x 16 characters */
	256,                    /* 256 characters */
	1,                  /* 1 bits per pixel */
	{ 0 },                  /* no bitplanes */
	/* x offsets */
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	/* y offsets */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8, 2048*8, 2049*8, 2050*8, 2051*8, 2052*8, 2053*8, 2054*8, 2055*8 },
	8*8                 /* every char takes 2 x 8 bytes */
};

static const gfx_layout pc_8_charlayout =
{
	8, 8,                   /* 8 x 8 characters */
	512,                    /* 512 characters */
	1,                  /* 1 bits per pixel */
	{ 0 },                  /* no bitplanes */
	/* x offsets */
	{ 0, 1, 2, 3, 4, 5, 6, 7 },
	/* y offsets */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	8*8                 /* every char takes 8 bytes */
};

static GFXDECODE_START( gfx_pcmda )
	GFXDECODE_ENTRY( "gfx1", 0x0000, pc_16_charlayout, 1, 1 )
	GFXDECODE_ENTRY( "gfx1", 0x1000, pc_8_charlayout, 1, 1 )
GFXDECODE_END


void isa8_mda_device::pc_cpu_line(int state)
{
	m_isa->irq7_w(state);
}


ROM_START( mda )
	/* IBM 1501981(CGA) and 1501985(MDA) Character rom */
	ROM_REGION(0x08100,"gfx1", 0)
	ROM_LOAD("5788005.u33", 0x00000, 0x02000, CRC(0bf56d70) SHA1(c2a8b10808bf51a3c123ba3eb1e9dd608231916f)) /* "AMI 8412PI // 5788005 // (C) IBM CORP. 1981 // KOREA" */
ROM_END

//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************

DEFINE_DEVICE_TYPE(ISA8_MDA, isa8_mda_device, "isa_ibm_mda", "IBM Monochrome Display and Printer Adapter")


//-------------------------------------------------
//  device_add_mconfig - add device configuration
//-------------------------------------------------

void isa8_mda_device::device_add_mconfig(machine_config &config)
{
	screen_device &screen(SCREEN(config, MDA_SCREEN_NAME, SCREEN_TYPE_RASTER));
	screen.set_raw(MDA_CLOCK, 882, 0, 720, 370, 0, 350);
	screen.set_screen_update(MC6845_NAME, FUNC(mc6845_device::screen_update));

	PALETTE(config, m_palette).set_entries(4);

	MC6845(config, m_crtc, MDA_CLOCK/9);
	m_crtc->set_screen(MDA_SCREEN_NAME);
	m_crtc->set_show_border_area(false);
	m_crtc->set_char_width(9);
	m_crtc->set_update_row_callback(FUNC(isa8_mda_device::crtc_update_row));
	m_crtc->out_hsync_callback().set(FUNC(isa8_mda_device::hsync_changed));
	m_crtc->out_vsync_callback().set(FUNC(isa8_mda_device::vsync_changed));

	GFXDECODE(config, "gfxdecode", m_palette, gfx_pcmda);

	PC_LPT(config, m_lpt);
	m_lpt->irq_handler().set(FUNC(isa8_mda_device::pc_cpu_line));
}

//-------------------------------------------------
//  rom_region - device-specific ROM region
//-------------------------------------------------

const tiny_rom_entry *isa8_mda_device::device_rom_region() const
{
	return ROM_NAME( mda );
}

//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  isa8_mda_device - constructor
//-------------------------------------------------

isa8_mda_device::isa8_mda_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	isa8_mda_device(mconfig, ISA8_MDA, tag, owner, clock)
{
}

isa8_mda_device::isa8_mda_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock) :
	device_t(mconfig, type, tag, owner, clock),
	device_isa8_card_interface(mconfig, *this), m_crtc(*this, MC6845_NAME), m_lpt(*this, "lpt"), m_framecnt(0), m_mode_control(0),
	m_update_row_type(-1), m_chr_gen(nullptr), m_vsync(0), m_hsync(0), m_pixel(0),
	m_palette(*this, "palette")
{
}

//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void isa8_mda_device::device_start()
{
	if (m_palette != nullptr && !m_palette->started())
		throw device_missing_dependencies();

	set_isa_device();
	m_videoram.resize(0x1000);
	m_isa->install_device(0x3b0, 0x3bf, read8sm_delegate(*this, FUNC(isa8_mda_device::io_read)), write8sm_delegate(*this, FUNC(isa8_mda_device::io_write)));
	m_isa->install_bank(0xb0000, 0xb0fff, &m_videoram[0]);
	m_isa->install_bank(0xb1000, 0xb1fff, &m_videoram[0]);
	m_isa->install_bank(0xb2000, 0xb2fff, &m_videoram[0]);
	m_isa->install_bank(0xb3000, 0xb3fff, &m_videoram[0]);
	m_isa->install_bank(0xb4000, 0xb4fff, &m_videoram[0]);
	m_isa->install_bank(0xb5000, 0xb5fff, &m_videoram[0]);
	m_isa->install_bank(0xb6000, 0xb6fff, &m_videoram[0]);
	m_isa->install_bank(0xb7000, 0xb7fff, &m_videoram[0]);

	/* Initialise the mda palette */
	for (int i = 0; i < 4; i++)
		m_palette->set_pen_color(i, rgb_t(mda_palette[i][0], mda_palette[i][1], mda_palette[i][2]));
}

//-------------------------------------------------
//  device_reset - device-specific reset
//-------------------------------------------------

void isa8_mda_device::device_reset()
{
	m_framecnt = 0;
	m_mode_control = 0;
	m_vsync = 0;
	m_hsync = 0;
	m_pixel = 0;

	m_chr_gen = memregion(subtag("gfx1").c_str())->base();
}

/***************************************************************************

  Monochrome Display Adapter (MDA) section

***************************************************************************/

/***************************************************************************
  Draw text mode with 80x25 characters (default) and intense background.
  The character cell size is 9x15. Column 9 is column 8 repeated for
  character codes 176 to 223.
***************************************************************************/
MC6845_UPDATE_ROW( isa8_mda_device::mda_text_inten_update_row )
{
	rgb_t const *const palette = m_palette->palette()->entry_list_raw();
	uint32_t *p = &bitmap.pix(y);
	uint16_t const chr_base = (ra & 0x08) ? 0x800 | (ra & 0x07) : ra;

	if (y == 0) LOGROW("%11.6f: %-24s\n", machine().time().as_double(), FUNCNAME);
	for (int i = 0; i < x_count; i++)
	{
		uint16_t const offset = ((ma + i) << 1) & 0x0fff;
		uint8_t const chr = m_videoram[offset];
		uint8_t const attr = m_videoram[offset + 1];
		uint8_t data = m_chr_gen[chr_base + chr * 8];
		uint8_t fg = (attr & 0x08) ? 3 : 2;
		uint8_t bg = 0;

		if ((attr & ~0x88) == 0)
		{
			data = 0x00;
		}

		switch (attr)
		{
		case 0x70:
			bg = 2;
			fg = 0;
			break;
		case 0x78:
			bg = 2;
			fg = 1;
			break;
		case 0xf0:
			bg = 3;
			fg = 0;
			break;
		case 0xf8:
			bg = 3;
			fg = 1;
			break;
		}

		if ((i == cursor_x && (m_framecnt & 0x08)) || (attr & 0x07) == 0x01)
		{
			data = 0xff;
		}

		*p++ = palette[(data & 0x80) ? fg : bg];
		*p++ = palette[(data & 0x40) ? fg : bg];
		*p++ = palette[(data & 0x20) ? fg : bg];
		*p++ = palette[(data & 0x10) ? fg : bg];
		*p++ = palette[(data & 0x08) ? fg : bg];
		*p++ = palette[(data & 0x04) ? fg : bg];
		*p++ = palette[(data & 0x02) ? fg : bg];
		*p++ = palette[(data & 0x01) ? fg : bg];
		if ((chr & 0xe0) == 0xc0)
		{
			*p++ = palette[(data & 0x01) ? fg : bg];
		}
		else
		{
			*p++ = palette[bg];
		}
	}
}


/***************************************************************************
  Draw text mode with 80x25 characters (default) and blinking characters.
  The character cell size is 9x15. Column 9 is column 8 repeated for
  character codes 176 to 223.
***************************************************************************/

MC6845_UPDATE_ROW( isa8_mda_device::mda_text_blink_update_row )
{
	rgb_t const *const palette = m_palette->palette()->entry_list_raw();
	uint32_t *p = &bitmap.pix(y);
	uint16_t const chr_base = (ra & 0x08) ? 0x800 | (ra & 0x07) : ra;

	if (y == 0) LOGROW("%11.6f: %-24s\n", machine().time().as_double(), FUNCNAME);
	for (int i = 0; i < x_count; i++)
	{
		uint16_t const offset = ((ma + i) << 1) & 0x0fff;
		uint8_t const chr = m_videoram[offset];
		uint8_t const attr = m_videoram[offset + 1];
		uint8_t data = m_chr_gen[chr_base + chr * 8];
		uint8_t fg = (attr & 0x08) ? 3 : 2;
		uint8_t bg = 0;

		if ((attr & ~0x88) == 0)
		{
			data = 0x00;
		}

		switch (attr)
		{
		case 0x70:
		case 0xf0:
			bg = 2;
			fg = 0;
			break;
		case 0x78:
		case 0xf8:
			bg = 2;
			fg = 1;
			break;
		}

		if ((attr & 0x07) == 0x01)
		{
			data = 0xff;
		}

		if (i == cursor_x)
		{
			if (m_framecnt & 0x08)
			{
				data = 0xff;
			}
		}
		else
		{
			if ((attr & 0x80) && (m_framecnt & 0x10))
			{
				data = 0x00;
			}
		}

		*p++ = palette[(data & 0x80) ? fg : bg];
		*p++ = palette[(data & 0x40) ? fg : bg];
		*p++ = palette[(data & 0x20) ? fg : bg];
		*p++ = palette[(data & 0x10) ? fg : bg];
		*p++ = palette[(data & 0x08) ? fg : bg];
		*p++ = palette[(data & 0x04) ? fg : bg];
		*p++ = palette[(data & 0x02) ? fg : bg];
		*p++ = palette[(data & 0x01) ? fg : bg];
		if ((chr & 0xe0) == 0xc0)
		{
			*p++ = palette[(data & 0x01) ? fg : bg];
		}
		else
		{
			*p++ = palette[bg];
		}
	}
}

MC6845_UPDATE_ROW( isa8_mda_device::crtc_update_row )
{
	if (m_update_row_type == -1)
		return;

	switch (m_update_row_type)
	{
		case MDA_TEXT_INTEN:
			mda_text_inten_update_row(bitmap, cliprect, ma, ra, y, x_count, cursor_x, de, hbp, vbp);
			break;
		case MDA_TEXT_BLINK:
			mda_text_blink_update_row(bitmap, cliprect, ma, ra, y, x_count, cursor_x, de, hbp, vbp);
			break;
	}
}


MC6845_UPDATE_ROW( isa8_hercules_device::crtc_update_row )
{
	if (m_update_row_type == -1)
		return;

	switch (m_update_row_type)
	{
		case HERCULES_GFX_BLINK:
			hercules_gfx_update_row(bitmap, cliprect, ma, ra, y, x_count, cursor_x, de, hbp, vbp);
			break;
		default:
			isa8_mda_device::crtc_update_row(bitmap, cliprect, ma, ra, y, x_count, cursor_x, de, hbp, vbp);
			break;
	}
}


void isa8_mda_device::hsync_changed(int state)
{
	m_hsync = state ? 1 : 0;
}


void isa8_mda_device::vsync_changed(int state)
{
	m_vsync = state ? 0x80 : 0;
	if ( state )
	{
		m_framecnt++;
	}
}


/*
 *  rW  MDA mode control register (see #P138)
 */
void isa8_mda_device::mode_control_w(uint8_t data)
{
	m_mode_control = data;

	switch( m_mode_control & 0x2a )
	{
	case 0x08:
		m_update_row_type = MDA_TEXT_INTEN;
		break;
	case 0x28:
		m_update_row_type = MDA_TEXT_BLINK;
		break;
	default:
		m_update_row_type = -1;
	}
}


/*  R-  CRT status register (see #P139)
 *      (EGA/VGA) input status 1 register
 *      7    HGC vertical sync in progress
 *      6-4  adapter 000  hercules
 *                   001  hercules+
 *                   101  hercules InColor
 *                   else unknown
 *      3    pixel stream (0 black, 1 white)
 *      2-1  reserved
 *      0    horizontal drive enable
 */
uint8_t isa8_mda_device::status_r()
{
	// Faking pixel stream here
	if (!machine().side_effects_disabled())
		m_pixel++;

	return 0xF0 | (m_pixel & 0x08) | m_hsync;
}


/*************************************************************************
 *
 *      MDA
 *      monochrome display adapter
 *
 *************************************************************************/
void isa8_mda_device::io_write(offs_t offset, uint8_t data)
{
	switch( offset )
	{
		case 0x00: case 0x02: case 0x04: case 0x06:
			m_crtc->address_w(data);
			break;
		case 0x01: case 0x03: case 0x05: case 0x07:
			m_crtc->register_w(data);
			break;
		case 0x08:
			mode_control_w(data);
			break;
		case 0x0c: case 0x0d:  case 0x0e:
			m_lpt->write(offset - 0x0c, data);
			break;
	}
}

uint8_t isa8_mda_device::io_read(offs_t offset)
{
	int data = 0xff;
	switch( offset )
	{
		case 0x00: case 0x02: case 0x04: case 0x06:
			/* return last written mc6845 address value here? */
			break;
		case 0x01: case 0x03: case 0x05: case 0x07:
			data = m_crtc->register_r();
			break;
		case 0x0a:
			data = status_r();
			break;
		/* LPT ports */
		case 0x0c: case 0x0d:  case 0x0e:
			data = m_lpt->read(offset - 0x0c);
			break;
	}
	return data;
}


/***************************************************************************

  Hercules Display Adapter section (re-uses parts from the MDA section)

***************************************************************************/

/*
When the Hercules changes to graphics mode, the number of pixels per access and
clock divider should be changed. The correct mc6845 implementation does not
allow this.

The divder/pixels per 6845 clock is 9 for text mode and 16 for graphics mode.
*/

static GFXDECODE_START( gfx_pcherc )
	GFXDECODE_ENTRY( "gfx1", 0x0000, pc_16_charlayout, 1, 1 )
GFXDECODE_END

ROM_START( hercules )
	ROM_REGION(0x1000, "gfx1", 0)
	ROM_SYSTEM_BIOS(0, "cp437", "Code page 437")
	ROMX_LOAD("um2301.bin", 0x0000, 0x1000, CRC(0827bdac) SHA1(15f1aceeee8b31f0d860ff420643e3c7f29b5ffc), ROM_BIOS(0))
	ROM_SYSTEM_BIOS(1, "mzv", "Mazovia (Polish)") // dumped from a Taiwanese-made card using the SiS 86C22 chip
	ROMX_LOAD("hgc_mzv_2301.bin", 0x0000, 0x1000, CRC(9431b9e0) SHA1(3279dfeed4a0f5daa7b57d455c96eafdcbb6bf41), ROM_BIOS(1))
ROM_END

//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************

DEFINE_DEVICE_TYPE(ISA8_HERCULES, isa8_hercules_device, "isa_hercules", "Hercules Graphics Card")

//-------------------------------------------------
//  device_add_mconfig - add device configuration
//-------------------------------------------------

void isa8_hercules_device::device_add_mconfig(machine_config &config)
{
	screen_device &screen(SCREEN(config, HERCULES_SCREEN_NAME, SCREEN_TYPE_RASTER));
	screen.set_raw(MDA_CLOCK, 882, 0, 720, 370, 0, 350);
	screen.set_screen_update(MC6845_NAME, FUNC(mc6845_device::screen_update));

	PALETTE(config, m_palette).set_entries(4);

	MC6845(config, m_crtc, MDA_CLOCK/9);
	m_crtc->set_screen(HERCULES_SCREEN_NAME);
	m_crtc->set_show_border_area(false);
	m_crtc->set_char_width(9);
	m_crtc->set_update_row_callback(FUNC(isa8_hercules_device::crtc_update_row));
	m_crtc->out_hsync_callback().set(FUNC(isa8_mda_device::hsync_changed));
	m_crtc->out_vsync_callback().set(FUNC(isa8_mda_device::vsync_changed));

	GFXDECODE(config, "gfxdecode", m_palette, gfx_pcherc);

	PC_LPT(config, m_lpt);
	m_lpt->irq_handler().set(FUNC(isa8_mda_device::pc_cpu_line));
}

//-------------------------------------------------
//  rom_region - device-specific ROM region
//-------------------------------------------------

const tiny_rom_entry *isa8_hercules_device::device_rom_region() const
{
	return ROM_NAME( hercules );
}

//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  isa8_hercules_device - constructor
//-------------------------------------------------

isa8_hercules_device::isa8_hercules_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	isa8_mda_device(mconfig, ISA8_HERCULES, tag, owner, clock), m_configuration_switch(0)
{
}

//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void isa8_hercules_device::device_start()
{
	if (m_palette != nullptr && !m_palette->started())
		throw device_missing_dependencies();

	m_videoram.resize(0x10000);
	set_isa_device();
	m_isa->install_device(0x3b0, 0x3bf, read8sm_delegate(*this, FUNC(isa8_hercules_device::io_read)), write8sm_delegate(*this, FUNC(isa8_hercules_device::io_write)));
	m_isa->install_bank(0xb0000, 0xbffff, &m_videoram[0]);

	/* Initialise the mda palette */
	for(int i = 0; i < (sizeof(mda_palette) / 3); i++)
		m_palette->set_pen_color(i, mda_palette[i][0], mda_palette[i][1], mda_palette[i][2]);
}

//-------------------------------------------------
//  device_reset - device-specific reset
//-------------------------------------------------

void isa8_hercules_device::device_reset()
{
	isa8_mda_device::device_reset();
	m_configuration_switch = 0;

	m_chr_gen = memregion(subtag("gfx1").c_str())->base();
}

/***************************************************************************
  Draw graphics with 720x348 pixels (default); so called Hercules gfx.
  The memory layout is divided into 4 banks where of size 0x2000.
  Every bank holds data for every n'th scanline, 8 pixels per byte,
  bit 7 being the leftmost.
***************************************************************************/

MC6845_UPDATE_ROW( isa8_hercules_device::hercules_gfx_update_row )
{
	rgb_t const *const palette = m_palette->palette()->entry_list_raw();
	uint32_t *p = &bitmap.pix(y);
	uint16_t const gfx_base = ((m_mode_control & 0x80) ? 0x8000 : 0x0000) | ((ra & 0x03) << 13);

	if (y == 0) LOGROW("%11.6f: %-24s\n", machine().time().as_double(), FUNCNAME);
	for (int i = 0; i < x_count; i++)
	{
		uint8_t data;

		data = m_videoram[gfx_base + ((ma + i) << 1)];

		*p++ = palette[(data & 0x80) ? 2 : 0];
		*p++ = palette[(data & 0x40) ? 2 : 0];
		*p++ = palette[(data & 0x20) ? 2 : 0];
		*p++ = palette[(data & 0x10) ? 2 : 0];
		*p++ = palette[(data & 0x08) ? 2 : 0];
		*p++ = palette[(data & 0x04) ? 2 : 0];
		*p++ = palette[(data & 0x02) ? 2 : 0];
		*p++ = palette[(data & 0x01) ? 2 : 0];

		data = m_videoram[gfx_base + ((ma + i) << 1) + 1];

		*p++ = palette[(data & 0x80) ? 2 : 0];
		*p++ = palette[(data & 0x40) ? 2 : 0];
		*p++ = palette[(data & 0x20) ? 2 : 0];
		*p++ = palette[(data & 0x10) ? 2 : 0];
		*p++ = palette[(data & 0x08) ? 2 : 0];
		*p++ = palette[(data & 0x04) ? 2 : 0];
		*p++ = palette[(data & 0x02) ? 2 : 0];
		*p++ = palette[(data & 0x01) ? 2 : 0];
	}
}


void isa8_hercules_device::mode_control_w(uint8_t data)
{
	m_mode_control = data;

	switch( m_mode_control & 0x2a )
	{
	case 0x08:
		m_update_row_type = MDA_TEXT_INTEN;
		break;
	case 0x28:
		m_update_row_type = MDA_TEXT_BLINK;
		break;
	case 0x0A:          /* Hercules modes */
	case 0x2A:
		m_update_row_type = HERCULES_GFX_BLINK;
		break;
	default:
		m_update_row_type = -1;
	}

	m_crtc->set_unscaled_clock( MDA_CLOCK / (m_mode_control & 0x02 ? 16 : 9) );
	m_crtc->set_hpixels_per_column( m_mode_control & 0x02 ? 16 : 9 );
}


void isa8_hercules_device::io_write(offs_t offset, uint8_t data)
{
	switch( offset )
	{
	case 0x00: case 0x02: case 0x04: case 0x06:
		m_crtc->address_w(data);
		break;
	case 0x01: case 0x03: case 0x05: case 0x07:
		m_crtc->register_w(data);
		break;
	case 0x08:
		mode_control_w(data);
		break;
	case 0x0c: case 0x0d:  case 0x0e:
		m_lpt->write(offset - 12, data);
		break;
	case 0x0f:
		m_configuration_switch = data;
		break;
	}
}


/*  R-  CRT status register (see #P139)
 *      (EGA/VGA) input status 1 register
 *      7    HGC vertical sync in progress
 *      6-4  adapter 000  hercules
 *                   001  hercules+
 *                   101  hercules InColor
 *                   else unknown
 *      3    pixel stream (0 black, 1 white)
 *      2-1  reserved
 *      0    horizontal drive enable
 */
uint8_t isa8_hercules_device::status_r()
{
	// Faking pixel stream here
	if (!machine().side_effects_disabled())
		m_pixel++;

	return m_vsync | ( m_pixel & 0x08 ) | m_hsync;
}


uint8_t isa8_hercules_device::io_read(offs_t offset)
{
	int data = 0xff;
	switch( offset )
	{
	case 0x00: case 0x02: case 0x04: case 0x06:
		/* return last written mc6845 address value here? */
		break;
	case 0x01: case 0x03: case 0x05: case 0x07:
		data = m_crtc->register_r();
		break;
	case 0x0a:
		data = status_r();
		break;
	/* LPT ports */
	case 0xc: case 0xd:  case 0xe:
		data = m_lpt->read(offset - 0x0c);
		break;
	}
	return data;
}

DEFINE_DEVICE_TYPE(ISA8_EC1840_0002, isa8_ec1840_0002_device, "ec1840_0002", "EC1840.0002 (MDA)")


//-------------------------------------------------
//  device_add_mconfig - add device configuration
//-------------------------------------------------

// XXX
void isa8_ec1840_0002_device::device_add_mconfig(machine_config &config)
{
	screen_device &screen(SCREEN(config, MDA_SCREEN_NAME, SCREEN_TYPE_RASTER));
	screen.set_raw(MDA_CLOCK, 792, 0, 640, 370, 0, 350);
	screen.set_screen_update(MC6845_NAME, FUNC(mc6845_device::screen_update));

	PALETTE(config, m_palette).set_entries(4);

	MC6845(config, m_crtc, MDA_CLOCK/8);
	m_crtc->set_screen(MDA_SCREEN_NAME);
	m_crtc->set_show_border_area(false);
	m_crtc->set_char_width(8);
	m_crtc->set_update_row_callback(FUNC(isa8_mda_device::crtc_update_row));
	m_crtc->out_hsync_callback().set(FUNC(isa8_mda_device::hsync_changed));
	m_crtc->out_vsync_callback().set(FUNC(isa8_mda_device::vsync_changed));
}

//-------------------------------------------------
//  isa8_ec1840_0002_device - constructor
//-------------------------------------------------

isa8_ec1840_0002_device::isa8_ec1840_0002_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	isa8_mda_device(mconfig, ISA8_EC1840_0002, tag, owner, clock), m_soft_chr_gen(nullptr)
{
}

//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void isa8_ec1840_0002_device::device_start()
{
	isa8_mda_device::device_start();

	m_soft_chr_gen = std::make_unique<uint8_t[]>(0x2000);
	m_isa->install_bank(0xdc000, 0xddfff, m_soft_chr_gen.get());
	m_isa->install_bank(0xde000, 0xdffff, m_soft_chr_gen.get());
}

void isa8_ec1840_0002_device::device_reset()
{
	isa8_mda_device::device_reset();

	m_chr_gen = m_soft_chr_gen.get();
}


/***************************************************************************
  Draw text mode with 80x25 characters (default) and intense background.
  The character cell size is 8x14.
***************************************************************************/

MC6845_UPDATE_ROW( isa8_ec1840_0002_device::mda_lowres_text_inten_update_row )
{
	rgb_t const *const palette = m_palette->palette()->entry_list_raw();
	uint32_t *p = &bitmap.pix(y);
	uint16_t const chr_base = ra;

	if (y == 0) LOGROW("%11.6f: %-24s\n", machine().time().as_double(), FUNCNAME);
	for (int i = 0; i < x_count; i++)
	{
		uint16_t const offset = ((ma + i) << 1) & 0x0fff;
		uint8_t const chr = m_videoram[offset];
		uint8_t const attr = m_videoram[offset + 1];
		uint8_t data = m_chr_gen[(chr_base + chr * 16) << 1];
		uint8_t fg = (attr & 0x08) ? 3 : 2;
		uint8_t bg = 0;

		if ((attr & ~0x88) == 0)
		{
			data = 0x00;
		}

		switch (attr)
		{
		case 0x70:
			bg = 2;
			fg = 0;
			break;
		case 0x78:
			bg = 2;
			fg = 1;
			break;
		case 0xf0:
			bg = 3;
			fg = 0;
			break;
		case 0xf8:
			bg = 3;
			fg = 1;
			break;
		}

		if ((i == cursor_x && (m_framecnt & 0x08)) || (attr & 0x07) == 0x01)
		{
			data = 0xff;
		}

		*p++ = palette[(data & 0x80) ? fg : bg];
		*p++ = palette[(data & 0x40) ? fg : bg];
		*p++ = palette[(data & 0x20) ? fg : bg];
		*p++ = palette[(data & 0x10) ? fg : bg];
		*p++ = palette[(data & 0x08) ? fg : bg];
		*p++ = palette[(data & 0x04) ? fg : bg];
		*p++ = palette[(data & 0x02) ? fg : bg];
		*p++ = palette[(data & 0x01) ? fg : bg];
	}
}


/***************************************************************************
  Draw text mode with 80x25 characters (default) and blinking characters.
  The character cell size is 8x14.
***************************************************************************/

MC6845_UPDATE_ROW( isa8_ec1840_0002_device::mda_lowres_text_blink_update_row )
{
	rgb_t const *const palette = m_palette->palette()->entry_list_raw();
	uint32_t *p = &bitmap.pix(y);
	uint16_t const chr_base = ra;

	if (y == 0) LOGROW("%11.6f: %-24s\n", machine().time().as_double(), FUNCNAME);
	for (int i = 0; i < x_count; i++)
	{
		uint16_t const offset = ((ma + i) << 1) & 0x0fff;
		uint8_t const chr = m_videoram[offset];
		uint8_t const attr = m_videoram[offset + 1];
		uint8_t data = m_chr_gen[(chr_base + chr * 16) << 1];
		uint8_t fg = (attr & 0x08) ? 3 : 2;
		uint8_t bg = 0;

		if ((attr & ~0x88) == 0)
		{
			data = 0x00;
		}

		switch (attr)
		{
		case 0x70:
		case 0xf0:
			bg = 2;
			fg = 0;
			break;
		case 0x78:
		case 0xf8:
			bg = 2;
			fg = 1;
			break;
		}

		if ((attr & 0x07) == 0x01)
		{
			data = 0xff;
		}

		if (i == cursor_x)
		{
			if (m_framecnt & 0x08)
			{
				data = 0xff;
			}
		}
		else
		{
			if ((attr & 0x80) && (m_framecnt & 0x10))
			{
				data = 0x00;
			}
		}

		*p++ = palette[(data & 0x80) ? fg : bg];
		*p++ = palette[(data & 0x40) ? fg : bg];
		*p++ = palette[(data & 0x20) ? fg : bg];
		*p++ = palette[(data & 0x10) ? fg : bg];
		*p++ = palette[(data & 0x08) ? fg : bg];
		*p++ = palette[(data & 0x04) ? fg : bg];
		*p++ = palette[(data & 0x02) ? fg : bg];
		*p++ = palette[(data & 0x01) ? fg : bg];
	}
}

void isa8_ec1840_0002_device::mode_control_w(uint8_t data)
{
	m_mode_control = data;

	switch( m_mode_control & 0x2a )
	{
	case 0x08:
		m_update_row_type = MDA_LOWRES_TEXT_INTEN;
		break;
	case 0x28:
		m_update_row_type = MDA_LOWRES_TEXT_BLINK;
		break;
	default:
		m_update_row_type = -1;
	}
}

MC6845_UPDATE_ROW( isa8_ec1840_0002_device::crtc_update_row )
{
	if (m_update_row_type == -1)
		return;

	switch (m_update_row_type)
	{
		case MDA_LOWRES_TEXT_INTEN:
			mda_lowres_text_inten_update_row(bitmap, cliprect, ma, ra, y, x_count, cursor_x, de, hbp, vbp);
			break;
		case MDA_LOWRES_TEXT_BLINK:
			mda_lowres_text_blink_update_row(bitmap, cliprect, ma, ra, y, x_count, cursor_x, de, hbp, vbp);
			break;
	}
}
