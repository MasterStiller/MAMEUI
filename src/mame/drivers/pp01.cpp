// license:BSD-3-Clause
// copyright-holders:Miodrag Milanovic
/***************************************************************************

        PP-01 driver by Miodrag Milanovic

        08/09/2008 Preliminary driver.

****************************************************************************/

#include "emu.h"
#include "includes/pp01.h"

#include "screen.h"
#include "speaker.h"


/* Address maps */
void pp01_state::pp01_mem(address_map &map)
{
	map(0x0000, 0x0fff).bankrw("bank1");
	map(0x1000, 0x1fff).bankrw("bank2");
	map(0x2000, 0x2fff).bankrw("bank3");
	map(0x3000, 0x3fff).bankrw("bank4");
	map(0x4000, 0x4fff).bankrw("bank5");
	map(0x5000, 0x5fff).bankrw("bank6");
	map(0x6000, 0x6fff).bankrw("bank7");
	map(0x7000, 0x7fff).bankrw("bank8");
	map(0x8000, 0x8fff).bankrw("bank9");
	map(0x9000, 0x9fff).bankrw("bank10");
	map(0xa000, 0xafff).bankrw("bank11");
	map(0xb000, 0xbfff).bankrw("bank12");
	map(0xc000, 0xcfff).bankrw("bank13");
	map(0xd000, 0xdfff).bankrw("bank14");
	map(0xe000, 0xefff).bankrw("bank15");
	map(0xf000, 0xffff).bankrw("bank16");
}

void pp01_state::pp01_io(address_map &map)
{
	map(0xc0, 0xc3).rw("ppi8255", FUNC(i8255_device::read), FUNC(i8255_device::write)); // system
	//AM_RANGE(0xc4, 0xc7) AM_DEVREADWRITE("ppi8255", i8255_device, read, write) // user
	map(0xc8, 0xc9).mirror(2).rw("uart", FUNC(i8251_device::read), FUNC(i8251_device::write));
	map(0xcc, 0xcf).w(FUNC(pp01_state::pp01_video_write_mode_w));
	map(0xd0, 0xd3).rw(m_pit, FUNC(pit8253_device::read), FUNC(pit8253_device::write));
	map(0xe0, 0xef).mirror(0x10).rw(FUNC(pp01_state::pp01_mem_block_r), FUNC(pp01_state::pp01_mem_block_w));
}

/* Input ports */
static INPUT_PORTS_START( pp01 )
	PORT_START("LINE0")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Space") PORT_CODE(KEYCODE_SPACE)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Home") PORT_CODE(KEYCODE_HOME)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num 0") PORT_CODE(KEYCODE_0_PAD)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("0") PORT_CODE(KEYCODE_0)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("P") PORT_CODE(KEYCODE_P)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINE1")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F1") PORT_CODE(KEYCODE_F1)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F2") PORT_CODE(KEYCODE_F2)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num 1") PORT_CODE(KEYCODE_1_PAD)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("1") PORT_CODE(KEYCODE_1)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("A") PORT_CODE(KEYCODE_A)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Q") PORT_CODE(KEYCODE_Q)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINE2")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F3") PORT_CODE(KEYCODE_F3)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F4") PORT_CODE(KEYCODE_F4)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num 2") PORT_CODE(KEYCODE_2_PAD)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("2") PORT_CODE(KEYCODE_2)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("B") PORT_CODE(KEYCODE_B)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("R") PORT_CODE(KEYCODE_R)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINE3")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F5") PORT_CODE(KEYCODE_F5)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F6") PORT_CODE(KEYCODE_F6)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num 3") PORT_CODE(KEYCODE_3_PAD)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("3") PORT_CODE(KEYCODE_3)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("C") PORT_CODE(KEYCODE_C)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("S") PORT_CODE(KEYCODE_S)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINE4")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F7") PORT_CODE(KEYCODE_F7)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F8") PORT_CODE(KEYCODE_F8)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num 4") PORT_CODE(KEYCODE_4_PAD)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("4") PORT_CODE(KEYCODE_4)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("D") PORT_CODE(KEYCODE_D)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("T") PORT_CODE(KEYCODE_T)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINE5")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F9") PORT_CODE(KEYCODE_F9)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F10") PORT_CODE(KEYCODE_F10)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num 5") PORT_CODE(KEYCODE_5_PAD)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("5") PORT_CODE(KEYCODE_5)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("E") PORT_CODE(KEYCODE_E)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("U") PORT_CODE(KEYCODE_U)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINE6")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F11") PORT_CODE(KEYCODE_F11)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F12") PORT_CODE(KEYCODE_F12)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num 6") PORT_CODE(KEYCODE_6_PAD)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("6") PORT_CODE(KEYCODE_6)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F") PORT_CODE(KEYCODE_F)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("V") PORT_CODE(KEYCODE_V)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINE7")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F13") PORT_CODE(KEYCODE_PGUP)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("F14") PORT_CODE(KEYCODE_PGDN)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num 7") PORT_CODE(KEYCODE_7_PAD)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("7") PORT_CODE(KEYCODE_7)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("G") PORT_CODE(KEYCODE_G)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("W") PORT_CODE(KEYCODE_W)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINE8")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Backspace") PORT_CODE(KEYCODE_BACKSPACE)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Delete line") PORT_CODE(KEYCODE_TILDE)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num 8") PORT_CODE(KEYCODE_8_PAD)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("8") PORT_CODE(KEYCODE_8)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("H") PORT_CODE(KEYCODE_H)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("X") PORT_CODE(KEYCODE_X)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINE9")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num +") PORT_CODE(KEYCODE_PLUS_PAD)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num 9") PORT_CODE(KEYCODE_9_PAD)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("9") PORT_CODE(KEYCODE_9)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("I") PORT_CODE(KEYCODE_I)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Y") PORT_CODE(KEYCODE_Y)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINEA")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME(":") PORT_CODE(KEYCODE_QUOTE)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("J") PORT_CODE(KEYCODE_J)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Z") PORT_CODE(KEYCODE_Z)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINEB")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME(";") PORT_CODE(KEYCODE_COLON)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("K") PORT_CODE(KEYCODE_K)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("[") PORT_CODE(KEYCODE_OPENBRACE)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINEC")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num *") PORT_CODE(KEYCODE_ASTERISK)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num -") PORT_CODE(KEYCODE_MINUS_PAD)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME(",") PORT_CODE(KEYCODE_COMMA)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("L") PORT_CODE(KEYCODE_L)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("\\") PORT_CODE(KEYCODE_BACKSLASH)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINED")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Enter") PORT_CODE(KEYCODE_ENTER)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("-") PORT_CODE(KEYCODE_MINUS)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("M") PORT_CODE(KEYCODE_M)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("]") PORT_CODE(KEYCODE_CLOSEBRACE)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINEE")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num /") PORT_CODE(KEYCODE_SLASH_PAD)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("Num .") PORT_CODE(KEYCODE_DEL_PAD)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME(".") PORT_CODE(KEYCODE_STOP)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("N") PORT_CODE(KEYCODE_N)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("^") PORT_CODE(KEYCODE_TILDE)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINEF")
		PORT_BIT(0x01, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x02, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x04, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x08, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("/") PORT_CODE(KEYCODE_SLASH)
		PORT_BIT(0x10, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("O") PORT_CODE(KEYCODE_O)
		PORT_BIT(0x20, IP_ACTIVE_LOW, IPT_KEYBOARD) PORT_NAME("_") PORT_CODE(KEYCODE_HOME)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED)
	PORT_START("LINEALL")
		PORT_BIT(0x3F, IP_ACTIVE_LOW, IPT_UNUSED)
		PORT_BIT(0x40, IP_ACTIVE_LOW, IPT_UNUSED) PORT_NAME("Ctrl") PORT_CODE(KEYCODE_LCONTROL) PORT_CODE(KEYCODE_RCONTROL)
		PORT_BIT(0x80, IP_ACTIVE_LOW, IPT_UNUSED) PORT_NAME("Shift") PORT_CODE(KEYCODE_LSHIFT) PORT_CODE(KEYCODE_RSHIFT)
INPUT_PORTS_END

/* Machine driver */
MACHINE_CONFIG_START(pp01_state::pp01)
	/* basic machine hardware */
	MCFG_DEVICE_ADD("maincpu", I8080, 2000000)
	MCFG_DEVICE_PROGRAM_MAP(pp01_mem)
	MCFG_DEVICE_IO_MAP(pp01_io)

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(50)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(2500)) /* not accurate */
	MCFG_SCREEN_SIZE(256, 256)
	MCFG_SCREEN_VISIBLE_AREA(0, 256-1, 0, 256-1)
	MCFG_SCREEN_UPDATE_DRIVER(pp01_state, screen_update_pp01)
	MCFG_SCREEN_PALETTE("palette")

	PALETTE(config, "palette", FUNC(pp01_state::pp01_palette), 8);

	/* sound hardware */
	SPEAKER(config, "mono").front_center();
	SPEAKER_SOUND(config, "speaker").add_route(ALL_OUTPUTS, "mono", 0.50);
	//WAVE(config, "wave", "cassette").add_route(ALL_OUTPUTS, "mono", 0.25);

	/* Devices */
	MCFG_DEVICE_ADD("uart", I8251, 0)
	// when rts and dtr are both high, the uart is being used for cassette operations

	PIT8253(config, m_pit, 0);
	m_pit->set_clk<0>(0);
	m_pit->out_handler<0>().set(FUNC(pp01_state::pp01_pit_out0));
	m_pit->set_clk<1>(2000000);
	m_pit->out_handler<1>().set(FUNC(pp01_state::pp01_pit_out1));
	m_pit->set_clk<2>(2000000);
	m_pit->out_handler<2>().set(m_pit, FUNC(pit8253_device::write_clk0));

	i8255_device &ppi(I8255A(config, "ppi8255"));
	ppi.in_pa_callback().set(FUNC(pp01_state::pp01_8255_porta_r));
	ppi.out_pa_callback().set(FUNC(pp01_state::pp01_8255_porta_w));
	ppi.in_pb_callback().set(FUNC(pp01_state::pp01_8255_portb_r));
	ppi.out_pb_callback().set(FUNC(pp01_state::pp01_8255_portb_w));
	ppi.in_pc_callback().set(FUNC(pp01_state::pp01_8255_portc_r));
	ppi.out_pc_callback().set(FUNC(pp01_state::pp01_8255_portc_w));

	/* internal ram */
	RAM(config, RAM_TAG).set_default_size("64K").set_default_value(0x00);
MACHINE_CONFIG_END

/* ROM definition */

ROM_START( pp01 )
	ROM_REGION( 0x18000, "maincpu", ROMREGION_ERASEFF )

	ROM_LOAD( "ep01.rom", 0x14000, 0x0400, CRC(e48e440f) SHA1(aaae41f0bf163f254e28f6e4d9f6010b349e0c45))
	ROM_LOAD( "ep02.rom", 0x14400, 0x0400, CRC(b9314867) SHA1(220e2a686c6624574e8ea7f6012030c8378a8be6))
	ROM_LOAD( "ep03.rom", 0x14800, 0x0400, CRC(47b91d49) SHA1(51445e24d1c375e798c5ed7b48cb1c7b5e01603a))
	ROM_LOAD( "ep04.rom", 0x14c00, 0x0400, CRC(cc890123) SHA1(7110f301453d925da0d3f29c2ba4eacd5ff8583d))
	ROM_LOAD( "ep05.rom", 0x15000, 0x0400, CRC(b36e2071) SHA1(7c7b2c410f4039980627196575109426bb7bab35))
	ROM_LOAD( "ep06.rom", 0x15400, 0x0400, CRC(3f74efb3) SHA1(77d055eb53ba9b4b14eb2bb8d71f93095b846bb5))
	ROM_LOAD( "ep07.rom", 0x15800, 0x0400, CRC(1dd0bc29) SHA1(109bc3f24a76884d2581f75edc532917cb7ea155))
	ROM_LOAD( "ep08.rom", 0x15c00, 0x0400, CRC(607b710a) SHA1(c5ac7da31204c54d6892d6b708d0291d71963581))
	ROM_LOAD( "ep09.rom", 0x16000, 0x0400, CRC(07f773d1) SHA1(a6cf454318ef0a564a86e669b751e414b12c12d1))
	ROM_LOAD( "ep10.rom", 0x16400, 0x0400, CRC(7793d84b) SHA1(d16ad3db36c8cec68713082ee60324219d3d5032))
	ROM_LOAD( "ep11.rom", 0x16800, 0x0400, CRC(d1b98b5e) SHA1(5526af6317667d707ae413916242cd1bac821373))
	ROM_LOAD( "ep12.rom", 0x16c00, 0x0400, CRC(03c45326) SHA1(f9157ab86cdf1d111bcf19c24e4d7250705be965))
	ROM_LOAD( "ep13.rom", 0x17000, 0x0400, CRC(51769d6c) SHA1(76d6e91cf32b1eb94b9c0ded02e5057ed7ff7d72))
	ROM_LOAD( "ep14.rom", 0x17400, 0x0400, CRC(5d48cefb) SHA1(9b8599dabaf7297ec800fa9a122ad7e3b7b30e8b))
	ROM_LOAD( "ep15.rom", 0x17800, 0x0400, CRC(f44bfe36) SHA1(0ab3c4f061dff6f779685409b07343d0a839fdae))
	ROM_LOAD( "ep16.rom", 0x17c00, 0x0400, CRC(f39c230d) SHA1(9789cedef5655bcb82561f387cc1a6cf48287d78))
ROM_END


/* Driver */

//    YEAR  NAME  PARENT  COMPAT  MACHINE  INPUT  CLASS       INIT        COMPANY  FULLNAME  FLAGS
COMP( 198?, pp01, 0,      0,      pp01,    pp01,  pp01_state, empty_init, "ZVT",   "PP-01",  MACHINE_NOT_WORKING )
