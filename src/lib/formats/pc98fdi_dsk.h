// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
/*********************************************************************

    formats/pc98fdi_dsk.h

    PC98FDI disk images

*********************************************************************/
#ifndef MAME_FORMATS_PC98FDI_DSK_H
#define MAME_FORMATS_PC98FDI_DSK_H

#pragma once

#include "flopimg.h"


class pc98fdi_format : public floppy_image_format_t
{
public:
	pc98fdi_format();

	virtual int identify(util::random_read &io, uint32_t form_factor, const std::vector<uint32_t> &variants) const override;
	virtual bool load(util::random_read &io, uint32_t form_factor, const std::vector<uint32_t> &variants, floppy_image &image) const override;

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;
	virtual bool supports_save() const noexcept override;
};

extern const pc98fdi_format FLOPPY_PC98FDI_FORMAT;

#endif // MAME_FORMATS_PC98FDI_DSK_H
