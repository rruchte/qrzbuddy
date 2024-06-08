#include <QClipboard>
#include <QGuiApplication>
#include <QKeyEvent>

#include "detailtableview.h"

DetailTableView::DetailTableView(QWidget *parent) : QTableView(parent)
{

}

void DetailTableView::keyPressEvent(QKeyEvent *event)
{
	QModelIndexList selectedRows = selectionModel()->selectedRows();

	// if at least one cell selected
	if (!selectedIndexes().isEmpty())
	{
		if (event->matches(QKeySequence::Copy))
		{
			QString text;

			QStringList headerContents;

			QItemSelectionRange range = selectionModel()->selection().first();

			for (auto i = range.top(); i <= range.bottom(); ++i)
			{
				QStringList rowContents;
				for (auto j = range.left(); j <= range.right(); ++j)
				{
					rowContents << model()->index(i, j).data().toString();
				}
				text += rowContents.join("\t");
				text += "\n";
			}
			QGuiApplication::clipboard()->setText(text);
		}
		else
		{
			QTableView::keyPressEvent(event);
		}
	}
}
