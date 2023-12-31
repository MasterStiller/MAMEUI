// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
/*********************************************************************

    formats/cpis_dsk.h

    Telenova Compis disk images

*********************************************************************/
#ifndef MAME_FORMATS_CPIS_DSK_H
#define MAME_FORMATS_CPIS_DSK_H

#pragma once

#include "upd765_dsk.h"

class cpis_format : public upd765_format {
public:
	cpis_format();

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;

private:
	static const format formats[];
};

extern const cpis_format FLOPPY_CPIS_FORMAT;

#endif // MAME_FORMATS_CPIS_DSK_H
