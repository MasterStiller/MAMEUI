// license:BSD-3-Clause
// copyright-holders:Miodrag Milanovic
/*********************************************************************

    formats/trd_dsk.h

    TRD disk images

*********************************************************************/
#ifndef MAME_FORMATS_TRD_DSK_H
#define MAME_FORMATS_TRD_DSK_H

#pragma once

#include "wd177x_dsk.h"

class trd_format : public wd177x_format {
public:
	trd_format();

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;

private:
	static const format formats[];
	virtual int find_size(util::random_read &io, uint32_t form_factor, const std::vector<uint32_t> &variants) const override;
};

extern const trd_format FLOPPY_TRD_FORMAT;

#endif // MAME_FORMATS_TRD_DSK_H
