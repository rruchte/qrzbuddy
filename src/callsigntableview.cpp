#include <QClipboard>
#include <QGuiApplication>
#include <QMenu>
#include <QKeyEvent>
#include <set>


#include "callsigntableview.h"

CallsignTableView::CallsignTableView(QWidget *parent) : QTableView(parent)
{
	auto showDetailForSelection = new QAction(QString("Show Details"), this);
	connect(showDetailForSelection, &QAction::triggered, this, [this]()
	{
		this->showDetailForSelection();
	});

	auto copyRows = new QAction(QString("Copy Selected Row(s)"), this);
	connect(copyRows, &QAction::triggered, this, [this]()
	{
		this->copyRows();
	});

	auto copySelected = new QAction(QString("Copy Selected Cells"), this);
	connect(copySelected, &QAction::triggered, this, [this]()
	{
		this->copySelected();
	});

	auto clearAllAction = new QAction(QString("Clear All"), this);
	connect(clearAllAction, &QAction::triggered, this, [this]()
	{
		this->clearAll();
	});

	auto removeSelectedAction = new QAction(QString("Clear Selected"), this);
	connect(removeSelectedAction, &QAction::triggered, this, [this]()
	{
		this->clearSelected();
	});

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QTableView::customContextMenuRequested, this, [this, showDetailForSelection, copyRows, copySelected, clearAllAction, removeSelectedAction](QPoint const &point)
	{
		QMenu *menu = new QMenu(this);

		menu->addAction(showDetailForSelection);
		menu->addAction(copyRows);
		menu->addAction(copySelected);
		menu->addSeparator();
		menu->addAction(removeSelectedAction);
		menu->addAction(clearAllAction);

		menu->popup(this->mapToGlobal(point));
	});
}

void CallsignTableView::keyPressEvent(QKeyEvent *event)
{
	QModelIndexList selectedRows = selectionModel()->selectedRows();

	// if at least one cell selected
	if (!selectedIndexes().isEmpty())
	{
		if (event->key() == Qt::Key_Delete && enableDelete)
		{
			clearSelected();
		}
		else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
		{
			showDetailForSelection();
		}
		else if (event->matches(QKeySequence::Copy))
		{
			copySelected();
		}
		else if (event->matches(QKeySequence::Paste) && enablePaste)
		{
			QString text = QGuiApplication::clipboard()->text();
			QStringList rowContents = text.split("\n", Qt::SkipEmptyParts);

			QModelIndex initIndex = selectedIndexes().at(0);
			auto initRow = initIndex.row();
			auto initCol = initIndex.column();

			for (auto i = 0; i < rowContents.size(); ++i)
			{
				QStringList columnContents = rowContents.at(i).split("\t");
				for (auto j = 0; j < columnContents.size(); ++j)
				{
					model()->setData(model()->index(
							initRow + i, initCol + j), columnContents.at(j));
				}
			}
		}
		else
		{
			QTableView::keyPressEvent(event);
		}
	}
}

void CallsignTableView::clearAll()
{
	for (int i = this->model()->rowCount(); i >= 0; i--)
	{
		this->model()->removeRow(i);
	}
}

void CallsignTableView::clearSelected()
{
	std::set<int> rows;
	int floor = this->selectedIndexes().at(0).row();
	foreach (QModelIndex index, this->selectedIndexes())
	{
		floor = (index.row() < floor) ? index.row():floor;
		rows.emplace(index.row());
	}

	model()->removeRows(floor, rows.size());
}

void CallsignTableView::copySelected()
{
	QString text;

	QStringList headerContents;

	QItemSelectionRange range = selectionModel()->selection().first();

	// First add headers IF there is more than one column selected
	if(range.left() != range.right())
	{
		for (auto i = range.left(); i <= range.right(); ++i)
		{
			headerContents << model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
		}
		text += headerContents.join("\t");
		text += "\n";
	}

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

void CallsignTableView::copyRows()
{
	QString text;

	QStringList headerContents;

	QItemSelectionRange range = selectionModel()->selection().first();

	// First add headers
	for (auto i = 0; i < model()->columnCount(); i++)
	{
		headerContents << model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
	}
	text += headerContents.join("\t");
	text += "\n";

	for (auto i = range.top(); i <= range.bottom(); ++i)
	{
		QStringList rowContents;
		for (auto j = 0; j < model()->columnCount(); j++)
		{
			rowContents << model()->index(i, j).data().toString();
		}
		text += rowContents.join("\t");
		text += "\n";
	}

	QGuiApplication::clipboard()->setText(text);
}

void CallsignTableView::showDetailForSelection()
{
	int floor = selectedIndexes().at(0).row();
	QString call = model()->index(floor, 0).data().toString();

	emit showDetailForCall(call);
}