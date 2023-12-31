// license:BSD-3-Clause
// copyright-holders:Dirk Best
/***************************************************************************

    Spectravideo SVI-318/328

    Disk image format

***************************************************************************/
#ifndef MAME_FORMATS_SVI_DSK_H
#define MAME_FORMATS_SVI_DSK_H

#pragma once

#include "flopimg.h"

class svi_format : public floppy_image_format_t
{
public:
	svi_format();

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;

	virtual int identify(util::random_read &io, uint32_t form_factor, const std::vector<uint32_t> &variants) const override;
	virtual bool load(util::random_read &io, uint32_t form_factor, const std::vector<uint32_t> &variants, floppy_image &image) const override;
	virtual bool save(util::random_read_write &io, const std::vector<uint32_t> &variants, const floppy_image &image) const override;
	virtual bool supports_save() const noexcept override;
};

extern const svi_format FLOPPY_SVI_FORMAT;

#endif // MAME_FORMATS_SVI_DSK_H
