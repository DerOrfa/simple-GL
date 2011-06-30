
#include <stdio.h>
#include <stdlib.h>

#include <qt4_glue/sglqtspace.h>
#include <text/sgl3dtext.h>
#include <text/sglconsole.h>
#include <helper/sgllensflare.h>

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc,argv);
	SGLprintState("Initialisiere Schnittstelle ...");
	SGLqtSpace *w= new SGLqtSpace(NULL, "sample1");
	;

	SGLshPtr<SGLQuader> lookat1(new SGLQuader),lookat2(new SGLQuader);
	lookat1->SetPosAndScale(-2,0,0,1);
	lookat2->SetPosAndScale(2,0,0,1);

	// let the "Quader"s face the "target" of the camera
	lookat1->FaceAt=&w->Camera->LookAt; 
	lookat2->FaceAt=&w->Camera->LookAt;

	// update the "Quader"s if the camera is changed
	w->Camera->link(*lookat1);
	w->Camera->link(*lookat2);


	w->registerObj(SGLshPtr_new<SGLQuader>()); // create a fixed default-"Quader"
	w->registerObj(lookat1);
	w->registerObj(lookat2);
	w->show();

	SGLprintState("fertsch");
	a.connect( &a, SIGNAL(lastWindowClosed()), SLOT(quit()) );
	return a.exec();

}
