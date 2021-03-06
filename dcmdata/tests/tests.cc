/*
 *
 *  Copyright (C) 2011, OFFIS e.V.
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
 *  Author:  Uli Schlachter
 *
 *  Purpose: main test program
 *
 *  Last Update:      $Author: joergr $
 *  Update Date:      $Date: 2011-12-02 11:02:53 $
 *  CVS/RCS Revision: $Revision: 1.15 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */

#include "dcmtk/config/osconfig.h"

#include "dcmtk/ofstd/oftest.h"

OFTEST_REGISTER(dcmdata_partialElementAccess);
OFTEST_REGISTER(dcmdata_i2d_bmp);
OFTEST_REGISTER(dcmdata_checkStringValue);
OFTEST_REGISTER(dcmdata_determineVM);
OFTEST_REGISTER(dcmdata_getValueFromString);
OFTEST_REGISTER(dcmdata_pathAccess);
OFTEST_REGISTER(dcmdata_dateTime);
OFTEST_REGISTER(dcmdata_decimalString_1);
OFTEST_REGISTER(dcmdata_decimalString_2);
OFTEST_REGISTER(dcmdata_decimalString_3);
OFTEST_REGISTER(dcmdata_decimalString_4);
OFTEST_REGISTER(dcmdata_personName);
OFTEST_REGISTER(dcmdata_uniqueIdentifier_1);
OFTEST_REGISTER(dcmdata_uniqueIdentifier_2);
OFTEST_REGISTER(dcmdata_elementLength_EVR_AE);
OFTEST_REGISTER(dcmdata_elementLength_EVR_AS);
OFTEST_REGISTER(dcmdata_elementLength_EVR_AT);
OFTEST_REGISTER(dcmdata_elementLength_EVR_CS);
OFTEST_REGISTER(dcmdata_elementLength_EVR_DA);
OFTEST_REGISTER(dcmdata_elementLength_EVR_DS);
OFTEST_REGISTER(dcmdata_elementLength_EVR_DT);
OFTEST_REGISTER(dcmdata_elementLength_EVR_FD);
OFTEST_REGISTER(dcmdata_elementLength_EVR_FL);
OFTEST_REGISTER(dcmdata_elementLength_EVR_IS);
OFTEST_REGISTER(dcmdata_elementLength_EVR_LO);
OFTEST_REGISTER(dcmdata_elementLength_EVR_LT);
OFTEST_REGISTER(dcmdata_elementLength_EVR_OB);
OFTEST_REGISTER(dcmdata_elementLength_EVR_OF);
OFTEST_REGISTER(dcmdata_elementLength_EVR_OW);
OFTEST_REGISTER(dcmdata_elementLength_EVR_OverlayData);
OFTEST_REGISTER(dcmdata_elementLength_EVR_PN);
OFTEST_REGISTER(dcmdata_elementLength_EVR_PixelData);
OFTEST_REGISTER(dcmdata_elementLength_EVR_SH);
OFTEST_REGISTER(dcmdata_elementLength_EVR_SL);
OFTEST_REGISTER(dcmdata_elementLength_EVR_SQ);
OFTEST_REGISTER(dcmdata_elementLength_EVR_SS);
OFTEST_REGISTER(dcmdata_elementLength_EVR_ST);
OFTEST_REGISTER(dcmdata_elementLength_EVR_TM);
OFTEST_REGISTER(dcmdata_elementLength_EVR_UI);
OFTEST_REGISTER(dcmdata_elementLength_EVR_UL);
OFTEST_REGISTER(dcmdata_elementLength_EVR_UN);
OFTEST_REGISTER(dcmdata_elementLength_EVR_UNKNOWN);
OFTEST_REGISTER(dcmdata_elementLength_EVR_UNKNOWN2B);
OFTEST_REGISTER(dcmdata_elementLength_EVR_US);
OFTEST_REGISTER(dcmdata_elementLength_EVR_UT);
OFTEST_REGISTER(dcmdata_elementLength_EVR_lt);
OFTEST_REGISTER(dcmdata_elementLength_EVR_na);
OFTEST_REGISTER(dcmdata_elementLength_EVR_ox);
OFTEST_REGISTER(dcmdata_elementLength_EVR_up);
OFTEST_REGISTER(dcmdata_elementLength_EVR_xs);
OFTEST_REGISTER(dcmdata_elementLength_pixelItem);
OFTEST_REGISTER(dcmdata_elementLength_pixelSequence);
OFTEST_REGISTER(dcmdata_parser_missingSequenceDelimitationItem);
OFTEST_REGISTER(dcmdata_parser_oddLengthPartialValue_lastItem);
OFTEST_REGISTER(dcmdata_parser_oddLengthPartialValue_notLastItem);
OFTEST_REGISTER(dcmdata_parser_wrongExplicitVRinDataset_default);
OFTEST_REGISTER(dcmdata_parser_wrongExplicitVRinDataset_preferDataDict);
OFTEST_REGISTER(dcmdata_readingDataDictionary);
OFTEST_REGISTER(dcmdata_usingDataDictionary);
OFTEST_REGISTER(dcmdata_specificCharacterSet_1);
OFTEST_REGISTER(dcmdata_specificCharacterSet_2);
OFTEST_REGISTER(dcmdata_specificCharacterSet_3);
OFTEST_REGISTER(dcmdata_specificCharacterSet_4);
OFTEST_MAIN("dcmdata")


/*
**
** CVS/RCS Log:
** $Log: tests.cc,v $
** Revision 1.15  2011-12-02 11:02:53  joergr
** Various fixes after first commit of the Native DICOM Model format support.
**
** Revision 1.14  2011-12-01 13:13:56  onken
** Added support for Application Hosting's Native DICOM Model xml format
** to dcm2xml.
**
** Revision 1.13  2011-11-15 08:04:58  joergr
** Added regression tests for class DcmUniqueIdentifier.
**
** Revision 1.12  2011-11-08 15:51:39  joergr
** Added support for converting files, datasets and element values to any DICOM
** character set that does not require code extension techniques (if compiled
** with and supported by libiconv), not only to UTF-8 as before.
**
** Revision 1.11  2011-11-01 14:54:07  joergr
** Added support for code extensions (escape sequences) according to ISO 2022
** to the character set conversion code.
**
** Revision 1.10  2011-10-26 16:13:03  joergr
** Added helper class for converting between different DICOM character sets.
** This initial version only supports the conversion to UTF-8 (Unicode) and only
** from DICOM characters sets without code extension techniques (i.e. ISO 2022).
**
** Revision 1.9  2011-10-18 14:03:05  joergr
** Added test program for various "string value" methods.
**
** Revision 1.8  2011-10-13 14:12:14  joergr
** Added tests for class DcmDecimalString, especially for getFloat64Vector().
**
** Revision 1.7  2011-08-23 14:24:30  joergr
** Fixed another inconsistent test name.
**
** Revision 1.6  2011-08-17 14:35:59  uli
** Added a small test for the data dictionary.
**
** Revision 1.5  2011-08-08 11:04:06  joergr
** Added test cases for new parser flag "dcmPreferVRFromDataDictionary".
**
** Revision 1.4  2011-07-06 13:04:18  uli
** Fixed some inconsistencies in test names.
**
** Revision 1.3  2011-07-06 12:25:03  uli
** Added tests for some bugs which we recently fixed.
**
** Revision 1.2  2011-06-07 07:06:57  uli
** Added test cases for DcmElement::calcElementLength().
**
** Revision 1.1  2011-05-25 10:05:55  uli
** Imported oftest and converted existing tests to oftest.
**
**
*/
