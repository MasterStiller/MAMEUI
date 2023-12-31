// license:BSD-3-Clause
// copyright-holders:Curt Coder
/*********************************************************************

    formats/sf7000_dsk.h

    sf7000 format

*********************************************************************/
#ifndef MAME_FORMATS_SF7000_DSK_H
#define MAME_FORMATS_SF7000_DSK_H

#pragma once

#include "upd765_dsk.h"

class sf7000_format : public upd765_format
{
public:
	sf7000_format();

	virtual const char *name() const noexcept override;
	virtual const char *description() const noexcept override;
	virtual const char *extensions() const noexcept override;

private:
	static const format formats[];
};

extern const sf7000_format FLOPPY_SF7000_FORMAT;

#endif // MAME_FORMATS_SF7000_DSK_H
