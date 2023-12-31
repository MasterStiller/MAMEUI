// license:BSD-3-Clause
// copyright-holders:Robbbert
/*********************************************************************

    formats/excali64_dsk.cpp

    Excalibur 64 disk image format

*********************************************************************/

#include "formats/excali64_dsk.h"

excali64_format::excali64_format() : wd177x_format(formats)
{
}

const char *excali64_format::name() const noexcept
{
	return "excali64";
}

const char *excali64_format::description() const noexcept
{
	return "Excalibur 64 disk image";
}

const char *excali64_format::extensions() const noexcept
{
	return "raw";
}

// Unverified gap sizes
const excali64_format::format excali64_format::formats[] = {
	{   /*  800K 19cm double density */
		floppy_image::FF_525,  floppy_image::DSDD, floppy_image::MFM,
		2000,  5, 80, 2, 1024, {}, 1, {}, 100, 22, 84
	},
	{}
};

const excali64_format FLOPPY_EXCALI64_FORMAT;
