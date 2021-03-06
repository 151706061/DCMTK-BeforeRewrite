/*
 *
 *  Copyright (C) 1994-2012, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module:  dcmdata
 *
 *  Author:  Andrew Hewett
 *
 *  Purpose: Error handling, codes and strings
 *
 *  Last Update:      $Author: uli $
 *  Update Date:      $Date: 2012-02-15 14:50:42 $
 *  CVS/RCS Revision: $Revision: 1.32 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */


#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#include "dcmtk/dcmdata/dcerror.h"

makeOFConditionConst(EC_InvalidTag,                 OFM_dcmdata,  1, OF_error, "Invalid tag"                                );
makeOFConditionConst(EC_TagNotFound,                OFM_dcmdata,  2, OF_error, "Tag not found"                              );
makeOFConditionConst(EC_InvalidVR,                  OFM_dcmdata,  3, OF_error, "Invalid VR"                                 );
makeOFConditionConst(EC_InvalidStream,              OFM_dcmdata,  4, OF_error, "Invalid stream"                             );
makeOFConditionConst(EC_EndOfStream,                OFM_dcmdata,  5, OF_error, "End of stream"                              );
makeOFConditionConst(EC_CorruptedData,              OFM_dcmdata,  6, OF_error, "Corrupted data"                             );
makeOFConditionConst(EC_IllegalCall,                OFM_dcmdata,  7, OF_error, "Illegal call, perhaps wrong parameters"     );
makeOFConditionConst(EC_SequEnd,                    OFM_dcmdata,  8, OF_error, "Sequence end"                               );
makeOFConditionConst(EC_DoubledTag,                 OFM_dcmdata,  9, OF_error, "Doubled tag"                                );
makeOFConditionConst(EC_StreamNotifyClient,         OFM_dcmdata, 10, OF_error, "I/O suspension or premature end of stream"  );
makeOFConditionConst(EC_WrongStreamMode,            OFM_dcmdata, 11, OF_error, "Mode (R/W, random/sequence) is wrong"       );
makeOFConditionConst(EC_ItemEnd,                    OFM_dcmdata, 12, OF_error, "Item end"                                   );
makeOFConditionConst(EC_RepresentationNotFound,     OFM_dcmdata, 13, OF_error, "Pixel representation not found"             );
makeOFConditionConst(EC_CannotChangeRepresentation, OFM_dcmdata, 14, OF_error, "Pixel representation cannot be changed"     );
makeOFConditionConst(EC_UnsupportedEncoding,        OFM_dcmdata, 15, OF_error, "Unsupported compression or encryption"      );
// error code 16 is reserved for zlib-related error messages
makeOFConditionConst(EC_PutbackFailed,              OFM_dcmdata, 17, OF_error, "Parser failure: Putback operation failed"   );
// error code 18 is reserved for file read error messages
// error code 19 is reserved for file write error messages
makeOFConditionConst(EC_DoubleCompressionFilters,   OFM_dcmdata, 20, OF_error, "Too many compression filters"               );
makeOFConditionConst(EC_ApplicationProfileViolated, OFM_dcmdata, 21, OF_error, "Storage media application profile violated" );
// error code 22 is reserved for dcmodify error messages
makeOFConditionConst(EC_InvalidOffset,              OFM_dcmdata, 23, OF_error, "Invalid offset"                             );
makeOFConditionConst(EC_TooManyBytesRequested,      OFM_dcmdata, 24, OF_error, "Too many bytes requested"                   );
// error code 25 is reserved for tag path parsing error messages
makeOFConditionConst(EC_InvalidBasicOffsetTable,    OFM_dcmdata, 26, OF_error, "Invalid basic offset table"                 );
makeOFConditionConst(EC_ElemLengthLargerThanItem,   OFM_dcmdata, 27, OF_error, "Length of element larger than explicit length of surrounding item" );
makeOFConditionConst(EC_FileMetaInfoHeaderMissing,  OFM_dcmdata, 28, OF_error, "File meta information header missing"       );
makeOFConditionConst(EC_SeqOrItemContentOverflow,   OFM_dcmdata, 29, OF_error, "Item or sequence content exceeds maximum of 32-bit length field");
makeOFConditionConst(EC_ValueRepresentationViolated,OFM_dcmdata, 30, OF_error, "Value Representation violated"              );
makeOFConditionConst(EC_ValueMultiplicityViolated,  OFM_dcmdata, 31, OF_error, "Value Multiplicity violated"                );
makeOFConditionConst(EC_MaximumLengthViolated,      OFM_dcmdata, 32, OF_error, "Maximum VR length violated"                 );
makeOFConditionConst(EC_ElemLengthExceeds16BitField,OFM_dcmdata, 33, OF_error, "Length of element value exceeds maximum of 16-bit length field" );
makeOFConditionConst(EC_DelimitationItemMissing,    OFM_dcmdata, 34, OF_error, "Item- or SequenceDelimitationItem missing at end of sequence" );
// error codes 35..36 are reserved for specific character set error messages (see below)
// error code 37 is reserved for XML conversion error messages (see below)

const unsigned short EC_CODE_CannotSelectCharacterSet  = 35;
const unsigned short EC_CODE_CannotConvertCharacterSet = 36;
const unsigned short EC_CODE_CannotConvertToXML        = 37;

const char *dcmErrorConditionToString(OFCondition cond)
{
  return cond.text();
}


/*
** CVS/RCS Log:
** $Log: dcerror.cc,v $
** Revision 1.32  2012-02-15 14:50:42  uli
** Removed dependency on static initialization order from OFCondition.
** All static condition objects are now created via makeOFConditionConst()
** in a way that doesn't need a constructor to run. This should only break
** code which defines its own condition objects, all other changes are
** backwards compatible.
**
** Revision 1.31  2011-12-02 11:02:50  joergr
** Various fixes after first commit of the Native DICOM Model format support.
**
** Revision 1.30  2011-12-01 13:14:02  onken
** Added support for Application Hosting's Native DICOM Model xml format
** to dcm2xml.
**
** Revision 1.29  2011-11-01 14:54:04  joergr
** Added support for code extensions (escape sequences) according to ISO 2022
** to the character set conversion code.
**
** Revision 1.28  2011-10-26 16:13:01  joergr
** Added helper class for converting between different DICOM character sets.
** This initial version only supports the conversion to UTF-8 (Unicode) and only
** from DICOM characters sets without code extension techniques (i.e. ISO 2022).
**
** Revision 1.27  2011-05-11 10:03:36  uli
** Improved handling of files which ended before the end of a sequence.
**
** Revision 1.26  2010-10-14 13:14:07  joergr
** Updated copyright header. Added reference to COPYRIGHT file.
**
** Revision 1.25  2010-02-25 13:51:15  joergr
** Fixed issue with element values which exceed the maximum of a 16-bit length
** field.
**
** Revision 1.24  2009-08-03 09:02:59  joergr
** Added methods that check whether a given string value conforms to the VR and
** VM definitions of the DICOM standards.
**
** Revision 1.23  2009-03-05 13:35:07  onken
** Added checks for sequence and item lengths which prevents overflow in length
** field, if total length of contained items (or sequences) exceeds 32-bit
** length field. Also introduced new flag (default: enabled) for writing
** in explicit length mode, which allows for automatically switching encoding
** of only that very sequence/item to undefined length coding (thus permitting
** to actually write the file).
**
** Revision 1.22  2009-02-11 16:35:27  joergr
** Introduced new error code EC_FileMetaInfoHeaderMissing.
**
** Revision 1.21  2009-02-04 17:59:15  joergr
** Fixed various layout and formatting issues.
**
** Revision 1.20  2009-02-04 14:06:01  onken
** Changed parser to make use of the new error ignoring flag when parsing.
** Added check (makes use of new flag) that notes whether an element's value is
** specified larger than the surrounding item (applicable for explicit length
** coding).
**
** Revision 1.19  2009-02-04 10:16:51  joergr
** Introduced new error code EC_InvalidBasicOffsetTable.
**
** Revision 1.18  2008-12-05 13:51:13  onken
** Introduced new error code number for specific findOrCreatePath() errors.
**
** Revision 1.17  2007-06-13 14:45:47  meichel
** Added module code OFM_dcmjpls and some new error codes.
**
** Revision 1.16  2005/12/08 15:41:09  meichel
** Changed include path schema for all DCMTK header files
**
** Revision 1.15  2004/11/05 17:20:31  onken
** Added reservation for dcmodify error messages.
**
** Revision 1.14  2002/12/06 12:18:57  joergr
** Added new error status "EC_ApplicationProfileViolated".
**
** Revision 1.13  2002/08/27 16:55:47  meichel
** Initial release of new DICOM I/O stream classes that add support for stream
**   compression (deflated little endian explicit VR transfer syntax)
**
** Revision 1.12  2001/09/25 17:19:50  meichel
** Adapted dcmdata to class OFCondition
**
** Revision 1.11  2001/06/01 15:49:04  meichel
** Updated copyright header
**
** Revision 1.10  2000/03/08 16:26:35  meichel
** Updated copyright header.
**
** Revision 1.9  2000/02/23 15:11:52  meichel
** Corrected macro for Borland C++ Builder 4 workaround.
**
** Revision 1.8  2000/02/01 10:12:07  meichel
** Avoiding to include <stdlib.h> as extern "C" on Borland C++ Builder 4,
**   workaround for bug in compiler header files.
**
** Revision 1.7  1999/03/31 09:25:27  meichel
** Updated copyright header in module dcmdata
**
** Revision 1.6  1997/10/01 08:44:12  meichel
** Including <unistd.h> if available.
**
** Revision 1.5  1997/07/21 08:17:41  andreas
** - New environment for encapsulated pixel representations. DcmPixelData
**   can contain different representations and uses codecs to convert
**   between them. Codecs are derived from the DcmCodec class. New error
**   codes are introduced for handling of representations. New internal
**   value representation (only for ident()) for PixelData
**
** Revision 1.4  1997/05/22 16:55:05  andreas
** - Added new error code EC_NotImplemented
**
** Revision 1.3  1996/01/29 13:38:26  andreas
** - new put method for every VR to put value as a string
** - better and unique print methods
**
** Revision 1.2  1996/01/05 13:27:36  andreas
** - changed to support new streaming facilities
** - unique read/write methods for file and block transfer
** - more cleanups
**
** Revision 1.1  1995/11/23 17:02:44  hewett
** Updated for loadable data dictionary.  Some cleanup (more to do).
**
*/
