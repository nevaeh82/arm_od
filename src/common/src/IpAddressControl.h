#pragma once


#include <QFrame>
#include <QLineEdit>
#include <QIntValidator>
#include "stdint.h"
#include <QHBoxLayout>
#include <QFont>
#include <QLabel>
#include <QKeyEvent>
#include <QStringList>

class IpAddressControl : public QFrame
{
	Q_OBJECT
private:
	enum
	{
		QTUTL_IP_SIZE   = 4,// number of octets in ip address
		MAX_DIGITS      = 3 // number of lines in LineEdit
	};

	QLineEdit* m_pLineEdit[QTUTL_IP_SIZE];

public:
	IpAddressControl(QWidget* parent = 0);
	~IpAddressControl();

	virtual bool eventFilter(QObject*, QEvent*);

	QString text();
	void setText(QString ipAddress);

private:
	void MoveNextLineEdit(int);
	void MovePrevLineEdit(int);

public slots:
	void slotTextChanged(QLineEdit*);

signals:
	void signalTextChanged(QLineEdit*);
	void signalEditingFinished();
	void signalFocusOut();
};


