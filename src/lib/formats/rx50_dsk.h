// license:GPL-2.0+
// copyright-holders:Karl-Ludwig Deisenhofer
/*********************************************************************

    formats/rx50_dsk.h

    Format for DEC RX50 floppy drive used e.g. by Rainbow 100 and 190

    Disk is PC MFM, 80 tracks, single sided, with 10 sectors per track

based on lib/formats/esq16_dsk.h

*********************************************************************/
#ifndef MAME_FORMATS_RX50_DSK_H
#define MAME_FORMATS_RX50_DSK_H

#pragma once

#include "flopimg.h"

class rx50img_format : public floppy_image_format_t
{
public:
	rx50img_format();

	virtual int identify(util::random_read &io, uint32_t form_factor, const std::vector<uint32_t> &variants) const override;
	virtual bool load(util::random_read &io, uint32_t form_factor, const std::vector<uint32_t> &variants, floppy_image &image) const override;
	virtual bool save(util::random_read_write &io, const std::vector<uint32_t> &variants, const floppy_image &image) const override;

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;
	virtual bool supports_save() const noexcept override;

	static const desc_e rx50_10_desc[];

private:
	static void find_size(util::random_read &io, uint8_t &track_count, uint8_t &head_count, uint8_t &sector_count);
};

extern const rx50img_format FLOPPY_RX50IMG_FORMAT;

#endif // MAME_FORMATS_RX50_DSK_H
