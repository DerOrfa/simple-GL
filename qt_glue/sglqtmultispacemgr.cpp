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
void SGLqtMultiSpaceMgr::registerSpace(SGLqtSpace *sw)
{
	connect(sw,SIGNAL(destroyed(QObject *)),SLOT(lostWidget(QObject *)));
	for(QLinkedList<SGLqtSpace *>::iterator it=childs.begin();it!=childs.end();it++)
	{
		SGLqtSpace *i= *it;
		sw->showObjectsIn(i);
		i->showObjectsIn(sw);
	}
	childs.push_back(sw);
	qDebug("Registered %s(0x%p)",sw->objectName().toStdString().c_str(),sw);
}

void SGLqtMultiSpaceMgr::lostWidget(QObject* obj)
{
	mutex.lock();
	childs.removeAll(reinterpret_cast<SGLqtSpace*>(obj)); // force type, as we dont really have it anymore, but doesn't matter 'for removing the pointer anyway
	mutex.unlock();
	qDebug("Unregistered %s(0x%p)",obj->objectName().toStdString().c_str(),obj);
}

/*!
    \fn SGLqtMultiSpaceMgr::closeAllChilds(SGLqtSpace *exception=NULL)
 */
void SGLqtMultiSpaceMgr::removeAllChilds(SGLqtSpace *exception)
{
	mutex.lock();
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
	mutex.unlock();
}

SGLqtSpace* SGLqtMultiSpaceMgr::createSharedSpace(QWidget *parent)
{
	mutex.lock();
	assert(parent);
	QGLWidget *shared=childs.empty() ? NULL:childs.front();
	SGLqtSpace* ret=new SGLqtSpace(shared,parent);
	ret->setObjectName(QString("shared SGLqtSpace in ")+parent->objectName());
	registerSpace(ret);
	mutex.unlock();
	return ret;
}
