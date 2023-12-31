// license:BSD-3-Clause
// copyright-holders:Curt Coder
/*********************************************************************

    formats/tandy2k_dsk.cpp

    Tandy 2000 disk format

*********************************************************************/

#include "formats/tandy2k_dsk.h"

tandy2k_format::tandy2k_format() : upd765_format(formats)
{
}

const char *tandy2k_format::name() const noexcept
{
	return "tandy2k";
}

const char *tandy2k_format::description() const noexcept
{
	return "Tandy 2000 disk image";
}

const char *tandy2k_format::extensions() const noexcept
{
	return "dsk";
}

const tandy2k_format::format tandy2k_format::formats[] = {
	{   // 720K 5.25 inch quad density - gaps unverified
		floppy_image::FF_525, floppy_image::DSQD, floppy_image::MFM,
		2000, 9, 80, 2, 512, {}, 1, {}, 80, 50, 22, 80
	},
	{}
};

const tandy2k_format FLOPPY_TANDY_2000_FORMAT;
