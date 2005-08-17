/* This file is part of KDevelop
    Copyright (C) 2004 Roberto Raggi <roberto@kdevelop.org>

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
#ifndef KDEVPROJECTBUILDER_H
#define KDEVPROJECTBUILDER_H

#include "kdevprojectmodel.h"

#include <qobject.h>

class KDevProject;

/**
@author Roberto Raggi

@short KDevProjectBuilder Base class for the Project Builders

Describes a <b>Project Builder</b> to the KDevelop's Project Manager.
*/
class KDevProjectBuilder: public QObject
{
    Q_OBJECT
public:
    KDevProjectBuilder(QObject *parent = 0);
    virtual ~KDevProjectBuilder();

    virtual KDevProject *project() const = 0;

    virtual bool isExecutable(KDevItem *dom) const = 0;

    virtual KDevItem *defaultExecutable() const = 0;
    virtual void setDefaultExecutable(KDevItem *dom) = 0;

    virtual bool configure(KDevItem *dom) = 0;
    virtual bool build(KDevItem *dom) = 0;
    virtual bool clean(KDevItem *dom) = 0;
    virtual bool execute(KDevItem *dom) = 0;

signals:
    void builded(KDevItem *dom);
    void failed();
};

#endif
