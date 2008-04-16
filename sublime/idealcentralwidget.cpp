/*
  Copyright 2008 Hamish Rodda <rodda@kde.org>

  Permission to use, copy, modify, distribute, and sell this software and its
  documentation for any purpose is hereby granted without fee, provided that
  the above copyright notice appear in all copies and that both that
  copyright notice and this permission notice appear in supporting
  documentation.

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  KDEVELOP TEAM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "idealcentralwidget.h"

#include "idealcentrallayout.h"
#include "ideal.h"
#include "ideallayout.h"

#include <sublime/mainwindow.h>

using namespace Sublime;

IdealCentralWidget::IdealCentralWidget(IdealMainWidget * parent)
    : QWidget(parent)
{
    setLayout(new IdealCentralLayout(parent->mainWindow(), this));
}

Area * IdealCentralWidget::currentArea() const
{
    return centralLayout()->currentArea();
}

void IdealCentralWidget::setArea(Area * area)
{
    if (centralLayout()->currentArea())
        centralLayout()->currentArea()->disconnect(this);

    centralLayout()->setArea(area);

    connect(area, SIGNAL(viewAdded(Sublime::AreaIndex*, Sublime::View*)), this, SLOT(viewAdded(Sublime::AreaIndex*, Sublime::View*)));
    connect(area, SIGNAL(aboutToRemoveView(Sublime::AreaIndex*, Sublime::View*)), this, SLOT(aboutToRemoveView(Sublime::AreaIndex*, Sublime::View*)));
}

IdealCentralLayout* IdealCentralWidget::centralLayout() const
{
    return static_cast<IdealCentralLayout*>(layout());
}

void IdealCentralWidget::viewAdded(Sublime::AreaIndex * index, Sublime::View * view)
{
    Q_UNUSED(view);
    centralLayout()->refreshAreaIndex(index);
}
#include <kdebug.h>
void IdealCentralWidget::aboutToRemoveView(Sublime::AreaIndex* index, Sublime::View* view)
{
    bool wasActive = m_mainWidget->mainWindow()->activeView() == view;
    int formerIdx = index->views().indexOf(view);
    Sublime::AreaIndex *formerParent = index->parent();

    centralLayout()->removeView(index, view);
    kDebug() << "ABOUT TO REMOVE";

    if (wasActive)
    {
        //look for the remaining views in the same index
        if (index->views().count() > 0)
        {
            int idx = formerIdx;
            //look left
            if (idx-1 > 0)
                idx -= 1;
            //else look right (stay at the same index)
            kDebug() << "ACTIVATING NEW VIEW" << index->views()[idx];
            m_mainWidget->mainWindow()->activateView(index->views()[idx]);
        }
        else
        {
            //look up
            //@fixme 
//             formerParent->
        }
    }
}

#include "idealcentralwidget.moc"
