// license:BSD-3-Clause
// copyright-holders:Olivier Galibert, Robbbert
/*********************************************************************

    formats/kaypro_dsk.cpp

    Kaypro disk image format

    There is no inter-sector info on these disks. It is simply a
    dump of the 512 bytes from each sector and track in order.
    It is just like a headerless quickload.
    Base class and gaps verified by OG. Please note that these
    disks usually have DSK extension, but that conflicts with
    the CPCEMU DSK format. You need to rename your Kaypro DSK
    disks to KAY extension.

*********************************************************************/

#include "formats/kaypro_dsk.h"

kayproii_format::kayproii_format() : upd765_format(formats)
{
}

const char *kayproii_format::name() const noexcept
{
	return "kaypro2";
}

const char *kayproii_format::description() const noexcept
{
	return "Kaypro disk image";
}

const char *kayproii_format::extensions() const noexcept
{
	return "kay,dsk";
}

const kayproii_format::format kayproii_format::formats[] = {
	{   /*  191K 13cm double density single sided */
		floppy_image::FF_525,  floppy_image::SSDD, floppy_image::MFM,
		2000, 10, 40, 1, 512, {}, 0, {}, 80, 16, 22, 26
	},
	{}
};

kaypro2x_format::kaypro2x_format() : upd765_format(formats)
{
}

const char *kaypro2x_format::name() const noexcept
{
	return "kaypro2x";
}

const char *kaypro2x_format::description() const noexcept
{
	return "Kaypro disk image";
}

const char *kaypro2x_format::extensions() const noexcept
{
	return "kay,dsk";
}

const kaypro2x_format::format kaypro2x_format::formats[] = {
	{   /*  382K 13cm double density double sided */
		floppy_image::FF_525,  floppy_image::DSDD, floppy_image::MFM,
		2000, 10, 40, 2, 512, {}, 0, {}, 80, 16, 22, 26
	},
	{}
};

const kayproii_format FLOPPY_KAYPROII_FORMAT;
const kaypro2x_format FLOPPY_KAYPRO2X_FORMAT;
