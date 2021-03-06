/* This file is part of KDevelop

   Copyright 2018 Anton Anikin <anton@anikin.xyz>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KDEVCLAZY_CHECKS_WIDGET_H
#define KDEVCLAZY_CHECKS_WIDGET_H

#include <QWidget>
#include <QHash>

class QTreeWidget;
class QTreeWidgetItem;

namespace Clazy
{

namespace Ui { class ChecksWidget; }

class ChecksDB;

class ChecksWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(
        QString checks
        READ checks
        WRITE setChecks
        NOTIFY checksChanged
        USER true)

public:
    explicit ChecksWidget(QWidget* parent = nullptr);
    ~ChecksWidget() override;

public:
    void setChecksDb(const QSharedPointer<const ChecksDB>& db);
    QString checks() const;

    void setChecks(const QString& checks);

    void setEditable(bool editable);

Q_SIGNALS:
    void checksChanged(const QString& checks);

private:
    void updateChecks();
    void setState(QTreeWidgetItem* item, Qt::CheckState state, bool force = true);
    void searchUpdated(const QString& searchString);

private:
    QScopedPointer<Ui::ChecksWidget> m_ui;

    QString m_checks;
    QHash<QString, QTreeWidgetItem*> m_items;
    bool m_isEditable = true;
};

}

#endif
