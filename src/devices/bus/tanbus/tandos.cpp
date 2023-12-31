// license:BSD-3-Clause
// copyright-holders:Nigel Barnes
/**********************************************************************

    Tangerine TANDOS (MT0078)

    http://www.microtan.ukpc.net/pageProducts.html#DOS

**********************************************************************/


#include "emu.h"
#include "tandos.h"


//**************************************************************************
//  DEVICE DEFINITIONS
//**************************************************************************

DEFINE_DEVICE_TYPE(TANBUS_TANDOS, tanbus_tandos_device, "tanbus_tandos", "Tangerine Tandos Board")

//-------------------------------------------------
//  SLOT_INTERFACE( tandos_floppies )
//-------------------------------------------------

static void tandos_floppies(device_slot_interface &device)
{
	device.option_add("525sssd", FLOPPY_525_SSSD);
	device.option_add("525sd", FLOPPY_525_SD);
	device.option_add("525qd", FLOPPY_525_QD);
}

//-------------------------------------------------
//  ROM( tandos )
//-------------------------------------------------

ROM_START(tandos)
	ROM_REGION(0x1000, "dos_rom", 0)
	ROM_DEFAULT_BIOS("step30")
	ROM_SYSTEM_BIOS(0, "step30", "30ms Stepping Rate")
	ROMX_LOAD("tandos_sr30.f2", 0x0000, 0x1000, CRC(54f55771) SHA1(5a801039fa8c05cd9227e9138469959524516c9e), ROM_BIOS(0))
	ROM_SYSTEM_BIOS(1, "step6", "6ms Stepping Rate")
	ROMX_LOAD("tandos_sr6.f2", 0x0000, 0x1000, CRC(10ef90fa) SHA1(b1e42cbf7197e693073d2578561803ea4c8efa8c), ROM_BIOS(1))
ROM_END

//-------------------------------------------------
//  device_add_mconfig - add device configuration
//-------------------------------------------------

void tanbus_tandos_device::device_add_mconfig(machine_config &config)
{
	FD1793(config, m_fdc, 8_MHz_XTAL / 8);
	m_fdc->intrq_wr_callback().set(FUNC(tanbus_tandos_device::fdc_irq_w));
	m_fdc->drq_wr_callback().set(FUNC(tanbus_tandos_device::fdc_drq_w));
	m_fdc->hld_wr_callback().set(FUNC(tanbus_tandos_device::fdc_hld_w));
	m_fdc->set_force_ready(true);

	FLOPPY_CONNECTOR(config, m_floppies[0], tandos_floppies, "525qd", floppy_image_device::default_mfm_floppy_formats).enable_sound(true);
	FLOPPY_CONNECTOR(config, m_floppies[1], tandos_floppies, "525qd", floppy_image_device::default_mfm_floppy_formats).enable_sound(true);
	FLOPPY_CONNECTOR(config, m_floppies[2], tandos_floppies, nullptr, floppy_image_device::default_mfm_floppy_formats).enable_sound(true);
	FLOPPY_CONNECTOR(config, m_floppies[3], tandos_floppies, nullptr, floppy_image_device::default_mfm_floppy_formats).enable_sound(true);

	TMS9914(config, m_tms9914, 8_MHz_XTAL / 2);
	m_tms9914->dio_read_cb().set(IEEE488_TAG, FUNC(ieee488_device::dio_r));
	m_tms9914->dio_write_cb().set(IEEE488_TAG, FUNC(ieee488_device::host_dio_w));
	m_tms9914->eoi_write_cb().set(IEEE488_TAG, FUNC(ieee488_device::host_eoi_w));
	m_tms9914->dav_write_cb().set(IEEE488_TAG, FUNC(ieee488_device::host_dav_w));
	m_tms9914->nrfd_write_cb().set(IEEE488_TAG, FUNC(ieee488_device::host_nrfd_w));
	m_tms9914->ndac_write_cb().set(IEEE488_TAG, FUNC(ieee488_device::host_ndac_w));
	m_tms9914->ifc_write_cb().set(IEEE488_TAG, FUNC(ieee488_device::host_ifc_w));
	m_tms9914->srq_write_cb().set(IEEE488_TAG, FUNC(ieee488_device::host_srq_w));
	m_tms9914->atn_write_cb().set(IEEE488_TAG, FUNC(ieee488_device::host_atn_w));
	m_tms9914->ren_write_cb().set(IEEE488_TAG, FUNC(ieee488_device::host_ren_w));

	IEEE488(config, m_ieee);
	m_ieee->eoi_callback().set(m_tms9914, FUNC(tms9914_device::eoi_w));
	m_ieee->dav_callback().set(m_tms9914, FUNC(tms9914_device::dav_w));
	m_ieee->nrfd_callback().set(m_tms9914, FUNC(tms9914_device::nrfd_w));
	m_ieee->ndac_callback().set(m_tms9914, FUNC(tms9914_device::ndac_w));
	m_ieee->ifc_callback().set(m_tms9914, FUNC(tms9914_device::ifc_w));
	m_ieee->srq_callback().set(m_tms9914, FUNC(tms9914_device::srq_w));
	m_ieee->atn_callback().set(m_tms9914, FUNC(tms9914_device::atn_w));
	m_ieee->ren_callback().set(m_tms9914, FUNC(tms9914_device::ren_w));
	IEEE488_SLOT(config, "ieee_dev", 0, cbm_ieee488_devices, nullptr);
}

const tiny_rom_entry *tanbus_tandos_device::device_rom_region() const
{
	return ROM_NAME(tandos);
}

//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  tanbus_tandos_device - constructor
//-------------------------------------------------

tanbus_tandos_device::tanbus_tandos_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: device_t(mconfig, TANBUS_TANDOS, tag, owner, clock)
	, device_tanbus_interface(mconfig, *this)
	, m_dos_rom(*this, "dos_rom")
	, m_ieee(*this, IEEE488_TAG)
	, m_tms9914(*this, "hpib")
	, m_fdc(*this, "fdc")
	, m_floppies(*this, "fdc:%u", 0)
	, m_floppy(nullptr)
	, m_status(0)
{
}


//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void tanbus_tandos_device::device_start()
{
	m_ram = std::make_unique<uint8_t[]>(0x0400);

	save_pointer(NAME(m_ram), 0x0400);
}

//-------------------------------------------------
//  device_reset - device-specific reset
//-------------------------------------------------

void tanbus_tandos_device::device_reset()
{
	// reset floppy control register
	control_w(0);
}

//-------------------------------------------------
//  read - card read
//-------------------------------------------------

uint8_t tanbus_tandos_device::read(offs_t offset, int inhrom, int inhram, int be)
{
	uint8_t data = 0xff;

	switch (offset & 0xfc00)
	{
	case 0xa800: case 0xac00: case 0xb000: case 0xb400:
		data = m_dos_rom->base()[offset & 0x0fff];
		break;

	case 0xb800:
		data = m_ram[offset & 0x03ff];
		break;

	default:
		switch (offset)
		{
		case 0xbf90: case 0xbf91: case 0xbf92: case 0xbf93:
			data = m_fdc->read(offset & 0x03);
			break;
		case 0xbf94: case 0xbf96:
			data = status_r();
			break;
		case 0xbf95: case 0xbf97:
			// GPIB PCB Switches
			break;
		case 0xbf98: case 0xbf99: case 0xbf9a: case 0xbf9b: case 0xbf9c: case 0xbf9d: case 0xbf9e: case 0xbf9f:
			data = m_tms9914->read(offset & 0x07);
			break;
		}
		break;
	}

	return data;
}

//-------------------------------------------------
//  write - card write
//-------------------------------------------------

void tanbus_tandos_device::write(offs_t offset, uint8_t data, int inhrom, int inhram, int be)
{
	switch (offset & 0xfc00)
	{
	case 0xb800:
		m_ram[offset & 0x03ff] = data;
		break;

	default:
		switch (offset)
		{
		case 0xbf90: case 0xbf91: case 0xbf92: case 0xbf93:
			m_fdc->write(offset & 0x03, data);
			break;
		case 0xbf94: case 0xbf96:
			control_w(data);
			break;
		case 0xbf98: case 0xbf99: case 0xbf9a: case 0xbf9b: case 0xbf9c: case 0xbf9d: case 0xbf9e: case 0xbf9f:
			m_tms9914->write(offset & 0x07, data);
			break;
		}
		break;
	}
}

//-------------------------------------------------
//  set_inhibit_lines
//-------------------------------------------------

void tanbus_tandos_device::set_inhibit_lines(offs_t offset, int &inhram, int &inhrom)
{
	if (offset >= 0xa800 && offset < 0xbc00)
	{
		inhram = 1;
		inhrom = 1;
	}
};

//**************************************************************************
//  IMPLEMENTATION
//**************************************************************************

void tanbus_tandos_device::control_w(uint8_t data)
{
	m_status = data & 0x3e;

	// bit 0: irq enable
	m_irq_enable = BIT(data, 0);

	// bit 1: data select (data stream controller)

	// bit 2, 3: drive select
	m_floppy = m_floppies[BIT(data, 2, 2)]->get_device();
	m_fdc->set_floppy(m_floppy);

	// bit 4: side select
	if (m_floppy)
		m_floppy->ss_w(BIT(data, 4));

	// bit 5: density
	m_fdc->dden_w(BIT(data, 5));

	// bit 6: head load timing
	m_fdc->hlt_w(BIT(data, 6));
	if (m_floppy)
		m_floppy->mon_w(!BIT(data, 6));

	// bit 7: drq enable
	m_drq_enable = BIT(data, 7);
}

uint8_t tanbus_tandos_device::status_r()
{
	return m_status | (m_fdc->drq_r() << 7) | (m_fdc->hld_r() << 6) | (m_fdc->intrq_r() << 0);
}


void tanbus_tandos_device::fdc_drq_w(int state)
{
	m_tanbus->so_w((m_drq_enable && state) ? ASSERT_LINE : CLEAR_LINE);
}

void tanbus_tandos_device::fdc_irq_w(int state)
{
	m_tanbus->irq_w((m_irq_enable && state) ? ASSERT_LINE : CLEAR_LINE);
}

void tanbus_tandos_device::fdc_hld_w(int state)
{
	if (m_floppy)
		m_floppy->mon_w(state);
}
