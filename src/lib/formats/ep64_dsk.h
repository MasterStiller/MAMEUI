// license:BSD-3-Clause
// copyright-holders:Curt Coder
/*********************************************************************

    formats/ep64_dsk.h

    Enterprise Sixty Four disk image format

*********************************************************************/
#ifndef MAME_FORMATS_EP64_DSK_H
#define MAME_FORMATS_EP64_DSK_H

#pragma once

#include "wd177x_dsk.h"

class ep64_format : public wd177x_format
{
public:
	ep64_format();

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;

private:
	static const format formats[];
};

extern const ep64_format FLOPPY_EP64_FORMAT;

#endif // MAME_FORMATS_EP64_DSK_H
