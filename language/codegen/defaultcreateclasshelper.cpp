/*  This file is part of KDevelop
    Copyright 2012 Miha Čančula <miha@noughmad.eu>

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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "defaultcreateclasshelper.h"
#include "templateclassgenerator.h"
#include "templateclassassistant.h"
#include "overridespage.h"
#include "classidentifierpage.h"

using namespace KDevelop;

class KDevelop::DefaultCreateClassHelperPrivate
{
public:
    TemplateClassAssistant* assistant;
};

DefaultCreateClassHelper::DefaultCreateClassHelper(TemplateClassAssistant* assistant)
: d(new DefaultCreateClassHelperPrivate)
{
    d->assistant = assistant;
}

DefaultCreateClassHelper::~DefaultCreateClassHelper()
{
    delete d;
}

KDevelop::TemplateClassGenerator* DefaultCreateClassHelper::generator()
{
    return new TemplateClassGenerator(d->assistant->baseUrl());
}

QList< DeclarationPointer > DefaultCreateClassHelper::defaultMethods(const QString& name) const
{
    Q_UNUSED(name);
    return QList<DeclarationPointer>();
}
