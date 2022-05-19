#pragma once
#include <QToolBar>

namespace Base
{
	class ToolBar : public QToolBar
	{
	public:
		using ActionList = QList< QAction*>;

		ToolBar(QWidget* pParent = nullptr);


	private:
		ActionList* m_spActions;
	};
} // end namespace Base