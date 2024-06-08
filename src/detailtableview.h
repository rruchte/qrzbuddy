//
// Created by rob on 5/8/24.
//

#ifndef QRZBUDDY_DETAILTABLEVIEW_H
#define QRZBUDDY_DETAILTABLEVIEW_H


#include <QWidget>
#include <QTableView>

class DetailTableView: public QTableView
{
	Q_OBJECT
public:
	DetailTableView(QWidget* parent = Q_NULLPTR);

protected:
	void keyPressEvent(QKeyEvent *event);

};


#endif //QRZBUDDY_DETAILTABLEVIEW_H
