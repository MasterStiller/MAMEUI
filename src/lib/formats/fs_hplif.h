// license:BSD-3-Clause
// copyright-holders:Nathan Woods
/***************************************************************************

    fs_hplif.h

    Management of CBM (Commodore) DOS disk images

***************************************************************************/

#ifndef MAME_FORMATS_FS_HPLIF_H
#define MAME_FORMATS_FS_HPLIF_H

#pragma once

#include "fsmgr.h"

namespace fs {

	class hplif_image : public manager_t {
	public:
		hplif_image() = default;

		virtual const char *name() const override;
		virtual const char *description() const override;

		virtual void enumerate_f(floppy_enumerator &fe) const override;
		virtual std::unique_ptr<filesystem_t> mount(fsblk_t &blockdev) const override;

		virtual bool can_format() const override;
		virtual bool can_read() const override;
		virtual bool can_write() const override;
		virtual bool has_rsrc() const override;

		virtual std::vector<meta_description> volume_meta_description() const override;
		virtual std::vector<meta_description> file_meta_description() const override;
	};

	extern const hplif_image HPLIF;

} // namespace fs

#endif // #define MAME_FORMATS_FS_HPLIF_H
