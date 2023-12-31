// license:LGPL-2.1+
// copyright-holders:Michael Zapf
/****************************************************************************

    v9938 Color bus

    The color bus runs from the v9938 video processor to some external device
    like a mouse or a lightpen.

    The Geneve mouse offers three buttons, of which the leftmost one is not
    connected to the v9938; the color bus implementation on the Geneve board
    offers a separate line going to the 9901.

    Michael Zapf

    March 2017

*****************************************************************************/

#include "emu.h"
#include "colorbus.h"
#include "busmouse.h"

DEFINE_DEVICE_TYPE(V9938_COLORBUS, bus::ti99::colorbus::v9938_colorbus_device, "v9938_colorbus", "V9938 Color bus")

namespace bus::ti99::colorbus {

v9938_colorbus_device::v9938_colorbus_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	device_t(mconfig, V9938_COLORBUS, tag, owner, clock),
	device_single_card_slot_interface<device_v9938_colorbus_interface>(mconfig, *this),
	m_v9938(*owner, TIGEN_V9938_TAG),
	m_extra_button(*this)
{
}

void v9938_colorbus_device::movex(int delta)
{
	m_v9938->colorbus_x_input(delta);
}

void v9938_colorbus_device::movey(int delta)
{
	m_v9938->colorbus_y_input(delta);
}

void v9938_colorbus_device::buttons(int bstate)
{
	m_v9938->colorbus_button_input(bstate & 1, bstate & 2);
	m_extra_button((bstate & 4)? ASSERT_LINE : CLEAR_LINE);
}

void v9938_colorbus_device::device_start()
{
}

/*****************************************************************************/

device_v9938_colorbus_interface::device_v9938_colorbus_interface(const machine_config &mconfig, device_t &device) :
	device_interface(device, "v9938colorbus"),
	m_colorbus(nullptr)
{
}

void device_v9938_colorbus_interface::interface_config_complete()
{
	m_colorbus = dynamic_cast<v9938_colorbus_device*>(device().owner());
}

} // end namespace bus::ti99::colorbus

void ti99_colorbus_options(device_slot_interface &device)
{
	device.option_add("busmouse", V9938_BUSMOUSE);
}
