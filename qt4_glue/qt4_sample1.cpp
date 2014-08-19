
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
	w->show();

	SGLLight	myLight;
	myLight.Abnahme.Quadratisch=.01;

	SGLshPtr<SGLQuader> quad1(new SGLQuader);
	quad1->SetPosAndScale(0,2,0,1);
	SGLshPtr<SGLQuader> quad2(new SGLQuader(*quad1));
	quad2->SetPosAndScale(0,-2,0,1);
	SGLshPtr<SGL3DText> text(new SGL3DText("simple|GL by Enni"));

	w->registerObj(text);
	w->registerObj(quad1);
	w->registerObj(quad2);

	SGLprintState("fertsch");
	a.connect( &a, SIGNAL(lastWindowClosed()), SLOT(quit()) );
	return a.exec();

}
