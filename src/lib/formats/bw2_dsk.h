// license:BSD-3-Clause
// copyright-holders:Olivier Galibert
/*********************************************************************

    formats/bw2_dsk.h

    Bondwell 2 format

*********************************************************************/
#ifndef MAME_FORMATS_BW2_DSK_H
#define MAME_FORMATS_BW2_DSK_H

#pragma once

#include "upd765_dsk.h"

class bw2_format : public upd765_format
{
public:
	bw2_format();

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;

private:
	static const format formats[];
};

extern const bw2_format FLOPPY_BW2_FORMAT;

#endif // MAME_FORMATS_BW2_DSK_H
