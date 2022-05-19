#pragma once
#include <QMainWindow>

class QWidget;

namespace Base
{
	class ToolBar;

	class MainWindow : public QMainWindow
	{
	public:
		MainWindow(QWidget* pParent = nullptr);

		void addToolBar(ToolBar* pToolBar);
		void addToolBar(Qt::ToolBarArea area, ToolBar* pToolBar);

	protected:
		ToolBar* m_pToolBar;
	};
} // end namespace Base
