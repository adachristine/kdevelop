/* This file is part of KDevelop
    Copyright (C) 2003 Roberto Raggi <roberto@kdevelop.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "tag.h"
#include <qdatastream.h>

Tag::Tag()
{
}

Tag::Tag( const Tag& source )
    : m_attributes( source.m_attributes )
{
}

Tag::~Tag()
{
}

Tag& Tag::operator = ( const Tag& source )
{
    m_attributes = source.m_attributes;
    return( *this );
}

void Tag::load( QDataStream& stream )
{
    stream >> m_attributes;
}

void Tag::store( QDataStream& stream ) const
{
    stream << m_attributes;
}
