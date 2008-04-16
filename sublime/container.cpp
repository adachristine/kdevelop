/***************************************************************************
 *   Copyright 2006-2007 Alexander Dymo  <adymo@kdevelop.org>       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#include "container.h"

#include <QMap>
#include <QLayout>
#include <QStackedLayout>

#include "view.h"
#include "document.h"
#include "switcher.h"

namespace Sublime {

// struct ContainerPrivate

struct ContainerPrivate {
    Switcher *switcher;
    QStackedLayout *stack;
    QMap<QWidget*, View*> viewForWidget;
};



// class Container

Container::Container(QWidget *parent)
    :QWidget(parent), d(new ContainerPrivate())
{

    QVBoxLayout *l = new QVBoxLayout(this);
    l->setMargin(0);
    l->setSpacing(0);

    d->switcher = new Switcher(this);
    connect(d->switcher, SIGNAL(currentChanged(int)), this, SLOT(widgetActivated(int)));
    l->addWidget(d->switcher);

    d->stack = new QStackedLayout(l);
}

Container::~Container()
{
    delete d;
}

void Container::widgetActivated(int idx)
{
    if (idx < 0)
        return;
    d->stack->setCurrentIndex(idx);
    if (QWidget* widget = d->stack->widget(idx)) {
        widget->setFocus();
        if(d->viewForWidget.contains(widget))
            emit activateView(d->viewForWidget[widget]);
    }
}

void Container::addWidget(View *view)
{
    QWidget *w = view->widget();
    int idx = d->stack->addWidget(w);
    d->switcher->insertTab(idx, view->document()->title());
    d->viewForWidget[w] = view;
}

void Sublime::Container::removeWidget(QWidget *w)
{
    if (w) {
        d->switcher->removeTab(d->stack->indexOf(w));
        d->stack->removeWidget(w);
        d->viewForWidget.remove(w);
    }
}

int Container::count() const
{
    return d->stack->count();
}

QWidget *Container::widget(int index) const
{
    return d->stack->widget(index);
}

bool Container::hasWidget(QWidget *w)
{
    return d->stack->indexOf(w) != -1;
}

void Container::setCurrentWidget(QWidget *w)
{
    if (!hasWidget(w))
        return;
    d->stack->setCurrentWidget(w);
    d->switcher->setCurrentIndex(d->stack->indexOf(w));
}

QWidget *Container::currentWidget() const
{
    return d->stack->currentWidget();
}

View *Container::viewForWidget(QWidget *w) const
{
    return d->viewForWidget[w];
}

}

#include "container.moc"

