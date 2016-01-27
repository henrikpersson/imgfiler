/* Copyright (C) Bud Millwood, 2016.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "exif.h"
#include <string.h>
#include <stdio.h>

void get_tag(ExifData* d, ExifIfd ifd, ExifTag tag, char* buf, int len);

bool exif_date (const char* fqpn, date_t* date)
{
  ExifData *ed;
  ExifEntry *entry;
  char data[1024];
  memset (data,0,sizeof(data));

  /* Load an ExifData object from an EXIF file */
  ed = exif_data_new_from_file(fqpn);
  if (!ed)
  {
    return false;  // no exif data in this file
  }

  get_tag(ed, EXIF_IFD_EXIF, EXIF_TAG_DATE_TIME_ORIGINAL,data,sizeof(data));  // EXIF tag 0x9003, DateTimeOriginal
  if (*data != 0 && read_yyyymmdd (data,date))
  {
    exif_data_unref(ed);
    return true;
  }

  memset (data,0,sizeof(data));
  get_tag(ed, EXIF_IFD_EXIF, EXIF_TAG_DATE_TIME_DIGITIZED,data,sizeof(data));  // EXIF tag 0x9003, DateTimeDigitized
  if (*data != 0 && read_yyyymmdd (data,date))
  {
    exif_data_unref(ed);
    return true;
  }

  memset (data,0,sizeof(data));
  get_tag(ed, EXIF_IFD_0, EXIF_TAG_DATE_TIME,data,sizeof(data));  // EXIF tag 0x0132, ModifyDate
  if (*data != 0 && read_yyyymmdd (data,date))
  {
    exif_data_unref(ed);
    return true;
  }

// 0x82aa MDPrepDate

  exif_data_unref(ed);

  return false;
}

// get the tag
void get_tag(ExifData* d, ExifIfd ifd, ExifTag tag, char* buf, int len)
{
  ExifEntry *entry = exif_content_get_entry(d->ifd[ifd],tag);
  if (entry) {
    exif_entry_get_value(entry, buf, len);
    right_trim(buf,' ');
  }
}
