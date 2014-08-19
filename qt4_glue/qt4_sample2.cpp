
#include <stdio.h>
#include <stdlib.h>

#include <qt4_glue/sglqtspace.h>
#include <text/sgl3dtext.h>
#include <text/sglconsole.h>
// #include <helper/sgllensflare.h>

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc,argv);
	SGLprintState("Initialisiere Schnittstelle ...");
	SGLqtSpace *w= new SGLqtSpace(NULL, "sample1");

	SGLVektor right(1,1,1),left(-1,1,1),origin;
	right.SetColor(255,0,0); // set the color at 1,1,1 to red
	left.SetColor(0,0,255); // set the color at -1,1,1 to blue
	origin.SetColor(0,0,0); // we have to set the other color as well (default is to use the previous color)


	w->registerObj(SGLshPtr_new<SGLStrecke>(right,origin)); // create the "Strecke" and register it
	w->registerObj(SGLshPtr_new<SGLStrecke>(left,origin)); // create the "Strecke" and register it
	w->show();

	SGLprintState("fertsch");
	a.connect( &a, SIGNAL(lastWindowClosed()), SLOT(quit()) );
	return a.exec();

}
