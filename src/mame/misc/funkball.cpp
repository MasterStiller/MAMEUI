// license:BSD-3-Clause
// copyright-holders:Angelo Salese, Philip Bennett

// dgPIX 'VRender 2 Beta Rev4' hardware
// MEDIAGX CPU + 3dFX VooDoo chipset
/***************************************************************************

driver by Angelo Salese & Phil Bennett

Notes:
-

Funky Ball
dgPIX, 1998

PCB Layout
----------

VRender 2Beta Rev4
  |--------------------------------------------------------------------|
  |TDA2005  14.31818MHz       |---------|   KM416C254 KM416C254        |
  |VOL KDA0340D    |-------|  |3DFX     |                              |
  |4558            |GENDAC |  |500-0004-02                             |
  |                |ICS5342|  |F004221.1|                              |
  |                |       |  |TMU      |                    KM416S1020|
  |                |-------|  |---------|   KM416C254 KM416C254        |
  |SERVICE_SW |---SUB---|                                              |
  |           |         |                                              |
  |           |FLASH.U3 |     |-----------|                            |
|-|           |         |     |3DFX       |                  KM416S1020|
|             |         |     |500-0003-03| KM416C254 KM416C254        |
|             |         |     |F006531.1  |                            |
|             |         |     |FBI        |                            |
|J            |         |     |           | KM416C254 KM416C254        |
|A            |         |     |-----------|                            |
|M            |         |RESET_SW                                      |
|M            |         |                                              |
|A            |---------|                     |-------------|          |
|      512K-EPR.U62       14.31818MHz         |Cyrix        |KM416S1020|
|                            |---------|      |GX MEDIA     |          |
|    |-------|  |------|     |LSI      |      |GXm-233GP    |          |
|    |XILINX |  |KS0164|     |L2A0788  |      |             |          |
|-|  |XCS05  |  |      |     |Cyrix    |      |             |          |
  |  |       |  |------|     |CX5520   |      |             |          |
  |  |-------| 16.9344MHz    |---------|      |-------------|KM416S1020|
  | LED               DIP20                                            |
  | |--------------FLASH-DAUGHTERBOARD----------------|                |
  | |                                                 |                |
  | |           FLASH.U30 FLASH.U29              DIP20|                |
  | |-------------------------------------------------|                |
  |--------------------------------------------------------------------|
Notes:
      Cyrix GXm233 - Main CPU; Cyrix GX Media running at 233MHz. Clock is generated by the Cyrix CX5520
                     and a 14.31818MHz xtal. That gives a 66.6MHz bus clock with a 3.5X multiplier for 233MHz
      Cyrix CX5520 - CPU-support chipset (BGA IC)
      FLASH.U29/30 - Intel Strata-Flash DA28F320J5 SSOP56 contained on a plug-in daughterboard; main program / gfx
      FLASH.U3     - Intel Strata-Flash DA28F320J5 SSOP56 contained on a plug-in daughterboard; sound program
      KS0164       - Samsung Electronics KS0164 General Midi compliant 32-voice Wavetable Synthesizer chip
                     with built-in 16bit CPU and MPU-401 compatibility (QFP100)
      512K-EPR     - 512k EPROM, boot-loader program. EPROM is tied to the KS0164 and the XCS05
      DIP20        - not-populated sockets
      KDA0340D     - Samsung KDA0340D CMOS low-power two-channel digital-to-analog converter (SOP28)
      KM416S1020   - Samsung 1M x16 SDRAM (x4, TSSOP50)
      KM416C254    - Samsung 256k x16 DRAM (x8, SOJ40)
      ICS5342      - combination dual programmable clock generator, 256bytes x18-bit RAM and a triple 8-bit video DAC (PLCC68)
      XCS05        - Xilinx Spartan XCS05 FPGA (PLCC84)

***************************************************************************/



#include "emu.h"

#include "pcshare.h"

#include "bus/rs232/rs232.h"
#include "cpu/i386/i386.h"
#include "machine/bankdev.h"
#include "machine/idectrl.h"
#include "machine/ins8250.h"
#include "machine/intelfsh.h"
#include "machine/lpci.h"
#include "machine/pckeybrd.h"
#include "sound/ks0164.h"
#include "video/voodoo.h"

#include "screen.h"
#include "speaker.h"


namespace {

class funkball_state : public pcat_base_state
{
public:
	funkball_state(const machine_config &mconfig, device_type type, const char *tag)
		: pcat_base_state(mconfig, type, tag)
		, m_voodoo(*this, "voodoo_0")
		, m_sound(*this, "ks0164")
		, m_unk_ram(*this, "unk_ram")
		, m_flashbank(*this, "flashbank")
		, m_inputs(*this, "IN.%u", 0)
	{ }

	void funkball(machine_config &config);

private:
	uint8_t m_funkball_config_reg_sel = 0;
	uint8_t m_funkball_config_regs[256]{};
	uint32_t m_cx5510_regs[256/4]{};
	std::unique_ptr<uint8_t[]> m_bios_ram;

	uint32_t m_biu_ctrl_reg[256/4]{};

	uint32_t flashbank_addr = 0;
	uint16_t m_latched_timer = 0;

	// devices
	required_device<voodoo_1_device> m_voodoo;
	required_device<ks0164_device> m_sound;

	required_shared_ptr<uint32_t> m_unk_ram;
	required_device<address_map_bank_device> m_flashbank;
	required_ioport_array<14> m_inputs;

	void flash_w (offs_t offset, uint32_t data, uint32_t mem_mask = ~0);
//  void bios_ram_w(offs_t offset, uint8_t data);
	uint8_t in_r(offs_t offset);
	uint8_t timer_r(offs_t offset);

	uint8_t funkball_config_reg_r();
	void funkball_config_reg_w(uint8_t data);

	virtual void video_start() override;
	uint32_t screen_update(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);

	struct
	{
		/* PCI */
		uint32_t command = 0;
		uint32_t base_addr = 0;

		uint32_t init_enable = 0;
	} m_voodoo_pci_regs;
	uint32_t biu_ctrl_r(offs_t offset);
	void biu_ctrl_w(offs_t offset, uint32_t data, uint32_t mem_mask = ~0);
	void bios_ram_w(offs_t offset, uint8_t data);
	uint8_t io20_r(offs_t offset);
	void io20_w(offs_t offset, uint8_t data);
	virtual void machine_start() override;
	virtual void machine_reset() override;
	void flashbank_map(address_map &map);
	void funkball_io(address_map &map);
	void funkball_map(address_map &map);

	uint32_t voodoo_0_pci_r(int function, int reg, uint32_t mem_mask);
	void voodoo_0_pci_w(int function, int reg, uint32_t data, uint32_t mem_mask);
	uint32_t cx5510_pci_r(int function, int reg, uint32_t mem_mask);
	void cx5510_pci_w(int function, int reg, uint32_t data, uint32_t mem_mask);
};

uint8_t funkball_state::timer_r(offs_t offset)
{
	if (offset == 0)
	{
		// exact frequency unknown, but the code checks against 32539 for a timeout
		// so guessing that could be it
		m_latched_timer = machine().time().as_ticks(32539);
		return m_latched_timer >> 8;
	}

	// the game polls this timer frequently; eat cycles as a cheap speedup
	m_maincpu->eat_cycles(500);
	return m_latched_timer;
}

void funkball_state::video_start()
{
}

uint32_t funkball_state::screen_update( screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect )
{
	return m_voodoo->update(bitmap, cliprect) ? 0 : UPDATE_HAS_NOT_CHANGED;
}

uint32_t funkball_state::voodoo_0_pci_r(int function, int reg, uint32_t mem_mask)
{
	uint32_t val = 0;

	printf("Voodoo PCI R: %x\n", reg);

	switch (reg)
	{
		case 0:
			val = 0x0001121a;
			break;
		case 0x10:
			val = m_voodoo_pci_regs.base_addr;
			break;
		case 0x40:
			val = m_voodoo_pci_regs.init_enable;
			break;
	}
	return val;
}

void funkball_state::voodoo_0_pci_w(int function, int reg, uint32_t data, uint32_t mem_mask)
{
	printf("Voodoo [%x]: %x\n", reg, data);

	switch (reg)
	{
		case 0x04:
			m_voodoo_pci_regs.command = data & 0x3;
			break;
		case 0x10:
			if (data == 0xffffffff)
				m_voodoo_pci_regs.base_addr = 0xff000000;
			else
				m_voodoo_pci_regs.base_addr = data;
			break;
		case 0x40:
			m_voodoo_pci_regs.init_enable = data;
			m_voodoo->set_init_enable(data);
			break;
	}
}

uint32_t funkball_state::cx5510_pci_r(int function, int reg, uint32_t mem_mask)
{
	//osd_printf_debug("CX5510: PCI read %d, %02X, %08X\n", function, reg, mem_mask);
	switch (reg)
	{
		case 0:     return 0x00001078;
	}

	return m_cx5510_regs[reg/4];
}

void funkball_state::cx5510_pci_w(int function, int reg, uint32_t data, uint32_t mem_mask)
{
	//osd_printf_debug("CX5510: PCI write %d, %02X, %08X, %08X\n", function, reg, data, mem_mask);
	COMBINE_DATA(&m_cx5510_regs[reg/4]);
}

uint8_t funkball_state::funkball_config_reg_r()
{
	//osd_printf_debug("funkball_config_reg_r %02X\n", funkball_config_reg_sel);
	return m_funkball_config_regs[m_funkball_config_reg_sel];
}

void funkball_state::funkball_config_reg_w(uint8_t data)
{
	//osd_printf_debug("funkball_config_reg_w %02X, %02X\n", funkball_config_reg_sel, data);
	m_funkball_config_regs[m_funkball_config_reg_sel] = data;
}

uint8_t funkball_state::io20_r(offs_t offset)
{
	uint8_t r = 0;

	// 0x22, 0x23, Cyrix configuration registers
	if (offset == 0x00)
	{
	}
	else if (offset == 0x01)
	{
		r = funkball_config_reg_r();
	}
	return r;
}

void funkball_state::io20_w(offs_t offset, uint8_t data)
{
	// 0x22, 0x23, Cyrix configuration registers
	if (offset == 0x00)
	{
		m_funkball_config_reg_sel = data;
	}
	else if (offset == 0x01)
	{
		funkball_config_reg_w(data);
	}
}

void funkball_state::flash_w(offs_t offset, uint32_t data, uint32_t mem_mask)
{
	COMBINE_DATA(&flashbank_addr);
	int tempbank = (flashbank_addr & 0x7fff) | ((flashbank_addr & 0x00800000) >> 8);
	m_flashbank->set_bank(tempbank);

	// note, other bits get used, but ignoring to keep the virtual bank space size sane.

}


uint32_t funkball_state::biu_ctrl_r(offs_t offset)
{
	if (offset == 0)
	{
		return 0xffffff;
	}
	return m_biu_ctrl_reg[offset];
}

void funkball_state::biu_ctrl_w(offs_t offset, uint32_t data, uint32_t mem_mask)
{
	//osd_printf_debug("biu_ctrl_w %08X, %08X, %08X\n", data, offset, mem_mask);
	COMBINE_DATA(m_biu_ctrl_reg + offset);

	if (offset == 0x0c/4)       // BC_XMAP_3 register
	{
		const char *const banknames[8] = { "bios_ext1", "bios_ext2", "bios_ext3","bios_ext4", "bios_bank1", "bios_bank2", "bios_bank3", "bios_bank4" };
		int i;

		for(i=0;i<8;i++)
		{
			if (data & 0x1 << i*4)      // enable RAM access to region 0xe0000 - 0xfffff
				membank(banknames[i])->set_base(m_bios_ram.get() + (0x4000 * i));
			else                    // disable RAM access (reads go to BIOS ROM)
				membank(banknames[i])->set_base(memregion("bios")->base() + (0x4000 * i));
		}
	}
}

void funkball_state::bios_ram_w(offs_t offset, uint8_t data)
{
	if(m_biu_ctrl_reg[0x0c/4] & (2 << ((offset & 0x4000)>>14)*4)) // memory is write-able
	{
		m_bios_ram[offset] = data;
	}
}

uint8_t funkball_state::in_r(offs_t offset)
{
	return m_inputs[offset]->read();
}

void funkball_state::funkball_map(address_map &map)
{
	map(0x00000000, 0x0009ffff).ram();
	map(0x000a0000, 0x000affff).ram();
	map(0x000b0000, 0x000bffff).m(m_flashbank, FUNC(address_map_bank_device::amap32));
	map(0x000c0000, 0x000cffff).ram();
	map(0x000d0000, 0x000dffff).ram();
	map(0x000e0000, 0x000e3fff).bankr("bios_ext1");
	map(0x000e4000, 0x000e7fff).bankr("bios_ext2");
	map(0x000e8000, 0x000ebfff).bankr("bios_ext3");
	map(0x000ec000, 0x000effff).bankr("bios_ext4");
	map(0x000f0000, 0x000f3fff).bankr("bios_bank1");
	map(0x000f4000, 0x000f7fff).bankr("bios_bank2");
	map(0x000f8000, 0x000fbfff).bankr("bios_bank3");
	map(0x000fc000, 0x000fffff).bankr("bios_bank4");
	map(0x000e0000, 0x000fffff).w(FUNC(funkball_state::bios_ram_w));
	map(0x00100000, 0x07ffffff).ram();
//  map(0x08000000, 0x0fffffff).noprw();
	map(0x40008000, 0x400080ff).rw(FUNC(funkball_state::biu_ctrl_r), FUNC(funkball_state::biu_ctrl_w));
	map(0x40010e00, 0x40010eff).ram().share("unk_ram");
	map(0xff000000, 0xfffdffff).rw(m_voodoo, FUNC(generic_voodoo_device::read), FUNC(generic_voodoo_device::write));
	map(0xfffe0000, 0xffffffff).rom().region("bios", 0);    /* System BIOS */
}

void funkball_state::flashbank_map(address_map &map)
{
	map(0x00000000, 0x003fffff).rw("u29", FUNC(intel_28f320j5_device::read), FUNC(intel_28f320j5_device::write)); // needed to boot
	map(0x00400000, 0x007fffff).rw("u30", FUNC(intel_28f320j5_device::read), FUNC(intel_28f320j5_device::write)); // i assume it maps directly after
//  map(0x02000000, 0x023fffff).rw("u3", FUNC(intel_28f320j5_device::read), FUNC(intel_28f320j5_device::write)); // sound program, don't think it matters where we map it, might not even be visible in this space
	/* it checks for 64MBit chips at 0x80000000 the way things are set up, they must return an intel Flash ID of 0x15 */
}

void funkball_state::funkball_io(address_map &map)
{
	pcat32_io_common(map);
	map(0x0022, 0x0023).rw(FUNC(funkball_state::io20_r), FUNC(funkball_state::io20_w));
	map(0x00e8, 0x00ef).noprw();

	map(0x01f0, 0x01f7).rw("ide", FUNC(ide_controller_device::cs0_r), FUNC(ide_controller_device::cs0_w));
	map(0x03f0, 0x03f7).rw("ide", FUNC(ide_controller_device::cs1_r), FUNC(ide_controller_device::cs1_w));
	map(0x03f8, 0x03ff).rw("uart", FUNC(ns16550_device::ins8250_r), FUNC(ns16550_device::ins8250_w));

	map(0x0cf8, 0x0cff).rw("pcibus", FUNC(pci_bus_legacy_device::read), FUNC(pci_bus_legacy_device::write));

	map(0x0320, 0x0323).rw(m_sound, FUNC(ks0164_device::mpu401_data_r), FUNC(ks0164_device::mpu401_data_w)).umask32(0x000000ff);
	map(0x0320, 0x0323).rw(m_sound, FUNC(ks0164_device::mpu401_status_r), FUNC(ks0164_device::mpu401_ctrl_w)).umask32(0x0000ff00);

	map(0x0360, 0x0363).w(FUNC(funkball_state::flash_w));

//  map(0x0320, 0x0323).r(FUNC(funkball_state::test_r));
	map(0x0360, 0x036d).r(FUNC(funkball_state::in_r)); // inputs
	map(0x036e, 0x036f).r(FUNC(funkball_state::timer_r)); // inputs
}

static INPUT_PORTS_START( funkball )
	PORT_START("IN.0")
	PORT_DIPNAME( 0x01, 0x01, "0" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_START("IN.1")
	PORT_DIPNAME( 0x01, 0x01, "1" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_START("IN.2")
	PORT_DIPNAME( 0x01, 0x01, "2" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_START("IN.3")
	PORT_DIPNAME( 0x01, 0x01, "3" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_START("IN.4")
	PORT_DIPNAME( 0x01, 0x01, "4" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_START("IN.5")
	PORT_DIPNAME( 0x01, 0x01, "5" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START("IN.6")
	PORT_DIPNAME( 0x01, 0x01, "6" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	/* 7-8 P1/P2 E-F "dgDelay" */
	PORT_START("IN.7")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP    ) PORT_PLAYER(1)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  ) PORT_PLAYER(1)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT  ) PORT_PLAYER(1)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_PLAYER(1)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1        ) PORT_PLAYER(1)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2        ) PORT_PLAYER(1)
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON3        ) PORT_PLAYER(1)
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON4        ) PORT_PLAYER(1)

	PORT_START("IN.8")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP    ) PORT_PLAYER(2)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN  ) PORT_PLAYER(2)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT  ) PORT_PLAYER(2)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_PLAYER(2)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1        ) PORT_PLAYER(2)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2        ) PORT_PLAYER(2)
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON3        ) PORT_PLAYER(2)
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON4        ) PORT_PLAYER(2)

	PORT_START("IN.9")
	PORT_DIPNAME( 0x01, 0x01, "9" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	// keeping pressed either one of these two at POST makes it to go into service mode
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_START2 )


	PORT_START("IN.10")
	PORT_DIPNAME( 0x01, 0x01, "10" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_START("IN.11")
	PORT_DIPNAME( 0x01, 0x01, "11" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_START("IN.12")
	PORT_DIPNAME( 0x01, 0x01, "12" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_START("IN.13")
	PORT_DIPNAME( 0x01, 0x01, "13" )
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
INPUT_PORTS_END

void funkball_state::machine_start()
{
	m_bios_ram = std::make_unique<uint8_t[]>(0x20000);

	/* defaults, otherwise it won't boot */
	m_unk_ram[0x010/4] = 0x2f8d85ff;
	m_unk_ram[0x018/4] = 0x000018c5;
}

void funkball_state::machine_reset()
{
	membank("bios_ext1")->set_base(memregion("bios")->base() + 0x00000);
	membank("bios_ext2")->set_base(memregion("bios")->base() + 0x04000);
	membank("bios_ext3")->set_base(memregion("bios")->base() + 0x08000);
	membank("bios_ext4")->set_base(memregion("bios")->base() + 0x0c000);
	membank("bios_bank1")->set_base(memregion("bios")->base() + 0x10000);
	membank("bios_bank2")->set_base(memregion("bios")->base() + 0x14000);
	membank("bios_bank3")->set_base(memregion("bios")->base() + 0x18000);
	membank("bios_bank4")->set_base(memregion("bios")->base() + 0x1c000);
	m_voodoo_pci_regs.base_addr = 0xff000000;
}

static DEVICE_INPUT_DEFAULTS_START( terminal )
	DEVICE_INPUT_DEFAULTS( "RS232_RXBAUD", 0xff, RS232_BAUD_57600 )
	DEVICE_INPUT_DEFAULTS( "RS232_TXBAUD", 0xff, RS232_BAUD_57600 )
	DEVICE_INPUT_DEFAULTS( "RS232_DATABITS", 0xff, RS232_DATABITS_8 )
	DEVICE_INPUT_DEFAULTS( "RS232_PARITY", 0xff, RS232_PARITY_NONE )
	DEVICE_INPUT_DEFAULTS( "RS232_STOPBITS", 0xff, RS232_STOPBITS_1 )
	DEVICE_INPUT_DEFAULTS( "TERM_CONF", 0x080, 0x080 ) // Auto LF on CR
DEVICE_INPUT_DEFAULTS_END

void funkball_state::funkball(machine_config &config)
{
	MEDIAGX(config, m_maincpu, 66666666*3.5); // 66,6 MHz x 3.5
	m_maincpu->set_addrmap(AS_PROGRAM, &funkball_state::funkball_map);
	m_maincpu->set_addrmap(AS_IO, &funkball_state::funkball_io);
	m_maincpu->set_irq_acknowledge_callback("pic8259_1", FUNC(pic8259_device::inta_cb));

	pcat_common(config);

	pci_bus_legacy_device &pcibus(PCI_BUS_LEGACY(config, "pcibus", 0, 0));
	pcibus.set_device(7, FUNC(funkball_state::voodoo_0_pci_r), FUNC(funkball_state::voodoo_0_pci_w));
	pcibus.set_device(18, FUNC(funkball_state::cx5510_pci_r), FUNC(funkball_state::cx5510_pci_w));

	ide_controller_device &ide(IDE_CONTROLLER(config, "ide").options(ata_devices, "hdd", nullptr, true));
	ide.irq_handler().set("pic8259_2", FUNC(pic8259_device::ir6_w));

	ADDRESS_MAP_BANK(config, "flashbank").set_map(&funkball_state::flashbank_map).set_options(ENDIANNESS_LITTLE, 32, 32, 0x10000);

	/* video hardware */
	VOODOO_1(config, m_voodoo, voodoo_1_device::NOMINAL_CLOCK);
	m_voodoo->set_fbmem(2);
	m_voodoo->set_tmumem(4, 0);
	m_voodoo->set_status_cycles(1000); // optimization to consume extra cycles when polling status
	m_voodoo->set_screen("screen");
	m_voodoo->set_cpu(m_maincpu);

	screen_device &screen(SCREEN(config, "screen", SCREEN_TYPE_RASTER));
	screen.set_refresh_hz(60);
	screen.set_vblank_time(ATTOSECONDS_IN_USEC(0));
	screen.set_screen_update(FUNC(funkball_state::screen_update));
	screen.set_size(1024, 1024);
	screen.set_visarea(0, 511, 16, 447);

	ns16550_device &uart(NS16550(config, "uart", 1843200)); // exact type unknown
	uart.out_tx_callback().set("rs232", FUNC(rs232_port_device::write_txd));
	uart.out_dtr_callback().set("rs232", FUNC(rs232_port_device::write_dtr));
	uart.out_rts_callback().set("rs232", FUNC(rs232_port_device::write_rts));

	rs232_port_device &rs232(RS232_PORT(config, "rs232", default_rs232_devices, "terminal"));
	rs232.set_option_device_input_defaults("terminal", DEVICE_INPUT_DEFAULTS_NAME(terminal));
	rs232.rxd_handler().set("uart", FUNC(ns16550_device::rx_w));
	rs232.dcd_handler().set("uart", FUNC(ns16550_device::dcd_w));
	rs232.dsr_handler().set("uart", FUNC(ns16550_device::dsr_w));
	rs232.cts_handler().set("uart", FUNC(ns16550_device::cts_w));

	INTEL_28F320J5(config, "u29");
	INTEL_28F320J5(config, "u30");
	INTEL_28F320J5(config, "u3");

	SPEAKER(config, "lspeaker").front_left();
	SPEAKER(config, "rspeaker").front_right();

	KS0164(config, m_sound, 16.9344_MHz_XTAL);
	m_sound->add_route(0, "lspeaker", 1.0);
	m_sound->add_route(1, "rspeaker", 1.0);
}

ROM_START( funkball )
	ROM_REGION32_LE(0x20000, "bios", ROMREGION_ERASEFF)
	ROM_LOAD( "512k-epr.u62", 0x010000, 0x010000, CRC(cced894a) SHA1(298c81716e375da4b7215f3e588a45ca3ea7e35c) )

	ROM_REGION(0x400000, "ks0164", ROMREGION_ERASE00) // Sound Program / Samples
	ROM_LOAD16_WORD_SWAP( "flash.u3", 0x000000, 0x400000, CRC(fb376abc) SHA1(ea4c48bb6cd2055431a33f5c426e52c7af6997eb) )

	ROM_REGION16_BE(0x400000, "u29", ROMREGION_ERASE00) // Main Program
	ROM_LOAD16_WORD_SWAP( "flash.u29",0x000000, 0x400000, CRC(7cf6ff4b) SHA1(4ccdd4864ad92cc218998f3923997119a1a9dd1d) )

	ROM_REGION16_BE(0x400000, "u30", ROMREGION_ERASE00)
	ROM_LOAD16_WORD_SWAP( "flash.u30",0x000000, 0x400000, CRC(1d46717a) SHA1(acfbd0a2ccf4d717779733c4a9c639296c3bbe0e) )
ROM_END

} // anonymous namespace


GAME(1998, funkball, 0, funkball, funkball, funkball_state, empty_init, ROT0, "dgPIX Entertainment Inc.", "Funky Ball", MACHINE_NOT_WORKING)