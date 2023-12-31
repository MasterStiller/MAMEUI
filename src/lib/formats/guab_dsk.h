// license:BSD-3-Clause
// copyright-holders:Dirk Best
/***************************************************************************

    JPM Give us a Break

    Disk image format

***************************************************************************/
#ifndef MAME_FORMATS_GUAB_DSK_H
#define MAME_FORMATS_GUAB_DSK_H

#pragma once

#include "wd177x_dsk.h"

class guab_format : public wd177x_format
{
public:
	guab_format();

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;

private:
	static const format formats[];
};

extern const guab_format FLOPPY_GUAB_FORMAT;

#endif // MAME_FORMATS_GUAB_DSK_H
