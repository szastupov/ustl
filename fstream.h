// This file is part of the ustl library, an STL implementation.
//
// Copyright (C) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.
//
// fstream.h
//

#ifndef FSTREAM_H_056E10F70EAD416443E3B36A2D6B5FA3
#define FSTREAM_H_056E10F70EAD416443E3B36A2D6B5FA3

#include "uios.h"
#include "ustring.h"

namespace ustl {

/// \class fstream fstream.h ustl.h
///
/// \brief Implements file operations.
///
/// This is not implemented as a stream, but rather as a base for one. You
/// should use ifstream or ofstream if you want flow operators. Otherwise
/// this only implements functions for binary i/o.
///
class fstream {
public:
			fstream (void);
    explicit		fstream (const char* filename, ios::openmode mode = ios::in | ios::out);
    explicit		fstream (int nfd, const char* filename = string::empty_string);
		       ~fstream (void) throw();
    void		open (const char* filename, ios::openmode mode, mode_t perms = 0644);
    void		attach (int nfd, const char* filename = string::empty_string);
    void		detach (void);
    void		close (void);
    off_t		read (void* p, off_t n);
    off_t		write (const void* p, off_t n);
    off_t		size (void) const;
    off_t		seek (off_t n, ios::seekdir whence = ios::beg);
    off_t		pos (void) const;
    inline int		fd (void) const		{ return (m_fd); }
    inline bool		is_open (void) const	{ return (fd() >= 0); }
    inline off_t	tellg (void) const	{ return (pos()); }
    inline off_t	tellp (void) const	{ return (pos()); }
    inline void		seekg (off_t n, ios::seekdir whence = ios::beg)	{ seek (n, whence); }
    inline void		seekp (off_t n, ios::seekdir whence = ios::beg)	{ seek (n, whence); }
    inline const string& name (void) const	{ return (m_Filename); }
    inline ios::iostate	rdstate (void) const	{ return (m_State); }
    inline bool		bad (void) const	{ return (rdstate() & ios::badbit); }
    inline bool		good (void) const	{ return (rdstate() == ios::goodbit); }
    inline bool		fail (void) const	{ return (rdstate() & (ios::badbit | ios::failbit)); }
    inline bool		operator! (void) const	{ return (fail()); }
    inline void		clear (ios::iostate v = ios::goodbit)	{ m_State = v; }
    inline void		setstate (ios::iostate v)		{ m_State |= v; }
    inline ios::iostate	exceptions (void) const			{ return (m_Exceptions); }
    inline ios::iostate	exceptions (ios::iostate v)		{ return (m_Exceptions = v); }
private:
    inline bool		set_and_throw (ios::iostate v)		{ setstate(v); return (exceptions() & v); }
    static int		om_to_flags (ios::openmode m);
private:
    string		m_Filename;	///< Currently open filename.
    int			m_fd;		///< Currently open file descriptor.
    uint16_t		m_State;	///< Open state, using ios::iostate_bits.
    uint16_t		m_Exceptions;	///< Exception flags, using ios::iostate_bits.
};

}

#endif

