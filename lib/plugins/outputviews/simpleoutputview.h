/* KDevelop Simple OutputView
 *
 * Copyright 2006 Andreas Pakulat <apaku@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef SIMPLEOUTPUTVIEW_H
#define SIMPLEOUTPUTVIEW_H

#include "ioutputview.h"
#include "iplugin.h"

class QStringList;
class QStandardItemModel;
class KUrl;
class K3Process;
class QString;

/**
@author Andreas Pakulat
*/

class SimpleOutputView : public KDevelop::IPlugin, public KDevelop::IOutputView
{
Q_OBJECT
Q_INTERFACES( KDevelop::IOutputView )

public:
    SimpleOutputView(QObject *parent = 0, const QStringList &args = QStringList());
    virtual ~SimpleOutputView();
    void queueCommand(const KUrl& dir, const QStringList& command, const QMap<QString, QString>& env );

    void registerLogView( const QString& title );
    void appendLine( const QString& title, const QString& line );
    void appendLines( const QString& title, const QStringList& line );

Q_SIGNALS:
    void commandFinished( const QString& command );
    void commandFailed( const QString& command );
    void modelAdded( const QString&, QStandardItemModel* );

private:
    class SimpleOutputViewPrivate* const d;
    friend class SimpleOutputViewPrivate;
};

#endif // SIMPLEOUTPUTVIEW_H

// kate: space-indent on; indent-width 4; tab-width: 4; replace-tabs on; auto-insert-doxygen on
