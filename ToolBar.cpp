#include "ToolBar.h"

#include <QList>

using namespace Base;

ToolBar::ToolBar(QWidget* pParent) :
	QToolBar(pParent)
{
	m_spActions = new ActionList();
}
