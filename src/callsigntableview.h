//
// Created by rob on 5/8/24.
//

#ifndef QRZBUDDY_CALLSIGNTABLEVIEW_H
#define QRZBUDDY_CALLSIGNTABLEVIEW_H


#include <QWidget>
#include <QTableView>

class CallsignTableView: public QTableView
{
	Q_OBJECT
public:
	CallsignTableView(QWidget* parent = Q_NULLPTR);
	void clearAll();

signals:
	void showDetailForCall(const QString &call);

protected:
	void keyPressEvent(QKeyEvent *event);
	void clearSelected();
	void copyRows();
	void copySelected();
	void showDetailForSelection();

private:
	bool enablePaste = false;
	bool enableDelete = true;
	bool enableInsert = false;
	bool enablePartialDelete = false;
};


#endif //QRZBUDDY_CALLSIGNTABLEVIEW_H
