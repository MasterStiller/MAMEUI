// license:BSD-3-Clause
// copyright-holders:Barry Rodewald
/*
 * fmtowns_dsk.cpp
 *
 *  FM Towns floppy image format
 *
 *  Created on: 23/03/2014
 */

#include "formats/fmtowns_dsk.h"

fmtowns_format::fmtowns_format() : wd177x_format(formats)
{
}

const char *fmtowns_format::name() const noexcept
{
	return "fmtowns";
}

const char *fmtowns_format::description() const noexcept
{
	return "FM Towns disk image";
}

const char *fmtowns_format::extensions() const noexcept
{
	return "bin";
}

// Unverified gap sizes
const fmtowns_format::format fmtowns_format::formats[] = {
	{   /*  1.2MB 3 1/2 inch high density */
		floppy_image::FF_35,  floppy_image::DSHD, floppy_image::MFM,
		1200, 8, 77, 2, 1024, {}, 1, {}, 50, 22, 84
	},
	{}
};

const fmtowns_format FLOPPY_FMTOWNS_FORMAT;
