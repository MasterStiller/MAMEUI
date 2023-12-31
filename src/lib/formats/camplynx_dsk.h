// license:BSD-3-Clause
// copyright-holders:Robbbert
/*********************************************************************

    formats/camplynx_dsk.h

    Camputers Lynx disk image format

*********************************************************************/
#ifndef MAME_FORMATS_CAMPLYNX_DSK_H
#define MAME_FORMATS_CAMPLYNX_DSK_H

#pragma once

#include "wd177x_dsk.h"

class camplynx_format : public wd177x_format
{
public:
	camplynx_format();

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;

private:
	static const format formats[];
};

extern const camplynx_format FLOPPY_CAMPLYNX_FORMAT;

#endif // MAME_FORMATS_CAMPLYNX_DSK_H
