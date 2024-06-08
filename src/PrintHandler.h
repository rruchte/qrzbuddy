//
// Created by rob on 5/11/24.
//

#ifndef QRZBUDDY_PRINTHANDLER_H
#define QRZBUDDY_PRINTHANDLER_H


#include <QEventLoop>
#include <QObject>
#include <QPrinter>

QT_BEGIN_NAMESPACE
class QPainter;
class QPrinter;
class QWebEngineView;
QT_END_NAMESPACE

class PrintHandler : public QObject
{
Q_OBJECT
public:
	PrintHandler(QObject *parent = nullptr);
	void setView(QWebEngineView *view);

public slots:
	void print();
	void printPreview();
	void printDocument(QPrinter *printer);
	void printFinished(bool success);

private:
	QWebEngineView *m_view = nullptr;
	QPrinter m_printer;
	QEventLoop m_waitForResult;
	bool m_inPrintPreview = false;
};

#endif //QRZBUDDY_PRINTHANDLER_H
