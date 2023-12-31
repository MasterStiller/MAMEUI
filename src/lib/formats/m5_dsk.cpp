// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
/*********************************************************************

    formats/m5_dsk.cpp

    sord m5 format

*********************************************************************/

#include "formats/m5_dsk.h"

m5_format::m5_format() : upd765_format(formats)
{
}

const char *m5_format::name() const noexcept
{
	return "m5";
}

const char *m5_format::description() const noexcept
{
	return "Sord M5 disk image";
}

const char *m5_format::extensions() const noexcept
{
	return "dsk";
}

// Unverified gap sizes
const m5_format::format m5_format::formats[] = {
	{
		floppy_image::FF_525, floppy_image::DSDD, floppy_image::MFM,
		2000, // 2us, 300rpm
		18, 40, 2,
		256, {},
		1, {},
		80, 50, 22, 80
	},
	{}
};

const m5_format FLOPPY_M5_FORMAT;
