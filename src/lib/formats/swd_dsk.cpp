// license:BSD-3-Clause
// copyright-holders:MetalliC
/*********************************************************************

    formats/swd_dsk.cpp

    Swift Disc disk images

*********************************************************************/

#include "formats/swd_dsk.h"

swd_format::swd_format() : wd177x_format(formats)
{
}

const char *swd_format::name() const noexcept
{
	return "swd";
}

const char *swd_format::description() const noexcept
{
	return "SWD floppy disk image";
}

const char *swd_format::extensions() const noexcept
{
	return "swd";
}

int swd_format::get_image_offset(const format &f, int head, int track) const
{
	return (f.track_count * head + track) * compute_track_size(f);
}

const swd_format::format swd_format::formats[] = {
	// note: original formatted disks use side# 1/2 instead of usual 0/1
	{   // 3'5 640K 80 track double sided double density
		floppy_image::FF_35,  floppy_image::DSDD, floppy_image::MFM,
		2000, 16, 80, 2, 256, {}, 1, {}, 60, 22, 56
	},
	{   // 5"25 640K 80 track double sided double density
		floppy_image::FF_525,  floppy_image::DSQD, floppy_image::MFM,
		2000, 16, 80, 2, 256, {}, 1, {}, 60, 22, 56
	},
	{}
};

const swd_format FLOPPY_SWD_FORMAT;
