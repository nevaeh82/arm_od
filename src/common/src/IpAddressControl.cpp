#include "IpAddressControl.h"

IpAddressControl::IpAddressControl(QWidget *parent) : QFrame(parent)
{
	setFrameShape(QFrame::StyledPanel);
	setFrameShadow(QFrame::Sunken);

	QHBoxLayout* pLayout = new QHBoxLayout(this);
	setLayout(pLayout);
	pLayout->setContentsMargins(0, 0, 1, 0);
	pLayout->setSpacing(0);

	for (int i = 0; i != QTUTL_IP_SIZE; ++i)
	{
		if (i != 0)
		{
			QLabel* pDot = new QLabel(".", this);
			// for future
//			QString bgColorName = palette().color(QPalette::Normal, QPalette::Window).name();
//			QPalette palette = pDot->palette();
//			palette.setColor(pDot->backgroundRole(), QColor(bgColorName));
//			pDot->setPalette(palette);
			pLayout->addWidget(pDot);
			pLayout->setStretch(pLayout->count(), 0);
		}

		m_pLineEdit[i] = new QLineEdit(this);
		QLineEdit* pEdit = m_pLineEdit[i];
		pEdit->installEventFilter(this);

		connect(pEdit, SIGNAL(editingFinished()), this, SIGNAL(signalEditingFinished()));

		pLayout->addWidget(pEdit);
		pLayout->setStretch(pLayout->count(), 1);

		pEdit->setFrame(false);
		pEdit->setAlignment(Qt::AlignCenter);

		QFont font = pEdit->font();
		font.setStyleHint(QFont::Monospace);
		font.setFixedPitch(true);
		pEdit->setFont(font);

		QRegExp rx ("^(0|[1-9]|[1-9][0-9]|1[0-9][0-9]|2([0-4][0-9]|5[0-5]))$");
		QValidator* validator = new QRegExpValidator(rx, pEdit);
		pEdit->setValidator(validator);
	}

	setMaximumWidth(30 * QTUTL_IP_SIZE);

	connect(this, SIGNAL(signalTextChanged(QLineEdit*)), this, SLOT(slotTextChanged(QLineEdit*)), Qt::QueuedConnection);
}

IpAddressControl::~IpAddressControl()
{

}

QString IpAddressControl::text()
{
	QString ipText;

	for (unsigned int i = 0; i != QTUTL_IP_SIZE; ++i){
		ipText += m_pLineEdit[i]->text();

		if (i != QTUTL_IP_SIZE - 1){
			ipText += ".";
		}
	}

	return ipText;
}

void IpAddressControl::setText(QString ipAddress)
{
	QStringList octets = ipAddress.split('.');
	unsigned int curOctet = 0;

	foreach (QString octet, octets){
		if (curOctet == QTUTL_IP_SIZE){
			break;
		}

		m_pLineEdit[curOctet]->setText(octet);

		++curOctet;
	}
}

void IpAddressControl::slotTextChanged(QLineEdit* pEdit)
{
	for (unsigned int i = 0; i != QTUTL_IP_SIZE; ++i)
	{
		if (pEdit != m_pLineEdit[i]){
			continue;
		}

		if ((pEdit->text().size() != MAX_DIGITS ||  pEdit->text().size() != pEdit->cursorPosition()) && (pEdit->text() != "0")){
			continue;
		}

		// auto-move to next item
		if (i + 1 == QTUTL_IP_SIZE){
			continue;
		}

		m_pLineEdit[i + 1]->setFocus();
		m_pLineEdit[i + 1]->selectAll();
	}
}

bool IpAddressControl::eventFilter(QObject *obj, QEvent *event)
{
	bool bRes = QFrame::eventFilter(obj, event);

	if (event->type() == QEvent::FocusOut){
		emit signalFocusOut();
		return false;
	}

	if (event->type() != QEvent::KeyPress){
		return false;
	}

	QKeyEvent* pEvent = dynamic_cast<QKeyEvent*>(event);

	if (!pEvent){
		return false;
	}

	for (unsigned int i = 0; i != QTUTL_IP_SIZE; ++i)
	{
		QLineEdit* pEdit = m_pLineEdit[i];

		if (pEdit != obj){
			continue;
		}

		switch (pEvent->key())
		{
			case Qt::Key_Left:
				if (pEdit->cursorPosition() == 0)
				{
					// user wants to move to previous item
					MovePrevLineEdit(i);
				}
				break;

			case Qt::Key_Right:
				if (pEdit->text().isEmpty() || (pEdit->text().size() == pEdit->cursorPosition()))
				{
					// user wants to move to next item
					MoveNextLineEdit(i);
				}
				break;

			case Qt::Key_0:
				if (pEdit->text().isEmpty() || pEdit->text() == "0")
				{
					pEdit->setText("0");
					// user wants to move to next item
					MoveNextLineEdit(i);
				}
				emit signalTextChanged(pEdit);
				break;

			case Qt::Key_Backspace:
				if (pEdit->text().isEmpty() || pEdit->cursorPosition() == 0)
				{
					// user wants to move to previous item
					MovePrevLineEdit(i);
				}
				break;

			case Qt::Key_Comma:
			case Qt::Key_Period:
				MoveNextLineEdit(i);
				break;

			default:
				emit signalTextChanged(pEdit);
				break;
		}
	}

	return bRes;
}

void IpAddressControl::MoveNextLineEdit(int i)
{
	if (i + 1 == QTUTL_IP_SIZE){
		return;
	}

	m_pLineEdit[i+1]->setFocus();
	m_pLineEdit[i+1]->setCursorPosition(0);
	m_pLineEdit[i+1]->selectAll();
}

void IpAddressControl::MovePrevLineEdit(int i)
{
	if (i == 0){
		return;
	}

	m_pLineEdit[i-1]->setFocus();
	m_pLineEdit[i-1]->setCursorPosition(m_pLineEdit[i - 1]->text().size());
}
