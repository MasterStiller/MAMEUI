// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
/*********************************************************************

    formats/wd177x_sk.h

    helper for simple wd177x-formatted disk images

*********************************************************************/
#ifndef MAME_FORMATS_WD177X_DSK_H
#define MAME_FORMATS_WD177X_DSK_H

#pragma once

#include "flopimg.h"

class wd177x_format : public floppy_image_format_t
{
public:
	struct format {
		uint32_t form_factor;      // See floppy_image for possible values
		uint32_t variant;          // See floppy_image for possible values
		uint32_t encoding;         // See floppy_image for possible values

		int cell_size;           // See floppy_image_format_t for details
		int sector_count;
		int track_count;
		int head_count;
		int sector_base_size;
		int per_sector_size[40]; // if sector_base_size is 0
		int sector_base_id;      // 0 or 1 usually, -1 if there's interleave
		int per_sector_id[40];   // if sector_base_id is -1.  If both per are used, then sector per_sector_id[i] has size per_sector_size[i]
		int gap_1;               // Usually around 80 - number of 4e between index and first IDAM sync
		int gap_2;               // 22 for <=1.44Mb, 41 for 2.88Mb - number of 4e between sector header and data sync
		int gap_3;               // Usually 24 - number of 4e between sector crc and next IDAM
	};

	// End the array with {}
	wd177x_format(const format *formats);

	virtual int identify(util::random_read &io, uint32_t form_factor, const std::vector<uint32_t> &variants) const override;
	virtual bool load(util::random_read &io, uint32_t form_factor, const std::vector<uint32_t> &variants, floppy_image &image) const override;
	virtual bool save(util::random_read_write &io, const std::vector<uint32_t> &variants, const floppy_image &image) const override;
	virtual bool supports_save() const noexcept override;

protected:
	enum { FM_DAM = 0xf56f, FM_DDAM = 0xf56a, MFM_DAM = 0xfb, MFM_DDAM = 0xf8 };

	const format *formats;

	virtual const wd177x_format::format &get_track_format(const format &f, int head, int track) const;
	virtual floppy_image_format_t::desc_e* get_desc_fm(const format &f, int &current_size, int &end_gap_index) const;
	virtual floppy_image_format_t::desc_e* get_desc_mfm(const format &f, int &current_size, int &end_gap_index) const;
	virtual int find_size(util::random_read &io, uint32_t form_factor, const std::vector<uint32_t> &variants) const;
	virtual int get_image_offset(const format &f, int head, int track) const;
	virtual int get_track_dam_fm(const format &f, int head, int track) const;
	virtual int get_track_dam_mfm(const format &f, int head, int track) const;

	static int compute_track_size(const format &f);
	virtual void build_sector_description(const format &d, uint8_t *sectdata, desc_s *sectors, int track, int head) const;
	virtual void check_compatibility(const floppy_image &image, std::vector<int> &candidates) const;
	static void extract_sectors(const floppy_image &image, const format &f, desc_s *sdesc, int track, int head);
};

#endif // MAME_FORMATS_WD177X_DSK_H
