#include <QApplication>

#include "MainWindow.h"
#include "CanvasMainWindow.h"
#include "ToolBar.h"

using namespace Base;

int main(int argc, char** argv)
{
	QApplication pApp(argc, argv);

	MainWindow* pMainWindow = new MainWindow();

	CanvasMainWindow* pCanvasMainWindow = new CanvasMainWindow(pMainWindow);
		
	pMainWindow->setCentralWidget(pCanvasMainWindow);
	pMainWindow->show();
	pMainWindow->showMaximized();
	
	pApp.exec();
}