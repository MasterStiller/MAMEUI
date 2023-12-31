// license:BSD-3-Clause
// copyright-holders:Dirk Best
/***************************************************************************

    Nascom 1/2/3

    Disk image format

***************************************************************************/
#ifndef MAME_FORMATS_NASCOM_DSK_H
#define MAME_FORMATS_NASCOM_DSK_H

#pragma once

#include "wd177x_dsk.h"

class nascom_format : public wd177x_format
{
public:
	nascom_format();

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;

private:
	static const format formats[];
};

extern const nascom_format FLOPPY_NASCOM_FORMAT;

#endif // MAME_FORMATS_NASCOM_DSK_H
