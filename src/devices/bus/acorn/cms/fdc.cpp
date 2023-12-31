// license:BSD-3-Clause
// copyright-holders:Nigel Barnes
/**********************************************************************

    CMS Floppy Disc Controller Board

    Part No. CMS 0015-1

**********************************************************************/

#include "emu.h"
#include "fdc.h"

#include "formats/acorn_dsk.h"


//**************************************************************************
//  DEVICE DEFINITIONS
//**************************************************************************

DEFINE_DEVICE_TYPE(CMS_FDC, cms_fdc_device, "cms_fdc", "CMS Floppy Disc Controller Board")

//-------------------------------------------------
//  MACHINE_DRIVER( cms_fdc )
//-------------------------------------------------

void cms_fdc_device::floppy_formats(format_registration &fr)
{
	fr.add_mfm_containers();
	fr.add(FLOPPY_ACORN_SSD_FORMAT);
	fr.add(FLOPPY_ACORN_DSD_FORMAT);
}

static void cms_floppies(device_slot_interface &device)
{
	device.option_add("35dd", FLOPPY_35_DD);
}

//-------------------------------------------------
//  device_add_mconfig - add device configuration
//-------------------------------------------------

void cms_fdc_device::device_add_mconfig(machine_config &config)
{
	WD1770(config, m_fdc, 8_MHz_XTAL);
	FLOPPY_CONNECTOR(config, m_floppy[0], cms_floppies, "35dd", floppy_formats).enable_sound(true);
	FLOPPY_CONNECTOR(config, m_floppy[1], cms_floppies, "35dd", floppy_formats).enable_sound(true);
	FLOPPY_CONNECTOR(config, m_floppy[2], cms_floppies, nullptr, floppy_formats).enable_sound(true);
}


//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  cms_fdc_device - constructor
//-------------------------------------------------

cms_fdc_device::cms_fdc_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: device_t(mconfig, CMS_FDC, tag, owner, clock)
	, device_acorn_bus_interface(mconfig, *this)
	, m_fdc(*this, "wd1770")
	, m_floppy(*this, "wd1770:%u", 0)
{
}


//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void cms_fdc_device::device_start()
{
	address_space &space = m_bus->memspace();

	space.install_readwrite_handler(0xfc50, 0xfc5f, read8smo_delegate(*this, FUNC(cms_fdc_device::wd1770_state_r)), write8smo_delegate(*this, FUNC(cms_fdc_device::wd1770_control_w)));
	space.install_readwrite_handler(0xfc40, 0xfc4f, read8sm_delegate(*m_fdc, FUNC(wd1770_device::read)), write8sm_delegate(*m_fdc, FUNC(wd1770_device::write)));
}


//**************************************************************************
//  IMPLEMENTATION
//**************************************************************************

uint8_t cms_fdc_device::wd1770_state_r()
{
	uint8_t data = 0x3f;

	data |= m_fdc->intrq_r() << 6;
	data |= m_fdc->drq_r() << 7;

	return data;
}

void cms_fdc_device::wd1770_control_w(uint8_t data)
{
	floppy_image_device *floppy = nullptr;

	// bit 0, 1, 2: drive select
	if (BIT(data, 0)) floppy = m_floppy[0]->get_device();
	if (BIT(data, 1)) floppy = m_floppy[1]->get_device();
	if (BIT(data, 2)) floppy = m_floppy[2]->get_device();
	m_fdc->set_floppy(floppy);

	// bit 3: side select
	if (floppy)
		floppy->ss_w(BIT(data, 3));

	// bit 7: density ??
	m_fdc->dden_w(BIT(data, 7));
}
