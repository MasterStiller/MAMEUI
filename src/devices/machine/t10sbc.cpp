// license:BSD-3-Clause
// copyright-holders:smf
#include "emu.h"
#include "t10sbc.h"

#include "multibyte.h"

void t10sbc::t10_start(device_t &device)
{
	m_device = &device;
	t10spc::t10_start(device);

	device.save_item( NAME( m_lba ) );
	device.save_item( NAME( m_blocks ) );
}

void t10sbc::t10_reset()
{
	t10spc::t10_reset();

	m_lba = 0;
	m_blocks = 0;
	m_sector_bytes = 512;

	if (!m_image->exists())
	{
		m_device->logerror("T10SBC %s: no HD found!\n", m_image->owner()->tag());
	}
	else
	{
		// get hard disk sector size from CHD metadata
		m_sector_bytes = m_image->get_info().sectorbytes;
	}
}

// scsihd_exec_command
void t10sbc::ExecCommand()
{
	switch ( command[0] )
	{
	case T10SBC_CMD_FORMAT_UNIT:
		m_phase = SCSI_PHASE_STATUS;
		m_status_code = SCSI_STATUS_CODE_GOOD;
		m_transfer_length = 0;
		break;

	case T10SBC_CMD_SEEK_6:
		m_lba = get_u24be(&command[1])&0x1fffff;

		m_device->logerror("S1410: SEEK to LBA %x\n", m_lba);

		m_phase = SCSI_PHASE_STATUS;
		m_transfer_length = 0;
		break;

	case T10SBC_CMD_READ_6:
		m_lba = get_u24be(&command[1])&0x1fffff;
		m_blocks = SCSILengthFromUINT8( &command[4] );

		m_device->logerror("T10SBC: READ at LBA %x for %x blocks\n", m_lba, m_blocks);

		m_phase = SCSI_PHASE_DATAIN;
		m_status_code = SCSI_STATUS_CODE_GOOD;
		m_transfer_length = m_blocks * m_sector_bytes;
		break;

	case T10SBC_CMD_WRITE_6:
		m_lba = get_u24be(&command[1])&0x1fffff;
		m_blocks = SCSILengthFromUINT8( &command[4] );

		m_device->logerror("T10SBC: WRITE to LBA %x for %x blocks\n", m_lba, m_blocks);

		m_phase = SCSI_PHASE_DATAOUT;
		m_status_code = SCSI_STATUS_CODE_GOOD;
		m_transfer_length = m_blocks * m_sector_bytes;
		break;

	case T10SPC_CMD_INQUIRY:
		m_phase = SCSI_PHASE_DATAIN;
		m_status_code = SCSI_STATUS_CODE_GOOD;
		m_transfer_length = SCSILengthFromUINT8( &command[ 4 ] );
		break;

	case T10SPC_CMD_MODE_SELECT_6:
		m_device->logerror("T10SBC: MODE SELECT length %x control %x\n", command[4], command[5]);
		m_phase = SCSI_PHASE_DATAOUT;
		m_status_code = SCSI_STATUS_CODE_GOOD;
		m_transfer_length = SCSILengthFromUINT8( &command[ 4 ] );
		break;

	case T10SPC_CMD_MODE_SENSE_6:
		m_phase = SCSI_PHASE_DATAIN;
		m_status_code = SCSI_STATUS_CODE_GOOD;
		m_transfer_length = SCSILengthFromUINT8( &command[ 4 ] );
		break;

	case T10SBC_CMD_READ_CAPACITY:
		m_phase = SCSI_PHASE_DATAIN;
		m_status_code = SCSI_STATUS_CODE_GOOD;
		m_transfer_length = 8;
		break;

	case T10SBC_CMD_READ_10:
		m_lba = get_u32be(&command[2]);
		m_blocks = SCSILengthFromUINT16( &command[7] );

		m_device->logerror("T10SBC: READ at LBA %x for %x blocks\n", m_lba, m_blocks);

		m_phase = SCSI_PHASE_DATAIN;
		m_status_code = SCSI_STATUS_CODE_GOOD;
		m_transfer_length = m_blocks * m_sector_bytes;
		break;

	case T10SBC_CMD_WRITE_10:
		m_lba = get_u32be(&command[2]);
		m_blocks = SCSILengthFromUINT16( &command[7] );

		m_device->logerror("T10SBC: WRITE to LBA %x for %x blocks\n", m_lba, m_blocks);

		m_phase = SCSI_PHASE_DATAOUT;
		m_status_code = SCSI_STATUS_CODE_GOOD;
		m_transfer_length = m_blocks * m_sector_bytes;
		break;

	case T10SBC_CMD_READ_12:
		m_lba = get_u32be(&command[2]);
		m_blocks = get_u32be(&command[6]);

		m_device->logerror("T10SBC: READ at LBA %x for %x blocks\n", m_lba, m_blocks);

		m_phase = SCSI_PHASE_DATAIN;
		m_status_code = SCSI_STATUS_CODE_GOOD;
		m_transfer_length = m_blocks * m_sector_bytes;
		break;

	default:
		t10spc::ExecCommand();
		break;
	}
}

void t10sbc::ReadData( uint8_t *data, int dataLength )
{
	// if we're a drive without a disk, return all zeroes
	if (!m_image->exists())
	{
		memset(data, 0, dataLength);
		return;
	}

	switch ( command[0] )
	{
	case T10SPC_CMD_INQUIRY:
		memset( data, 0, dataLength );
		data[0] = 0x00; // device is direct-access (e.g. hard disk)
		data[1] = 0x00; // media is not removable
		data[2] = 0x05; // device complies with SPC-3 standard
		data[3] = 0x02; // response data format = SPC-3 standard
		// Apple HD SC setup utility needs to see this
		strcpy((char *)&data[8], " SEAGATE");
		strcpy((char *)&data[16], "          ST225N");
		strcpy((char *)&data[32], "1.0");
		break;

	case T10SPC_CMD_MODE_SENSE_6:
	{
		const uint8_t page = command[2] & 0x3f;
		const uint8_t subpage = command[3];

		switch (page)
		{
			case 0x03:
			{
				// Format Parameters
				if (subpage != 0)
				{
					m_device->logerror("T10SBC: Unsupported MODE SENSE subpage for Format Parameters page: %02x\n", subpage);
					break;
				}

				const size_t fullSize = sizeof(format_page_t) + 12;
				if (dataLength < fullSize)
				{
					m_device->logerror("T10SBC: Insufficient MODE SENSE buffer room for Format Parameters page: Need %d, given %d\n", fullSize, dataLength);
					break;
				}

				m_device->logerror("T10SBC: MODE SENSE (6), Format Parameters page\n");
				memset(data, 0, (uint8_t)fullSize);
				data[0] = fullSize;
				data[3] = 8;

				ReadCapacity(&data[4]);

				format_page_t format;
				GetFormatPage(&format);
				memcpy(&data[12], &format, sizeof(format_page_t));
				break;
			}
			case 0x04:
			{
				// Rigid Drive Geometry Parameters
				if (subpage != 0)
				{
					m_device->logerror("T10SBC: Unsupported MODE SENSE subpage for Geometry Parameters page: %02x\n", subpage);
					break;
				}

				const size_t fullSize = sizeof(geometry_page_t) + 12;
				if (dataLength < fullSize)
				{
					m_device->logerror("T10SBC: Insufficient MODE SENSE buffer room for Geometry Parameters page: Need %d, given %d\n", fullSize, dataLength);
					break;
				}

				m_device->logerror("T10SBC: MODE SENSE (6), Geometry Parameters page\n");
				memset(data, 0, fullSize);
				data[0] = (uint8_t)fullSize;
				data[3] = 8;

				ReadCapacity(&data[4]);

				geometry_page_t geometry;
				GetGeometryPage(&geometry);
				memcpy(&data[12], &geometry, sizeof(geometry_page_t));
				break;
			}
			case 0x30:
				// special Apple ID page.  this is a vendor-specific page,
				// so unless collisions occur there should be no need
				// to change it.
				memset(data, 0, 40);
				data[0] = 0x14;
				strcpy((char *)&data[14], "APPLE COMPUTER, INC.");
				break;
			case 0x3f:
				if (subpage == 0)
				{
					const size_t fullSize = sizeof(format_page_t) + sizeof(geometry_page_t) + 12;
					if (dataLength < fullSize)
					{
						m_device->logerror("T10SBC: Insufficient MODE SENSE buffer room for Return All Pages: Need %d, given %d\n", fullSize, dataLength);
						break;
					}

					m_device->logerror("T10SBC: MODE SENSE (6), Return All Pages\n");
					memset(data, 0, (uint8_t)fullSize);
					data[0] = (uint8_t)fullSize;
					data[3] = (uint8_t)8;

					ReadCapacity(&data[4]);

					format_page_t format;
					GetFormatPage(&format);
					memcpy(&data[12], &format, sizeof(format_page_t));

					geometry_page_t geometry;
					GetGeometryPage(&geometry);
					memcpy(&data[12 + sizeof(format_page_t)], &geometry, sizeof(geometry_page_t));
				}
				else
				{
					m_device->logerror("T10SBC: Unsupported MODE SENSE subpage for Return All Pages: %02x\n", subpage);
				}
				break;
			default:
				m_device->logerror("T10SBC: Unknown MODE SENSE page: %02x\n", page);
				break;
		}
		break;
	}

	case T10SBC_CMD_READ_6:
	case T10SBC_CMD_READ_10:
	case T10SBC_CMD_READ_12:
		if (m_image->exists() && (m_blocks))
		{
			m_device->logerror("T10SBC: Reading %d bytes from HD\n", dataLength);
			while (dataLength > 0)
			{
				if (!m_image->read(m_lba,  data))
				{
					m_device->logerror("T10SBC: HD read error!\n");
				}
				m_lba++;
				m_blocks--;
				dataLength -= m_sector_bytes;
				data += m_sector_bytes;
			}
		}
		break;

	case T10SBC_CMD_READ_CAPACITY: // READ CAPACITY
		m_device->logerror("T10SBC: READ CAPACITY\n");
		ReadCapacity(&data[0]);
		break;

	default:
		t10spc::ReadData( data, dataLength );
		break;
	}
}

void t10sbc::WriteData( uint8_t *data, int dataLength )
{
	if (!m_image->exists())
	{
		return;
	}

	switch ( command[0] )
	{
	case T10SPC_CMD_MODE_SELECT_6:
		break;

	case T10SBC_CMD_WRITE_6:
	case T10SBC_CMD_WRITE_10:
		if (m_image->exists() && (m_blocks))
		{
			m_device->logerror("T10SBC: Writing %d bytes to HD\n", dataLength);
			while (dataLength > 0)
			{
				if (!m_image->write(m_lba, data))
				{
					m_device->logerror("T10SBC: HD write error!\n");
				}
				m_lba++;
				m_blocks--;
				dataLength -= m_sector_bytes;
				data += m_sector_bytes;
			}
		}
		break;

	default:
		t10spc::WriteData( data, dataLength );
		break;
	}
}

void t10sbc::GetFormatPage( format_page_t *page )
{
	const auto &info = m_image->get_info();

	memset(page, 0, sizeof(format_page_t));
	page->m_page_code = 0x03;
	page->m_page_length = 0x16;
	page->m_sectors_per_track_msb = (uint8_t)(info.sectors >> 8);
	page->m_sectors_per_track_lsb = (uint8_t)info.sectors;
	page->m_bytes_per_sector_msb = (uint8_t)(info.sectorbytes >> 8);
	page->m_bytes_per_sector_lsb = (uint8_t)info.sectorbytes;
	page->m_format = 0x80; // SSEC, Soft-Sectored
}

void t10sbc::GetGeometryPage( geometry_page_t *page )
{
	const auto &info = m_image->get_info();

	memset(page, 0, sizeof(geometry_page_t));
	page->m_page_code = 0x04;
	page->m_page_length = 0x16;
	page->m_num_cylinders_msb = (uint8_t)(info.cylinders >> 16);
	page->m_num_cylinders_2nd = (uint8_t)(info.cylinders >> 8);
	page->m_num_cylinders_lsb = (uint8_t)info.cylinders;
	page->m_num_heads = (uint8_t)info.heads;
	page->m_rot_rate_msb = (uint8_t)(3600 >> 8);
	page->m_rot_rate_lsb = (uint8_t)3600;
}

void t10sbc::ReadCapacity( uint8_t *data )
{
	const auto &info = m_image->get_info();

	// get # of sectors
	uint32_t temp = info.cylinders * info.heads * info.sectors - 1;

	put_u32be(&data[0], temp);
	put_u32be(&data[4], info.sectorbytes);
}
