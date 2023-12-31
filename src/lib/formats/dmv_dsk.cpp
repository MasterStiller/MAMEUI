// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
/*********************************************************************

    formats/dmv_dsk.cpp

    NCR Decision Mate V format

*********************************************************************/

#include "formats/dmv_dsk.h"

dmv_format::dmv_format() : upd765_format(formats)
{
}

const char *dmv_format::name() const noexcept
{
	return "dmv";
}

const char *dmv_format::description() const noexcept
{
	return "Decision Mate V disk image";
}

const char *dmv_format::extensions() const noexcept
{
	return "img";
}

// gap size from hardware reference manual
const dmv_format::format dmv_format::formats[] = {
	{
		floppy_image::FF_525, floppy_image::DSDD, floppy_image::MFM,
		2000,
		9, 40, 2,
		512, {},
		1, {},
		80, 50, 22, 80
	},
	{
		floppy_image::FF_525, floppy_image::DSDD, floppy_image::MFM,
		2000,
		8, 40, 2,
		512, {},
		1, {},
		80, 50, 22, 80
	},
	{}
};

const dmv_format FLOPPY_DMV_FORMAT;
