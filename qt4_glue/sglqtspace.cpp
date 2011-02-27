//
// C++ Implementation: sglqtspace
//
// Description:
//
//
// Author: Enrico Reimer,,, <enni@Akira>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sglqtspace.h"
#include <QLayout>
#include <QWidget>
#include <time.h>

#include <QApplication>
#include <qregexp.h>
#include <QMouseEvent>
#include <QFrame>

/**
 * Konstruktor zur Mitverwendeung eines schon initailisierten Renderers.
 * Erzeugt ein QGLWidget in dem mit parent angegeben Kontainer.
 * @param shareWidget das SGLqtSpace-Objekt dessen Renderer verwendet werden soll.
 * @param parent der Kontainer, in dem angezeigt werden soll
 * @param name der Name der View (wird im GL-Fenster angezeigt wenn renderName true ist)
 * @param killOnClose wenn true, löscht sich das Objekt selbst, wenn sein Kontainer geschlossen wird.
 */
SGLqtSpace::SGLqtSpace(const SGLqtSpace *shareWidget,QWidget *parent, const char *name,bool killOnClose):
    QGLWidget(parent, shareWidget),SGLSpace(0,0,102,102,204)
{
    setObjectName(name);
    setAttribute(Qt::WA_DeleteOnClose,killOnClose);
	this->shareWidget=shareWidget;
	cloned=true;
	commonConstr(parent);
}
/**
 * Konstruktor zur Verwendeung (und Initialisierung) eines neuen Renderers.
 * Erzeugt ein QGLWidget in dem mit parent angegeben Kontainer.
 * @param parent der Kontainer, in dem angezeigt werden soll
 * @param name der Name der View (wird im GL-Fenster angezeigt wenn renderName true ist)
 */
SGLqtSpace::SGLqtSpace(QWidget *parent, const char *name):QGLWidget(parent),SGLSpace(0,0,102,102,204)
{
    setObjectName(name);
    shareWidget=NULL;
    commonConstr(parent);
}


/**
 * Destruktor.
 * Sendet für alle eigenen Objekte sigUnshowObj().
 * Dies bewirkt, daß seine Objekte gegebenenfalls aus allen Views, die sie mit ihm teilten entfernt werden.
 */
SGLqtSpace::~SGLqtSpace()
{
	for(list<shared_obj>::iterator i=ObjLst.Objects.begin();i!=ObjLst.Objects.end();i++)
		if((*i)->shared && (*i)->myList==&ObjLst)
			sigUnshowObj(*i);
	for(list<shared_obj>::iterator i=TranspObjLst.Objects.begin();i!=TranspObjLst.Objects.end();i++)
		if((*i)->shared && (*i)->myList==&TranspObjLst)
			sigUnshowObj(*i);
}

/**
 * Wird vom Framework beim Erzeugen der view aufgerufen.
 * Setzt die view auf und initialisiert einige Werte.
 * Es wird jedoch nicht gezeichnet.
 */
void SGLqtSpace::initializeGL()
{
  PixelMMSize.x=(double)this->widthMM() / (double)this->width();
  PixelMMSize.y=(double)this->heightMM() / (double)this->height();
	if(context()) // es wird schon hier ein glContext beötigt - geht das
	{
		makeCurrent();
		printf("OpenGL-Kontext initialisiert\n");
	}
	else
	{
		printf("OpenGL konnte nicht initialisiert werden");
		exit(1);
	}
  sglInit(this->width(),this->height());
	CompileIntObs();
	ObjLst.Compile(true);
	TranspObjLst.Compile(true);
//	redraw();
}

/**
 * Wird vom Framework aufgerufen, wenn sich die Größe des Kontainers ändert.
 * @param w die neue Breite der View
 * @param h die neue Höhe der View
 */
void SGLqtSpace::resizeGL( int w, int h )
{
	OnResize(w,h);
	redrawOther(this);
}

/**
 * Wird von QT aufgerufen um die View neu zu zeichnen.
 * Die Prozedur löst erst draw aus und schreibt gegebenenfalls den Namen der View in die View.
 */
void SGLqtSpace::paintGL()
{
	draw();
	if(renderName)
	{
		const QFontMetrics font(QApplication::font(this));
		const QSize s=size();
    const QString n(windowTitle());
		renderText(s.width()/2-font.width(n)/2,font.height(),n);
	}
}

void SGLqtSpace::callUpdate()
{
	updateGL();
}

bool SGLqtSpace::setup_video(int w,int h)
{
	StatusInfo.WindowHeight=h;
	StatusInfo.WindowWidth=w;
	return StatusInfo.glServerReady=isValid();
}

void SGLqtSpace::mousePressEvent ( QMouseEvent * e )
{
	MouseInfo.MovedPastDownBtn=false;
	mouseBtnDown(e);
	MouseInfo.DownBtns=e->button();
	QGLWidget::mousePressEvent (e);
}
void SGLqtSpace::mouseReleaseEvent ( QMouseEvent * e )
{
	if(!MouseInfo.MovedPastDownBtn)onMouseClick(e);
	mouseBtnUp(e,MouseInfo.MovedPastDownBtn);
	MouseInfo.DownBtns=e->button();
	QGLWidget::mouseReleaseEvent(e);
}
void SGLqtSpace::mouseDoubleClickEvent ( QMouseEvent * e )
{
	onMouseDblClick(e);
}

/**
 * Wird von QT ausgelöst, wenn die Maus innerhalb der View bewegt wird.
 * Das Ereigniss wird ignoriert, wenn gerade gezeichnet wird oder wenn MouseInfo.FollowMouse nicht true ist.
 * Ist in MouseInfo.MouseMode das SGL_FOLLOW_MOUSE_FLOAT-Flag gesetzt, werden die Signale pressedMouseMoveAbs/Rel bzw. releasedMouseMoveAbs/Rel.
 * Diesen Signalen werden in diesem Fall die relativen View-Koordinaten übergeben. Die linke obere Ecke hat dabei die Koordinaten (-1,-1)
 * Ist SGL_FOLLOW_MOUSE_REAL gesetzt, werden die Signale releasedMouseMoveAbs und pressedMouseMoveAbs ausgelöst.
 * Allen Signalen werden die Informationen über das QT-Event als erster Parameter übergeben.
 * Diese Prozedur löst redraw() aus.
 * @param e Informationen über das Ereigniss
 */
void SGLqtSpace::mouseMoveEvent ( QMouseEvent * e )
{
	if(!MouseInfo.FollowMouse || StatusInfo.Processing)return;
	MouseInfo.MovedPastDownBtn=true;
	if(MouseInfo.MouseMode && StatusInfo.WindowWidth && StatusInfo.WindowHeight)
	{
		if(MouseInfo.MouseMode & SGL_FOLLOW_MOUSE_FLOAT)
		{
			double RelXPos=(e->x()-double(StatusInfo.WindowWidth)/2)/double(StatusInfo.WindowWidth)*2;
			double RelYPos=-(e->y()-double(StatusInfo.WindowHeight)/2)/double(StatusInfo.WindowHeight)*2;

			if(e->buttons()&(Qt::LeftButton|Qt::RightButton|Qt::MidButton))
			{
				pressedMouseMoveAbs(e,RelXPos,RelYPos);
				pressedMouseMoveRel(e,RelXPos-MouseInfo.OldX,MouseInfo.OldY-RelYPos);
			}
			else
			{
				releasedMouseMoveAbs(e,RelXPos,RelYPos);
				releasedMouseMoveRel(e,RelXPos-MouseInfo.OldX,MouseInfo.OldY-RelYPos);
			}
			MouseInfo.OldX=RelXPos;
			MouseInfo.OldY=RelYPos;
			if(StatusInfo.StatusString)
				sprintf(StatusInfo.StatusString,"%sRelXPos: %.3f, RelYPos: %.3f\n",StatusInfo.StatusString,RelXPos,RelYPos);
		}
		if(MouseInfo.MouseMode & SGL_FOLLOW_MOUSE_REAL)
		{
			if(e->buttons()&(Qt::LeftButton|Qt::RightButton|Qt::MidButton))pressedMouseMoveAbs(e);
			else releasedMouseMoveAbs(e);
		}
		if(StatusInfo.StatusString)
			sprintf(StatusInfo.StatusString,"%sDownBtns: %d\n",StatusInfo.StatusString,MouseInfo.DownBtns);
		redraw();
		e->accept();
	}
	else
	{
		e->ignore();
		QGLWidget::mouseMoveEvent(e);
	}
}

/**
 * Behandlung von Mausrad-Ereignissen.
 * Diese Prozedur löst redraw() aus.
 * @param e Informationen über das Ereigniss
 */
void SGLqtSpace::wheelEvent ( QWheelEvent * e )
{
	if(e->buttons()==Qt::NoButton)
	{
		e->accept();
		if(e->delta()>0)
		{
			Camera->MoveZoom(0.95);
			sprintf(StatusInfo.StatusString,"%sEntfernung zum Ziel verringert auf %.3f\n",StatusInfo.StatusString,Camera->ViewLength());
			camChanged();
		}
		else if(e->delta()<0)
		{
			Camera->MoveZoom(1.05);
			sprintf(StatusInfo.StatusString,"%sEntfernung zum Ziel erhöht auf %.3f\n",StatusInfo.StatusString,Camera->ViewLength());
			camChanged();
		};
		mouseWheel(e);
		redraw();
	}else e->ignore();

}

/**
 * Standard-Tastenbehandlung.
 * Diese Prozedur löst redraw() aus.
 * @param e Informationen über das Ereigniss
 */
void SGLqtSpace::keyPressEvent(QKeyEvent * e )
{
	if(!keyIgnore[e->key()])
	{
		switch (e->key())
		{
		case Qt::Key_Left:Camera->RotateCam(-2,0);camChanged();break;
		case Qt::Key_Right:Camera->RotateCam(2,0);camChanged();break;
		case Qt::Key_Up:Camera->RotateCam(0,2);camChanged();break;
		case Qt::Key_Down:Camera->RotateCam(0,-2);camChanged();break;
		case Qt::Key_PageUp:Camera->MoveZoom(0.95);break;
		case Qt::Key_PageDown:Camera->MoveZoom(1.05);break;
		case Qt::Key_Home:Camera->OptZoom(1.05);camChanged();break;
		case Qt::Key_End:Camera->OptZoom(0.95);camChanged();break;
		case Qt::Key_Insert:Camera->Roll(-1);camChanged();break;
		case Qt::Key_F1:Grids.doGrid^=1;break;
		case Qt::Key_F2:Grids.doGrid^=2;break;
		case Qt::Key_F3:Grids.doGrid^=4;break;
		case Qt::Key_Return:Camera->ResetView();camChanged();break;//Enter
		case Qt::Key_Delete:Camera->Roll(1);camChanged();break;//Entf
		case Qt::Key_Escape:close();break;//ESC
		default:
			StatusInfo.StatusString[0]=0;
			//@todo Wenn das Event mehrfach aufgerufen wird, ohne das show_status aufgerufen wurde, wird das ganze immer länger. Und läuft irgendwann über
			//Diese Lösung ist aber auch nicht so toll, denn damit geht alles was vorher hier stand verloren
			sprintf(StatusInfo.StatusString,"%sF1-F3 => Koordinatensysteme umschalten\n",StatusInfo.StatusString);
			sprintf(StatusInfo.StatusString,"%slinke Maus / Cursor => Kamera rotieren (um das Ziel)\n",StatusInfo.StatusString);
			sprintf(StatusInfo.StatusString,"%srechte Maus => Ziel rotieren (um die Kamera)\n",StatusInfo.StatusString);
			sprintf(StatusInfo.StatusString,"%srechte Maus Click => Kameralicht ein/aus\n",StatusInfo.StatusString);
			sprintf(StatusInfo.StatusString,"%sMausrad / BildAuf|BildAb => Phys. Zoom (Kamera auf Ziel zubew.)\n",StatusInfo.StatusString);
			sprintf(StatusInfo.StatusString,"%sHome|End => Opt. Zoom (Brennweite ï¿½dern)\n",StatusInfo.StatusString);
			sprintf(StatusInfo.StatusString,"%sIns|Del => Kamera rollen (auf Sichtachse rotieren)\n",StatusInfo.StatusString);
			sprintf(StatusInfo.StatusString,"%sEnter => Kamera zurck auf Ausgangspos.\n",StatusInfo.StatusString);
			sprintf(StatusInfo.StatusString,"%sESC => beenden\n",StatusInfo.StatusString);
		break;
		}
		redraw();
	}
	QGLWidget::keyPressEvent(e);
}

void SGLqtSpace::onMouseDblClick(QMouseEvent *e){}

/**
 * Standard-Bahndlung für Mausklicks.
 * Diese Prozedur löst redraw() aus.
 * @param e Informationen über das Ereigniss
 */
void SGLqtSpace::onMouseClick(QMouseEvent *e)
{
	switch(e->button())
	{
	case Qt::RightButton:StdLight->On(!StdLight->IsOn());break;
	default:e->ignore();break;
	}
	redraw();
}

/**
 * Allgemeines Objekt-Setup.
 * Wird von allen Konstruktoren aufgerufen. Führt für allgemeine Operationen aus.
 * @param parent der Kontainer in dem die View erzeugt wird
 */
void SGLqtSpace::commonConstr(QWidget *parent)
{
	mainConsole=NULL;
	renderName=false;
	StatusInfo.FullScreen=false;
	mouse_real_listener=mouse_float_listener=0;
	setMouseTracking(true);
	if(parent)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored));
		if(parent->layout())
		{
// 			QBoxLayout *bl=dynamic_cast<QBoxLayout*>(parent->layout());
//			QGridLayout *gl=dynamic_cast<QBoxLayout>(parent->layout());
/*			if(bl)
			{
				if(!bl->findChild(this))bl->add(this);
			}
			else*/
			{
				SGLprintWarning("Konnte nicht prüfen, ob dieser Space schon in diesem Layout ist.  Füge ihn hinzu.");
				parent->layout()->addWidget(this);
			}
		}
		else
			(new QVBoxLayout(parent))->addWidget(this);
	}
  setFocusPolicy(Qt::WheelFocus);
	connect(this,SIGNAL(redraw()),SLOT(updateGL()));
	connect(this,SIGNAL(pressedMouseMoveAbs(QMouseEvent *,float ,float)),SLOT(defaultHandler_pressedMouseMoveAbs(QMouseEvent *,float ,float)));
	connect(this,SIGNAL(releasedMouseMoveAbs(QMouseEvent *)),SLOT(defaultHandler_releasedMouseMoveAbs(QMouseEvent *)));
}



/**
 * Schaltet die Standardbehandlung für die Mausbewegungen ab.
 * defaultHandler_pressedMouseMoveAbs() und defaultHandler_releasedMouseMoveAbs werden daher nicht mehr automatisch aufgerufen.
 */
void SGLqtSpace::disableDefaultHandler()
{
	disconnect(SIGNAL(pressedMouseMoveAbs(QMouseEvent *,float ,float)),this,SLOT(defaultHandler_pressedMouseMoveAbs(QMouseEvent *,float ,float)));
	disconnect(SIGNAL(releasedMouseMoveAbs(QMouseEvent *)),this,SLOT(defaultHandler_releasedMouseMoveAbs(QMouseEvent *)));
}


/**
 * Manuelles Auslösen von redraw().
 */
void SGLqtSpace::sendRedraw(){redraw();}
/**
 * Manuelles Auslösen von redrawOther(this);
 */
void SGLqtSpace::sendRedrawOther(){redrawOther(this);}
/**
 * Manuelles Auslösen von sigShowObj(obj);
 * @param obj das Objekt das gezeigt werden soll.
 */
void SGLqtSpace::sendShowObj(shared_obj obj){sigShowObj(obj);}
/**
 * Manuelles Auslösen von sigUnshowObj(obj);
 * @param obj das Objekt das nicht mehr gezeigt werden soll.
 */
void SGLqtSpace::sendUnshowObj(shared_obj obj){sigUnshowObj(obj);}

/**
 * Slot zur Behandlung von sigShow()-Signalen.
 * Es wird automatisch bestimmt, um welche Objektart es sich handelt und die entsprechende SGLSpace::registerObj() aufgerufen.
 * Danach wird diese View neu gezeichnet.
 * Gehört das Objekt noch keinem Space-Objekt, wenn es also in noch keiner Objekliste steht, wird es in die entsprechenden Objektlisten dieses Space eingetragen.
 * @param obj das Objekt das gezeigt werden soll.
 */
void SGLqtSpace::showObj(const shared_obj &obj)
{
	SGLshPtr<SGLObj> sglobj;
	SGLshPtr<SGLFlObj> flobj;

	if(obj.dcast(flobj,false))registerObj(flobj);
	else if(obj.dcast(sglobj,false))registerObj(sglobj);
	else {SGLprintError("Konnte das %s-Objekt keinem Standarttyp zuordnen",obj->guesType());}
	updateGL();
}
/**
 * Slot zur Behandlung von sigUnshow()-Signalen.
 * Es wird automatisch bestimmt, um welche Objektart es sich handelt und die entsprechende SGLSpace::unregisterObj() aufgerufen.
 * Wird das Objekt danach nicht mehr benötigt, wird es automatisch gelöscht.
 * Außerdem diese View neu am Ende dieser Operation neu gezeichnet.
 * @param obj das Objekt das gezeigt werden soll.
 */
void SGLqtSpace::unshowObj(const shared_obj &obj)
{
	SGLshPtr<SGLObj> sglobj;
	SGLshPtr<SGLFlObj> flobj;
	if(obj.dcast(flobj,false))unregisterObj(flobj);
	else if(obj.dcast(sglobj,false))unregisterObj(sglobj);
	else {SGLprintError("Konnte das %s-Objekt keinem Standarttyp zuordnen",obj->guesType());}
	updateGL();
}


/**
 * Zeigt alle später mittels sigShowObj() angezeigten Objekte zusätzlich in dem angegeben Space ein.
 * Die Signale sigShowObj(), sigUnshowObj() und redrawOther()/redraw() werden mit den Slots showObj, unshowObj bzw. updateGL verbunden.
 * Die Spaces müssen sich einen Rendering-Context Teilen.
 * @param sp der SGLqtSpace in dem die eigenen Objekte sichtbar sein sollen.
 */
void SGLqtSpace::showObjectsIn(const SGLqtSpace *sp)
{
	sp->connect(this,SIGNAL(sigShowObj(const shared_obj&)),SLOT(showObj(const shared_obj&)));
	sp->connect(this,SIGNAL(sigUnshowObj(const shared_obj&)),SLOT(unshowObj(const shared_obj&)));
	sp->connect(this,SIGNAL(redraw()),SLOT(updateGL()));
	sp->connect(this,SIGNAL(redrawOther(SGLqtSpace *)),SLOT(updateGL()));//Damit das redrawOther vom init schonverarbeitet werden kann vor "show"
}

/**
 * Wird ausgelöst, wenn die View diese Spaces den Eingabefocus bekommt.
 * @param e Ereignissinformationen
 */
void SGLqtSpace::focusInEvent(QFocusEvent *e)
{
	QFrame* frame=dynamic_cast<QFrame*>(parentWidget());
	if(frame)
		frame->setFrameStyle(QFrame::Box|QFrame::Sunken);
	gotFocus(e->reason());
	QGLWidget::focusInEvent(e);
}
/**
 * Wird ausgelöst, wenn die View diese Spaces den Eingabefocus verliert.
 * @param e Ereignissinformationen
 */
void SGLqtSpace::focusOutEvent(QFocusEvent *e)
{
	QFrame* frame=dynamic_cast<QFrame*>(parentWidget());
	if(frame)
		frame->setFrameStyle(QFrame::Box|QFrame::Raised);
	lostFocus(e->reason());
	QGLWidget::focusOutEvent(e);
}

void SGLqtSpace::benchmark()
{
	Camera->RotateCamAround(1,1,Camera->LookAt);
	if(benchTime!=time(NULL))
	{
		cout << benchCnt << endl;
		time(&benchTime);
		benchCnt=1;
	}
	else benchCnt++;
	updateGL();
}

void SGLqtSpace::defaultHandler_releasedMouseMoveAbs(QMouseEvent * e)
{
	if(Camera)//@todo das hat hier eigentlich nix zu suchen
		mouseMoved(e,Camera->screen2welt(e->x(),e->y()));//@todo ist vielleicht nicht so clever das auf Verdacht zu machen - kostet ja doch einiges an rechenzeit
}

void SGLqtSpace::defaultHandler_pressedMouseMoveAbs(QMouseEvent * e,float XPos,float YPos)
{
		if(e->buttons()&Qt::LeftButton)
		{
			RotateCam(XPos,YPos,*Camera);
			camChanged();
		}
		else if(e->buttons()&Qt::RightButton)
		{
			/*if(e->buttons()&Qt::ShiftButton)RotateAim(XPos,YPos,*Camera);
			else*/ MoveAim(XPos,YPos,*Camera);
			camChanged();
		}
}

void SGLqtSpace::connectNotify( const char * signal )
{
	QRegExp moveFloat("^[0-9]\\w+MouseMove\\w+\\([^,]+,[\\s]*float[^,]*,[\\s]*float[^)]*\\)$");
	QRegExp moveReal("^[0-9]\\w+MouseMove\\w+\\([^),]*\\)$");

	if(moveFloat.exactMatch(signal))
	{
		mouse_float_listener++;
		MouseInfo.MouseMode|=SGL_FOLLOW_MOUSE_FLOAT;
	}
	if(moveReal.exactMatch(signal))
	{
		mouse_real_listener++;
		MouseInfo.MouseMode|=SGL_FOLLOW_MOUSE_REAL;
	}
}

void SGLqtSpace::disconnectNotify( const char * signal )
{
	QRegExp moveFloat("^[0-9]\\w+MouseMove\\w+\\([^,]+,[\\s]*float[^,]*,[\\s]*float[^)]*\\)$");
	QRegExp moveReal("^[0-9]\\w+MouseMove\\w+\\([^),]*\\)$");

	if(moveFloat.exactMatch(signal))
	{
		if(mouse_float_listener>0)mouse_float_listener--;
		if(!mouse_float_listener)MouseInfo.MouseMode&=~SGL_FOLLOW_MOUSE_FLOAT;
	}
	else if(moveReal.exactMatch(signal))
	{
		if(mouse_real_listener>0)mouse_real_listener--;
		if(!mouse_real_listener)MouseInfo.MouseMode&=~SGL_FOLLOW_MOUSE_REAL;
	}
	setMouseTracking(MouseInfo.MouseMode);
}
