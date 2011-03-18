/***************************************************************************
 *   Copyright (C) 2004 by Enrico Reimer, 1.01.2005,hayd,,@[e/ea],-131.-   *
 *   reimer@santiago                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "sglqtmultispacemgr.h"
#include <QLinkedList>

SGLqtMultiSpaceMgr::SGLqtMultiSpaceMgr(QObject *parent, const char *name) : QObject(parent)
{
    setObjectName(name);
}


SGLqtMultiSpaceMgr::~SGLqtMultiSpaceMgr()
{
	removeAllChilds();
}

/*!
    \fn SGLqtMultiSpaceMgr::onNewSpace(SGLqtSpace *newSpace)
 */
void SGLqtMultiSpaceMgr::onNewSpace(SGLqtSpace *sw)
{
	connect(sw,SIGNAL(destroyed(QObject *)),SLOT(lostChild(QObject *)));
	for(QLinkedList<SGLqtSpace *>::iterator it=childs.begin();it!=childs.end();it++)
	{
		SGLqtSpace *i= *it;
		sw->showObjectsIn(i);
		i->showObjectsIn(sw);
	}
	childs.push_back(sw);
}

/*!
    \fn GLvlWndMgr_Impl::lostChild(QObject * obj)
 */
void SGLqtMultiSpaceMgr::lostChild(SGLqtSpace* obj)
{
	childs.removeAll(obj);
}

/*!
    \fn SGLqtMultiSpaceMgr::closeAllChilds(SGLqtSpace *exception=NULL)
 */
void SGLqtMultiSpaceMgr::removeAllChilds(SGLqtSpace *exception)
{
	QLinkedList<SGLqtSpace *>::iterator it=childs.begin();
	while(it!=childs.end())
	{
		SGLqtSpace *i= *it;
		if(i !=exception)
		{
			(*it)->close();
			it=childs.erase(it);
		}
	}
}
