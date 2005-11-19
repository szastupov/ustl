// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// fdostringstream.h
//

#ifndef FDOSTREAM_H_5E27FC3D530BF3CA04D6C73F5700EECC
#define FDOSTREAM_H_5E27FC3D530BF3CA04D6C73F5700EECC

#include "sistream.h"
#include "sostream.h"
#include "fstream.h"

#ifndef WITHOUT_CIN_COUT_CERR
namespace ustl {

class string;

/// \class fdostringstream fdostream.h ustl.h
/// \ingroup DeviceStreams
/// \brief A string stream that writes to an fd. Implements cout and cerr.
class fdostringstream : public ostringstream {
public:
    explicit			fdostringstream (int fd);
    virtual		       ~fdostringstream (void);
    void			flush (void);
    virtual size_type		overflow (size_type n = 1);
private:
    int				m_Fd;
};

/// \class fdistringstream fdostream.h ustl.h
/// \ingroup DeviceStreams
/// \brief A string stream that reads from an fd. Implements cin.
class fdistringstream : public istringstream {
public:
    explicit			fdistringstream (int fd);
    virtual size_type		underflow (size_type n = 1);
    void			sync (void);
private:
    string			m_Buffer;
    int				m_Fd;
};

extern fdostringstream cout, cerr;
extern fdistringstream cin;

} // namespace ustl

#endif
#endif

