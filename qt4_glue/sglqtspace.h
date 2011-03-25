//
// C++ Interface: sglqtspace
//
// Description:
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SGLQTSPACE_H
#define SGLQTSPACE_H

#include "../sglspace.h"
#include <QtOpenGL>

/**
@author Enrico Reimer,,,
*/
class SGLqtSpace : public QGLWidget , public SGLSpace
{
Q_OBJECT
	short mouse_real_listener,mouse_float_listener;
public:
	SGLqtSpace(const QGLWidget * shareWidget,QWidget *parent=NULL, const char *name=NULL,bool killOnClose=true);
	SGLqtSpace(QWidget *parent=NULL, const char *name=NULL);
	~SGLqtSpace();
	void initializeGL();
	void resizeGL( int w, int h );
	void paintGL();
	virtual bool setup_video(int w,int h);
	virtual void onMouseClick(QMouseEvent *e);
	virtual void onMouseDblClick(QMouseEvent *e);
	void sendRedraw();
	void sendRedrawOther();
	void sendShowObj(shared_obj obj);
	void sendUnshowObj(shared_obj obj);
	void showObjectsIn(const SGLqtSpace *sp);
	unsigned int benchCnt;
	virtual void callUpdate();
	time_t benchTime;
	bool renderName;
	void disableDefaultHandler();
	std::map<int,bool> keyIgnore;

public Q_SLOTS:
	void showObj(const shared_obj &obj);
	void unshowObj(const shared_obj &obj);
	void benchmark();
	virtual void defaultHandler_pressedMouseMoveAbs(QMouseEvent * e,float relMoveX,float relMoveY);
	virtual void defaultHandler_releasedMouseMoveAbs(QMouseEvent * e);

protected:
	virtual void mousePressEvent ( QMouseEvent * e );
	virtual void mouseReleaseEvent ( QMouseEvent * e );
	virtual void mouseMoveEvent ( QMouseEvent * e );
	virtual void mouseDoubleClickEvent ( QMouseEvent * e );
	virtual void wheelEvent ( QWheelEvent * e );
	virtual void keyPressEvent ( QKeyEvent * e );
	virtual void focusInEvent(QFocusEvent*e);
	virtual void focusOutEvent(QFocusEvent*e);
	struct {double x,y;}PixelMMSize;
	void connectNotify( const char * signal );
	void disconnectNotify( const char * signal );

Q_SIGNALS:
	void sigShowObj(const shared_obj &obj);
	void sigUnshowObj(const shared_obj &obj);
	void redraw();
	void redrawOther(SGLqtSpace *exception);
	void mouseBtnUp(QMouseEvent * e,bool moved);
	void mouseBtnDown(QMouseEvent * e );
	void mouseWheel(QWheelEvent * e );
	void pressedMouseMoveRel(QMouseEvent * e,float relMoveX,float relMoveY);
	void releasedMouseMoveRel(QMouseEvent * e,float relMoveX,float relMoveY);
	void pressedMouseMoveAbs(QMouseEvent * e,float relMoveX,float relMoveY);
	void releasedMouseMoveAbs(QMouseEvent * e,float relMoveX,float relMoveY);
	void pressedMouseMoveAbs(QMouseEvent * e);
	void releasedMouseMoveAbs(QMouseEvent * e);
	void camChanged();
	void mouseMoved(QMouseEvent *e,SGLVektor weltKoord);
private:
	void commonConstr(QWidget *parent);
};

#endif
