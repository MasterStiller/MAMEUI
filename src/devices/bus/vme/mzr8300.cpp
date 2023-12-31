// license:BSD-3-Clause
// copyright-holders:Joakim Larsson Edstrom
/***************************************************************************
 *
 *  Mizar VME8300 rev G 3U VME slave slot device
 *
 *  23/09/2015
 *
 * This device was drycoded based on OS9 boot strap code on a Mizar mz8105 board
 * which expects to find a SIO on the VME bus + photos of a Mizar 8300 board on Ebay
 * I have found no formal documents for this board so far, so needs verification.
 *
 *         ||
 *         ||
 *         ||
 *         ||
 *         ||____________________________________________________________   ___
 * \+++====|| U2|AM26LS32|   |  NEC               | |74LS04N||74LS645   ||_|   |
 *  \=/-  o||   +--------+   |  D7201C            | ++-----+++----------+| |   |
 *   |  |  ||      +-------+ +--------------------+  |     | |SN74LS374N|| |   |
 *   |  |  ||    U1| xxx   |  ____________________   |     | +----------+| |   |
 *   |  |  ||      +-------+ |  NEC               |  |AMD  | |SN74LS374N|| |   |
 *   |  |  ||                |  7201C             |  |AM9513++----------+| |   |
 *   |  |  ||       K10      +--------------------+  | APC ||PAL14L8    || |VME|
 *   |  |==||            +-------+  K5       _______ |     |+-----------+| |   |
 *   |  |==||            |MC1488P|   K4     |SN74S38|| STC ||PAL20L8    || |P1 |
 *   |  |  ||           ++-------+--------+-+------++|     |+-----------+| |   |
 *   |  |  ||       K2  |AM26LS32|AM26LS32| 74S74  | |_____| |SN74LS244N|| |   |
 *   |  |  ||           +--------+--------+--------+_______  +----------+| |   |
 *   |  |  ||                             | 74S74  | 74F85 |             | |   |
 *   |  |  ||           +-------+--------++-------++-------+ +----------+| |   |
 *  /=\-  o||J1    K1 U4| xxx   |  xxx   | 74LS164|   K6     |AM25LS2521|| |   |
 * /+++====||  J2       +-------+--------+--------+--------+ +----------+|_|   |
 *         ||Rev G    U3| MC1488| MC1488 | 74LS161|  74F85 |      K8     | |___|
 *         ||-----------+-------+-----------------------------------------
 *         ||
 *         ||
 *
 *
 * Misc links about this board:
 * http://www.ebay.com/itm/MIZAR-INC-8300-0-01-REV-J-INTERFACE-CONTROL-BOARD-W-RIBBON-AND-PLATE-/231508658429?hash=item35e6fdc8fd
 *
 * Description
 * ------------
 * The Mizar mz8300 is a Quad Serial board.
 *
 *      -  Single High (3U) VME Slave board
 *      -  Two upd7201 SIO Serial Input/Ouput
 *      -  One AM9513 STC System Timing Controller
 *
 * Address Map (just guesses based on driver software behaviours)
 * --------------------------------------------------------------------------
 * Local          VME                  Decscription
 * -------------------------------------------------------------------------
 *  n/a           0xff0000 0xff0003   mzr8105.c Bootstrap expects to find a
 *                                    UPD7201 serial device here - configurable!
 * --------------------------------------------------------------------------
 *
 * Interrupt sources MVME
 * ----------------------------------------------------------
 * Description                  Device  Lvl  IRQ    VME board
 *                           /Board      Vector  Address
 * ----------------------------------------------------------
 * On board Sources
 *
 * Off board Sources (other VME boards)
 *
 * ----------------------------------------------------------
 *
 * DMAC Channel Assignments
 * ----------------------------------------------------------
 * Channel
 * ----------------------------------------------------------
 *
 *  TODO:
 *  - Setup a working address map (STARTED)
 *  - Get documentation for the board
 *  - Add VME bus interface
 *  - Hook up a CPU board that supports this board (mzr8105.c)
 *  - Get terminal working through this device over the VME interface
 *
 ****************************************************************************/

#include "emu.h"
#include "mzr8300.h"

#include "bus/rs232/rs232.h"

#define VERBOSE (0) // (LOG_GENERAL)
//#define LOG_OUTPUT_FUNC osd_printf_info
#include "logmacro.h"


#ifdef _MSC_VER
#define FUNCNAME __func__
#else
#define FUNCNAME __PRETTY_FUNCTION__
#endif


//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************

DEFINE_DEVICE_TYPE(VME_MZR8300, vme_mzr8300_card_device, "mzr8300", "Mizar 8300")

/* These values are borrowed just to get the terminal going and should be replaced
 * once a proper serial board hardware (ie MZ 8300) is found and emulated. */


//-------------------------------------------------
//  device_add_mconfig - add device configuration
//-------------------------------------------------

void vme_mzr8300_card_device::device_add_mconfig(machine_config &config)
{
	UPD7201(config, m_sio[0], XTAL(4'000'000));
	m_sio[0]->out_txdb_callback().set("rs232p1", FUNC(rs232_port_device::write_txd));
	m_sio[0]->out_dtrb_callback().set("rs232p1", FUNC(rs232_port_device::write_dtr));
	m_sio[0]->out_rtsb_callback().set("rs232p1", FUNC(rs232_port_device::write_rts));

	UPD7201(config, m_sio[1], XTAL(4'000'000));

	rs232_port_device &rs232p1(RS232_PORT(config, "rs232p1", default_rs232_devices, "terminal"));
	rs232p1.rxd_handler().set(m_sio[0], FUNC(upd7201_device::rxb_w));
	rs232p1.cts_handler().set(m_sio[0], FUNC(upd7201_device::ctsb_w));

	AM9513(config, m_stc, 4_MHz_XTAL);
	m_stc->out1_cb().set(m_sio[0], FUNC(upd7201_device::rxca_w));
	m_stc->out1_cb().append(m_sio[0], FUNC(upd7201_device::txca_w));
	m_stc->out2_cb().set(m_sio[0], FUNC(upd7201_device::rxcb_w));
	m_stc->out2_cb().append(m_sio[0], FUNC(upd7201_device::txcb_w));
	m_stc->out3_cb().set(m_sio[1], FUNC(upd7201_device::rxca_w));
	m_stc->out3_cb().append(m_sio[1], FUNC(upd7201_device::txca_w));
	m_stc->out4_cb().set(m_sio[1], FUNC(upd7201_device::rxcb_w));
	m_stc->out4_cb().append(m_sio[1], FUNC(upd7201_device::txcb_w));
}


//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

vme_mzr8300_card_device::vme_mzr8300_card_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock)
	: device_t(mconfig, type, tag, owner, clock)
	, device_vme_card_interface(mconfig, *this)
	, m_sio(*this, "sio%u", 0U)
	, m_stc(*this, "stc")
{
	LOG("%s %s\n", tag, FUNCNAME);
}

vme_mzr8300_card_device::vme_mzr8300_card_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	vme_mzr8300_card_device(mconfig, VME_MZR8300, tag, owner, clock)
{
}

void vme_mzr8300_card_device::device_start()
{
	LOG("%s %s\n", tag(), FUNCNAME);

	/* Setup r/w handlers for first SIO in A16 */
	vme_space(vme::AM_29).install_readwrite_handler(0, 1, // Channel B - Data
			read8smo_delegate(m_sio[0], FUNC(z80sio_device::db_r)),
			write8smo_delegate(m_sio[0], FUNC(z80sio_device::db_w)), 0x00ff);
	vme_space(vme::AM_29).install_readwrite_handler(2, 3, // Channel B - Control
			read8smo_delegate(m_sio[0], FUNC(z80sio_device::cb_r)),
			write8smo_delegate(m_sio[0], FUNC(z80sio_device::cb_w)), 0x00ff);
	vme_space(vme::AM_29).install_readwrite_handler(4, 5, // Channel A - Data
			read8smo_delegate(m_sio[0], FUNC(z80sio_device::da_r)),
			write8smo_delegate(m_sio[0], FUNC(z80sio_device::da_w)), 0x00ff);
	vme_space(vme::AM_29).install_readwrite_handler(6, 7, // Channel A - Control
			read8smo_delegate(m_sio[0], FUNC(z80sio_device::ca_r)),
			write8smo_delegate(m_sio[0], FUNC(z80sio_device::ca_w)), 0x00ff);
	vme_space(vme::AM_29).install_readwrite_handler(0x10, 0x13, // Am9513
			read8sm_delegate(m_stc, FUNC(am9513_device::read8)),
			write8sm_delegate(m_stc, FUNC(am9513_device::write8)), 0x00ff);
}

void vme_mzr8300_card_device::device_reset()
{
	LOG("%s %s\n", tag(), FUNCNAME);
}

#if 0
uint8_t vme_mzr8300_card_device::read8(offs_t offset){
	LOG("%s()\n", FUNCNAME);
	return (uint8_t) 0;
}

void vme_mzr8300_card_device::write8(offs_t offset, uint8_t data){
	LOG("%s()\n", FUNCNAME);
}
#endif
