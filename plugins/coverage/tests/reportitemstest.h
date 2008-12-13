/* KDevelop xUnit plugin
 *    Copyright 2008 Manuel Breugelmans <mbr.nxi@gmail.com>
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


#ifndef QTEST_REPORTITEMSTEST_H_INCLUDED
#define QTEST_REPORTITEMSTEST_H_INCLUDED

#include <QtCore/QObject>
class QStandardItem;

namespace Veritas
{

class CoveredFile;
class ReportDirItem;

class ReportItemsTest : public QObject
{
Q_OBJECT
private slots:
    void init();
    void cleanup();

    void constructDoubleValueItem();
    void constructIntValueItem();

    void constructDirItem();
    void addFileToDirItem();
    void addMultipleFilesToDir();

    void constructDirData();
    void dirDataSetSloc();
    void dirDataSetInstrumented();
    void dirDataCoverage();

private:
    void addCoverageDataTo(ReportDirItem& dir, const QString& path, int sloc, int instrumented);
    
private:
    QList<QStandardItem*> m_garbage;
    QList<CoveredFile*> m_garbageFiles;

};

}

#endif // QTEST_REPORTITEMSTEST_H_INCLUDED
