// license:BSD-3-Clause
// copyright-holders:Nathan Woods
/***************************************************************************

    fs_coco_rsdos.h

    Management of CoCo "RS-DOS" floppy images

***************************************************************************/

#ifndef MAME_FORMATS_FS_COCO_RSDOS_H
#define MAME_FORMATS_FS_COCO_RSDOS_H

#pragma once

#include "fsmgr.h"

namespace fs {

class coco_rsdos_image : public manager_t {
public:
	coco_rsdos_image() = default;

	virtual const char *name() const override;
	virtual const char *description() const override;

	virtual void enumerate_f(floppy_enumerator &fe) const override;
	virtual std::unique_ptr<filesystem_t> mount(fsblk_t &blockdev) const override;

	virtual bool can_format() const override;
	virtual bool can_read() const override;
	virtual bool can_write() const override;
	virtual bool has_rsrc() const override;

	virtual std::vector<meta_description> file_meta_description() const override;
};


extern const coco_rsdos_image COCO_RSDOS;

} // namespace fs

#endif // MAME_FORMATS_FS_COCO_RSDOS_H
