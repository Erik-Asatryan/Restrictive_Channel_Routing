#include "MainWindow.h"

#include "ToolBar.h"

#include <QWidget>

using namespace Base;

MainWindow::MainWindow(QWidget* pParent /* = nullptr */) :
	QMainWindow(pParent)
{
}

void MainWindow::addToolBar(ToolBar* pToolBar)
{
	QMainWindow::addToolBar(pToolBar);
	m_pToolBar = pToolBar;
}

void MainWindow::addToolBar(Qt::ToolBarArea area, ToolBar* pToolBar)
{
	QMainWindow::addToolBar(area, pToolBar);
	m_pToolBar = pToolBar;
}