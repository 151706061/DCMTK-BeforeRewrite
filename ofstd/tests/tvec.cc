/*
 *
 *  Copyright (C) 2010-2011, OFFIS e.V.
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
 *  Module:  ofstd
 *
 *  Author:  Uli Schlachter
 *
 *  Purpose: test programm for class OFVec
 *
 *  Last Update:      $Author: uli $
 *  Update Date:      $Date: 2011-05-25 10:05:59 $
 *  CVS/RCS Revision: $Revision: 1.4 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */

#include "dcmtk/config/osconfig.h"

#define OFTEST_OFSTD_ONLY
#include "dcmtk/ofstd/oftest.h"
#include "dcmtk/ofstd/ofvector.h"


OFTEST(ofstd_OFVector)
{
    OFVector<int> m;
    OFVector<int>::iterator it;
    int i;

    // Just for the fun of it, test if an empty vector is empty
    // (and if one can have vectors in other vectors).
    OFCHECK(OFVector<OFVector<char> >().empty());

    // Test whether iterators stay valid if enough space was reserved
    m.reserve(6);
    it = m.begin();

    // Fill with some entries
    for (i = 1; i <= 6; ++i)
        m.push_back(i);

    OFCHECK(it == m.begin());
    OFCHECK(it != m.end());

    // verify that the entries where really added
    OFCHECK_EQUAL(m.size(), 6);
    for (i = 1; i <= 6; ++i)
        OFCHECK_EQUAL(*(it++), i);

    OFCHECK(it == m.end());

    // check erase()
    for (i = 1; i <= 3; ++i)
    {
        OFCHECK_EQUAL(m.at(0), i);
        OFCHECK_EQUAL(m[0], i);
        m.erase(m.begin());
    }

    OFCHECK_EQUAL(m.size(), 3);

    // test the copy-constructor
    OFVector<int> n(m);
    OFCHECK_EQUAL(n.size(), 3);

    // does pop_back() work?
    n.pop_back();
    OFCHECK_EQUAL(n.size(), 2);
    OFCHECK_EQUAL(m.size(), 3);

    // test operator= and the range-constructor
    n = OFVector<int>(m.begin(), m.end());
    OFCHECK_EQUAL(n.size(), 3);
    OFCHECK_EQUAL(n[1], 5);
    n[1] = 42;
    OFCHECK_EQUAL(n[1], 42);
    n.pop_back();

    // does swap do what it is supposed to do?
    it = m.begin();
    m.swap(n);
    OFCHECK(it == n.begin());
    OFCHECK_EQUAL(n.size(), 3);
    OFCHECK_EQUAL(m.size(), 2);
    OFCHECK_EQUAL(n[1], 5);
    OFCHECK_EQUAL(m[1], 42);

    // does clear() really clear the vector?
    n.clear();
    OFCHECK(n.empty());

    // this should produce a vector which elements 0 to 49 with value "5",
    // then the values "4" and "42" once and then another 50 times "6".
    n = OFVector<int>(50, 5);
    n.resize(100, 6);
    OFCHECK_EQUAL(n.size(), 100);

    n.insert(n.begin() + 50, m.begin(), m.end());
    OFCHECK_EQUAL(n.size(), 102);

    it = n.begin();
    for (i = 0; it != n.end(); ++it, ++i)
    {
        if (i < 50)
            OFCHECK_EQUAL(*it, 5);
        else if(i == 50)
            OFCHECK_EQUAL(*it, 4);
        else if(i == 51)
            OFCHECK_EQUAL(*it, 42);
        else
            OFCHECK_EQUAL(*it, 6);
    }
}


/*
**
** CVS/RCS Log:
** $Log: tvec.cc,v $
** Revision 1.4  2011-05-25 10:05:59  uli
** Imported oftest and converted existing tests to oftest.
**
** Revision 1.3  2010-10-14 13:15:16  joergr
** Updated copyright header. Added reference to COPYRIGHT file.
**
** Revision 1.2  2010-10-08 13:52:14  uli
** Fixed an invalid copyright date.
**
** Revision 1.1  2010-10-08 13:25:33  uli
** Implement OFVector.
**
**
**
**
*/
