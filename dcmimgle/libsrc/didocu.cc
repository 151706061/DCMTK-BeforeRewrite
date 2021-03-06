/*
 *
 *  Copyright (C) 1996-2011, OFFIS e.V.
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
 *  Module:  dcmimgle
 *
 *  Author:  Joerg Riesmeier
 *
 *  Purpose: DicomDocument (Source)
 *
 *  Last Update:      $Author: joergr $
 *  Update Date:      $Date: 2011-11-11 11:05:53 $
 *  CVS/RCS Revision: $Revision: 1.30 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */


#include "dcmtk/config/osconfig.h"

#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/ofstd/ofstring.h"

#include "dcmtk/dcmimgle/didocu.h"
#include "dcmtk/dcmimgle/diutils.h"


/*----------------*
 *  constructors  *
 *----------------*/

DiDocument::DiDocument(const char *filename,
                       const unsigned long flags,
                       const unsigned long fstart,
                       const unsigned long fcount)
  : Object(NULL),
    FileFormat(new DcmFileFormat()),
    PixelData(NULL),
    Xfer(EXS_Unknown),
    FrameStart(fstart),
    FrameCount(fcount),
    Flags(flags),
    PhotometricInterpretation()
{
    if (FileFormat)
    {
        if (FileFormat->loadFile(filename).bad())
        {
            DCMIMGLE_ERROR("can't read file '" << filename << "'");
            delete FileFormat;
            FileFormat = NULL;
        } else {
            Object = FileFormat->getDataset();
            if (Object != NULL)
            {
                Xfer = OFstatic_cast(DcmDataset *, Object)->getOriginalXfer();
                convertPixelData();
            }
        }
    }
}


DiDocument::DiDocument(DcmObject *object,
                       const E_TransferSyntax xfer,
                       const unsigned long flags,
                       const unsigned long fstart,
                       const unsigned long fcount)
  : Object(NULL),
    FileFormat(NULL),
    PixelData(NULL),
    Xfer(xfer),
    FrameStart(fstart),
    FrameCount(fcount),
    Flags(flags),
    PhotometricInterpretation()
{
    if (object != NULL)
    {
        const DcmEVR classType = object->ident();
        // check whether given DICOM object has a valid type
        if (classType == EVR_fileFormat)
        {
            // store reference to DICOM file format to be deleted on object destruction
            if (Flags & CIF_TakeOverExternalDataset)
                FileFormat = OFstatic_cast(DcmFileFormat *, object);
            Object = OFstatic_cast(DcmFileFormat *, object)->getDataset();
        }
        else if ((classType == EVR_dataset) || (classType == EVR_item))
            Object = object;
        else
            DCMIMGLE_ERROR("invalid DICOM object passed to constructor (wrong class)");
        if (Object != NULL)
        {
            // try to determine the transfer syntax from the given object
            if (Xfer == EXS_Unknown)
            {
                // check type before casting the object
                if (Object->ident() == EVR_dataset)
                    Xfer = OFstatic_cast(DcmDataset *, Object)->getOriginalXfer();
                else // could only be an item
                    DCMIMGLE_WARN("can't determine original transfer syntax from given DICOM object");
            }
            convertPixelData();
        }
    }
}


void DiDocument::convertPixelData()
{
    DcmStack pstack;
    DcmXfer xfer(Xfer);
    DCMIMGLE_DEBUG("transfer syntax of DICOM dataset: " << xfer.getXferName() << " (" << xfer.getXferID() << ")");
    // only search on main dataset level
    if (search(DCM_PixelData, pstack))
    {
        DcmObject *pobject = pstack.top();
        if (pobject != NULL)
        {
            // check for correct class before type casting
            if (pobject->ident() == EVR_PixelData)
            {
                PixelData = OFstatic_cast(DcmPixelData *, pobject);
                // check for a special (faulty) case where the original pixel data is uncompressed and
                // the transfer syntax of the dataset refers to encapsulated format (i.e. compression)
                if (Object->ident() == EVR_dataset)
                {
                    E_TransferSyntax repType = EXS_Unknown;
                    const DcmRepresentationParameter *repParam = NULL;
                    PixelData->getOriginalRepresentationKey(repType, repParam);
                    if (xfer.isEncapsulated() && !DcmXfer(repType).isEncapsulated())
                    {
                        DCMIMGLE_WARN("pixel data is stored in uncompressed format, although "
                            << "the transfer syntax of the dataset refers to encapsulated format");
                    }
                }
                // convert pixel data to uncompressed format (if required)
                if ((Flags & CIF_DecompressCompletePixelData) || !(Flags & CIF_UsePartialAccessToPixelData))
                {
                    pstack.clear();
                    // push reference to DICOM dataset on the stack (required for decompression process)
                    pstack.push(Object);
                    // dummy stack entry
                    pstack.push(PixelData);
                    if (PixelData->chooseRepresentation(EXS_LittleEndianExplicit, NULL, pstack).good())
                    {
                        // set transfer syntax to unencapsulated/uncompressed
                        if (xfer.isEncapsulated())
                        {
                            Xfer = EXS_LittleEndianExplicit;
                            DCMIMGLE_DEBUG("decompressed complete pixel data in memory: " << PixelData->getLength(Xfer) << " bytes");
                        }
                    } else
                        DCMIMGLE_ERROR("can't change to unencapsulated representation for pixel data");
                }
                // determine color model of the decompressed image
                OFCondition status = PixelData->getDecompressedColorModel(OFstatic_cast(DcmItem *, Object), PhotometricInterpretation);
                if (status.bad())
                {
                    DCMIMGLE_ERROR("can't determine 'PhotometricInterpretation' of decompressed image");
                    DCMIMGLE_DEBUG("DcmPixelData::getDecompressedColorModel() returned: " << status.text());
                }
            } else {
                DCMIMGLE_ERROR("invalid pixel data in DICOM dataset (wrong class)");
                DCMIMGLE_DEBUG("found PixelData " << DCM_PixelData << " as an instance of the class for VR '"
                    << OFSTRING_GUARD(DcmVR(pobject->ident()).getVRName()) << "' instead of '"
                    << OFSTRING_GUARD(DcmVR(EVR_PixelData).getVRName()) << "'");
            }
        } else
            DCMIMGLE_ERROR("invalid pixel data in DICOM dataset");
    } else
        DCMIMGLE_ERROR("no pixel data found in DICOM dataset");
}


/*--------------*
 *  destructor  *
 *--------------*/

DiDocument::~DiDocument()
{
    // DICOM image loaded from file: delete file format (and data set)
    if (FileFormat != NULL)
        delete FileFormat;
    // DICOM image loaded from external data set: only delete if flag is set
    else if (Flags & CIF_TakeOverExternalDataset)
        delete Object;
}


/********************************************************************/


DcmElement *DiDocument::search(const DcmTagKey &tag,
                               DcmObject *obj) const
{
    DcmStack stack;
    if (obj == NULL)
        obj = Object;
    // only search on main dataset level
    if ((obj != NULL) && (obj->search(tag, stack, ESM_fromHere, OFFalse /* searchIntoSub */) == EC_Normal) &&
        (stack.top()->getLength(Xfer) > 0))
    {
        return OFstatic_cast(DcmElement *, stack.top());
    }
    return NULL;
}


/********************************************************************/


int DiDocument::search(const DcmTagKey &tag,
                       DcmStack &pstack) const
{
    if (pstack.empty())
        pstack.push(Object);
    DcmObject *obj = pstack.top();
    if ((obj != NULL) && (obj->search(tag, pstack, ESM_fromHere, OFFalse /* searchIntoSub */) == EC_Normal) &&
        (pstack.top()->getLength(Xfer) > 0))
            return 1;
    return 0;
}


/********************************************************************/


unsigned long DiDocument::getVM(const DcmTagKey &tag) const
{
    DcmElement *elem = search(tag);
    if (elem != NULL)
        return elem->getVM();
    return 0;
}


unsigned long DiDocument::getValue(const DcmTagKey &tag,
                                   Uint16 &returnVal,
                                   const unsigned long pos,
                                   DcmItem *item,
                                   const OFBool allowSigned) const
{
    return getElemValue(search(tag, item), returnVal, pos, allowSigned);
}


unsigned long DiDocument::getValue(const DcmTagKey &tag,
                                   Sint16 &returnVal,
                                   const unsigned long pos,
                                   DcmItem *item) const
{
    DcmElement *elem = search(tag, item);
    if (elem != NULL)
    {
        if (elem->getSint16(returnVal, pos).good())
            return elem->getVM();
    }
    return 0;
}


unsigned long DiDocument::getValue(const DcmTagKey &tag,
                                   Uint32 &returnVal,
                                   const unsigned long pos,
                                   DcmItem *item) const
{
    DcmElement *elem = search(tag, item);
    if (elem != NULL)
    {
        if (elem->getUint32(returnVal, pos).good())
            return elem->getVM();
    }
    return 0;
}


unsigned long DiDocument::getValue(const DcmTagKey &tag,
                                   Sint32 &returnVal,
                                   const unsigned long pos,
                                   DcmItem *item) const
{
    DcmElement *elem = search(tag, item);
    if (elem != NULL)
    {
        if (elem->getSint32(returnVal, pos).good())
            return elem->getVM();
    }
    return 0;
}


unsigned long DiDocument::getValue(const DcmTagKey &tag,
                                   double &returnVal,
                                   const unsigned long pos,
                                   DcmItem *item) const
{
    DcmElement *elem = search(tag, item);
    if (elem != NULL)
    {
        if (elem->getFloat64(returnVal, pos).good())
            return elem->getVM();
    }
    return 0;
}


unsigned long DiDocument::getValue(const DcmTagKey &tag,
                                   const Uint16 *&returnVal,
                                   DcmItem *item) const
{
    DcmElement *elem = search(tag, item);
    if (elem != NULL)
    {
        Uint16 *val;
        if (elem->getUint16Array(val).good())
        {
            returnVal = val;
            const DcmEVR vr = elem->getVR();
            if ((vr == EVR_OB) || (vr == EVR_OW) || (vr == EVR_lt))
                return elem->getLength(Xfer) / sizeof(Uint16);
            return elem->getVM();
        }
    }
    return 0;
}


unsigned long DiDocument::getValue(const DcmTagKey &tag,
                                   const char *&returnVal,
                                   DcmItem *item) const
{
    return getElemValue(search(tag, item), returnVal);
}


unsigned long DiDocument::getValue(const DcmTagKey &tag,
                                   OFString &returnVal,
                                   const unsigned long pos,
                                   DcmItem *item) const
{
    return getElemValue(search(tag, item), returnVal, pos);
}


unsigned long DiDocument::getSequence(const DcmTagKey &tag,
                                      DcmSequenceOfItems *&seq,
                                      DcmItem *item) const
{
    DcmElement *elem = search(tag, item);
    if ((elem != NULL) && (elem->ident() == EVR_SQ))
        return (seq = OFstatic_cast(DcmSequenceOfItems *, elem))->card();
    return 0;
}


unsigned long DiDocument::getElemValue(const DcmElement *elem,
                                       Uint16 &returnVal,
                                       const unsigned long pos,
                                       const OFBool allowSigned)
{
    if (elem != NULL)
    {
        // remove 'const' to use non-const methods
        if (OFconst_cast(DcmElement *, elem)->getUint16(returnVal, pos).good())
            return OFconst_cast(DcmElement *, elem)->getVM();
        else if (allowSigned)
        {
            // try to retrieve signed value ...
            Sint16 value = 0;
            if (OFconst_cast(DcmElement *, elem)->getSint16(value, pos).good())
            {
                // ... and cast to unsigned type
                returnVal = OFstatic_cast(Uint16, value);
                DCMIMGLE_TRACE("retrieved signed value (" << value << ") at position " << pos
                    << " from element " << OFconst_cast(DcmElement *, elem)->getTag()
                    << ", VR=" << DcmVR(OFconst_cast(DcmElement *, elem)->getVR()).getVRName()
                    << ", VM=" << OFconst_cast(DcmElement *, elem)->getVM());
                return OFconst_cast(DcmElement *, elem)->getVM();
            }
        }
    }
    return 0;
}


unsigned long DiDocument::getElemValue(const DcmElement *elem,
                                       const Uint16 *&returnVal)
{
    if (elem != NULL)
    {
        Uint16 *val;
        // remove 'const' to use non-const methods
        if (OFconst_cast(DcmElement *, elem)->getUint16Array(val).good())
        {
            returnVal = val;
            const DcmEVR vr = OFconst_cast(DcmElement *, elem)->getVR();
            if ((vr == EVR_OW) || (vr == EVR_lt))
                return OFconst_cast(DcmElement *, elem)->getLength(/*Xfer*/) / sizeof(Uint16);
            return OFconst_cast(DcmElement *, elem)->getVM();
        }
    }
    return 0;
}


unsigned long DiDocument::getElemValue(const DcmElement *elem,
                                       const char *&returnVal)
{
    if (elem != NULL)
    {
        char *val;
        // remove 'const' to use non-const methods
        if (OFconst_cast(DcmElement *, elem)->getString(val).good())
        {
            returnVal = val;
            return OFconst_cast(DcmElement *, elem)->getVM();
        }
    }
    return 0;
}


unsigned long DiDocument::getElemValue(const DcmElement *elem,
                                       OFString &returnVal,
                                       const unsigned long pos)
{
    if (elem != NULL)
    {
        // remove 'const' to use non-const methods
        if (OFconst_cast(DcmElement *, elem)->getOFString(returnVal, pos).good())
            return OFconst_cast(DcmElement *, elem)->getVM();
    }
    return 0;
}


/*
 *
 * CVS/RCS Log:
 * $Log: didocu.cc,v $
 * Revision 1.30  2011-11-11 11:05:53  joergr
 * Changed optional DcmObject* parameter into DcmItem* and added this optional
 * parameter to some further getValue() methods.
 *
 * Revision 1.29  2011-04-27 10:01:30  joergr
 * Added more checks on the type of DICOM object passed to the constructor.
 * Added more log messages in order to get details on invalid DICOM objects.
 *
 * Revision 1.28  2011-04-26 16:33:35  joergr
 * Output a warning message in case the pixel data on the main dataset level is
 * uncompressed but the transfer syntax is encapsulated (i.e. compressed).
 *
 * Revision 1.27  2011-02-09 14:16:03  joergr
 * Added check on class of PixelData element before casting to DcmPixelData.
 *
 * Revision 1.26  2010-11-02 11:27:08  joergr
 * Enhanced output to debug logger: Added more details in case of failures.
 *
 * Revision 1.25  2010-10-14 13:14:18  joergr
 * Updated copyright header. Added reference to COPYRIGHT file.
 *
 * Revision 1.24  2010-01-05 11:35:33  joergr
 * Moved determination of pixel data length to the body of a logging macro
 * (since the length value is only used for logging purposes).
 *
 * Revision 1.23  2009-11-25 16:35:42  joergr
 * Adapted code for new approach to access individual frames of a DICOM image.
 * Fixed issue with attributes that use a value representation of US or SS.
 * Added more logging messages.
 *
 * Revision 1.22  2009-10-28 14:26:01  joergr
 * Fixed minor issues in log output.
 *
 * Revision 1.21  2009-10-28 09:53:40  uli
 * Switched to logging mechanism provided by the "new" oflog module.
 *
 * Revision 1.20  2008-02-06 13:39:10  joergr
 * Added check of the return value of DcmElement::getXXX() methods.
 *
 * Revision 1.19  2006/08/15 16:30:11  meichel
 * Updated the code in module dcmimgle to correctly compile when
 *   all standard C++ classes remain in namespace std.
 *
 * Revision 1.18  2006/07/03 14:23:57  joergr
 * Fixed issue with handling of LUT data in DICOM objects with implicit VR.
 *
 * Revision 1.17  2006/02/03 16:53:14  joergr
 * Fixed inconsistent source code layout.
 *
 * Revision 1.16  2005/12/08 15:42:48  meichel
 * Changed include path schema for all DCMTK header files
 *
 * Revision 1.15  2003/12/08 15:13:18  joergr
 * Adapted type casts to new-style typecast operators defined in ofcast.h.
 *
 * Revision 1.14  2002/08/21 09:51:47  meichel
 * Removed DicomImage and DiDocument constructors that take a DcmStream
 *   parameter
 *
 * Revision 1.13  2002/06/26 16:10:15  joergr
 * Added new methods to get the explanation string of stored VOI windows and
 * LUTs (not only of the currently selected VOI transformation).
 * Added configuration flag that enables the DicomImage class to take the
 * responsibility of an external DICOM dataset (i.e. delete it on destruction).
 *
 * Revision 1.12  2001/11/29 16:59:52  joergr
 * Fixed bug in dcmimgle that caused incorrect decoding of some JPEG compressed
 * images (certain DICOM attributes, e.g. photometric interpretation, might
 * change during decompression process).
 *
 * Revision 1.11  2001/11/19 12:57:04  joergr
 * Adapted code to support new dcmjpeg module and JPEG compressed images.
 *
 * Revision 1.10  2001/06/01 15:49:54  meichel
 * Updated copyright header
 *
 * Revision 1.9  2000/09/12 10:06:14  joergr
 * Corrected bug: wrong parameter for attribute search routine led to crashes
 * when multiple pixel data attributes were contained in the dataset (e.g.
 * IconImageSequence).
 *
 * Revision 1.8  2000/04/28 12:33:42  joergr
 * DebugLevel - global for the module - now derived from OFGlobal (MF-safe).
 *
 * Revision 1.7  2000/04/27 13:10:26  joergr
 * Dcmimgle library code now consistently uses ofConsole for error output.
 *
 * Revision 1.6  2000/03/08 16:24:27  meichel
 * Updated copyright header.
 *
 * Revision 1.5  2000/03/03 14:09:18  meichel
 * Implemented library support for redirecting error messages into memory
 *   instead of printing them to stdout/stderr for GUI applications.
 *
 * Revision 1.4  1999/04/28 15:01:43  joergr
 * Introduced new scheme for the debug level variable: now each level can be
 * set separately (there is no "include" relationship).
 *
 * Revision 1.3  1998/12/16 16:11:14  joergr
 * Added methods to use getOFString from class DcmElement (incl. multi value
 * fields).
 *
 * Revision 1.2  1998/12/14 17:33:45  joergr
 * Added (simplified) methods to return values of a given DcmElement object.
 *
 * Revision 1.1  1998/11/27 15:53:36  joergr
 * Added copyright message.
 * Added static methods to return the value of a given element.
 * Added support of frame start and count for future use (will be explained
 * later if it is fully implemented).
 *
 * Revision 1.9  1998/07/01 08:39:35  joergr
 * Minor changes to avoid compiler warnings (gcc 2.8.1 with additional
 * options), e.g. add copy constructors.
 *
 * Revision 1.8  1998/06/25 08:52:05  joergr
 * Added compatibility mode to support ACR-NEMA images and wrong
 * palette attribute tags.
 *
 * Revision 1.7  1998/05/11 14:52:28  joergr
 * Added CVS/RCS header to each file.
 *
 *
 */
