// This file is part of the ustl library, an STL implementation.
// Copyright (C) 2003 by Mike Sharov <msharov@talentg.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
// Boston, MA  02111-1307  USA.
//
// memblock.h
//

#ifndef MEMBLOCK_H
#define MEMBLOCK_H

#include "memlink.h"

namespace ustl {

//
/// \brief Allocated memory block.
//
/// Adds memory management capabilities to memlink. Uses malloc and realloc to
/// maintain the internal pointer, but only if allocated using members of this class,
/// or if linked to using the Manage() member function. Managed memory is automatically
/// freed in the destructor.
///
/// \see memlink
///
class memblock : public memlink {
public:
    static const size_t c_PageSize = 64;		///< The default minimum allocation unit.
public:
    inline 		memblock (void);
    explicit		memblock (size_t n);
    inline 		memblock (void* p, size_t n);
    inline		memblock (const void* p, size_t n);
    inline explicit	memblock (const cmemlink& b);
    inline explicit	memblock (const memlink& b);
    explicit		memblock (const memblock& b);
    inline virtual     ~memblock (void);
    const memblock&	operator= (const cmemlink& b);
    const memblock&	operator= (const memlink& b);
    const memblock&	operator= (const memblock& b);
    void		assign (const cmemlink& l);
    inline void		swap (memblock& l);
    void		reserve (size_t newSize, bool bExact = true);
    inline void		resize (size_t newSize, bool bExact = true);
    iterator		insert (iterator start, size_t size);
    iterator		erase (iterator start, size_t size);
    inline void		clear (void);
    void		deallocate (void);
    void		manage (void* p, size_t n);
    inline void		manage (memlink& l);
    inline size_t	capacity (void) const;
    inline virtual void	unlink (void);
    inline size_t	max_size (void) const;
    void		read (istream& is);
    void		read_file (const char* filename);
    inline bool		is_linked (void) const;
private:
    size_t		m_AllocatedSize;	///< Number of bytes allocated by Resize.
};

/// Allocates 0 bytes for the internal block.
inline memblock::memblock (void)
: memlink (),
  m_AllocatedSize (0)
{
}

/// links to \p p, \p n. Data can be modified but will not be freed.
inline memblock::memblock (void* p, size_t n)
: memlink (p, n),
  m_AllocatedSize (0)
{
}

/// links to \p p, \p n. Data can not be modified and will not be freed.
inline memblock::memblock (const void* p, size_t n)
: memlink (p, n),
  m_AllocatedSize (0)
{
}

/// Links to what \p b is linked to.
inline memblock::memblock (const cmemlink& b)
: memlink (b),
  m_AllocatedSize (0)
{
}

/// Links to what \p b is linked to.
inline memblock::memblock (const memlink& b)
: memlink (b),
  m_AllocatedSize (0)
{
}

/// Frees internal data, if appropriate
/// Only if the block was allocated using resize, or linked to using Manage,
/// will it be freed. Also, Derived classes should call DestructBlock from
/// their destructor, because upstream virtual functions are unavailable at
/// this point and will not be called automatically.
///
inline memblock::~memblock (void)
{
    if (!is_linked())
	deallocate();
}

/// Returns the number of bytes allocated.
inline size_t memblock::capacity (void) const
{
    return (m_AllocatedSize);
}

/// Assumes control over block pointed to by \p l
inline void memblock::manage (memlink& l)
{
    manage (l.begin(), l.size());
}

/// Returns the maximum possible size of the block
inline size_t memblock::max_size (void) const
{
    return (numeric_limits<size_t>::max() / elementSize());
}

/// Resizes the block to 0
inline void memblock::clear (void)
{
    resize (0);
}

/// Returns true if the storage is linked, false if allocated.
inline bool memblock::is_linked (void) const
{
    return (!m_AllocatedSize && cdata());
}

/// Unlinks object.
inline void memblock::unlink (void)
{
    memlink::unlink();
    m_AllocatedSize = 0;
}

/// Swaps the contents with \p l
inline void memblock::swap (memblock& l)
{
    memlink::swap (l);
    ::ustl::swap (m_AllocatedSize, l.m_AllocatedSize);
}

/// resizes the block to \p newSize bytes, reallocating if necessary.
inline void memblock::resize (size_t newSize, bool bExact)
{
    reserve (newSize, bExact);
    memlink::resize (newSize);
}

/// Reads object \p l from stream \p is
inline istream& operator>> (istream& is, memblock& l)
{
    l.read (is);
    return (is);
}

} // namespace ustl

#endif

