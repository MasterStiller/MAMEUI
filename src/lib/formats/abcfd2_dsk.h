// license:BSD-3-Clause
// copyright-holders:Curt Coder
/*********************************************************************

    formats/abcfd2_dsk.h

    Scandia Metric ABC FD2 disk image formats

*********************************************************************/
#ifndef MAME_FORMATS_ABCFD2_DSK_H
#define MAME_FORMATS_ABCFD2_DSK_H

#pragma once

#include "wd177x_dsk.h"

class abc_fd2_format : public wd177x_format
{
public:
	abc_fd2_format();

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;

private:
	static const format formats[];
};

extern const abc_fd2_format FLOPPY_ABC_FD2_FORMAT;

#endif // MAME_FORMATS_ABCFD2_DSK_H
