#include "calendardaylabel.h"

//#include <QDateTime>
#include <QToolTip>
#include <QPainter>
#include <QEnterEvent>

CalendarDayLabel::CalendarDayLabel(int width, int height, TYPE type, QWidget*parent, Qt::WindowFlags f)
	: Super(parent, f), m_Width(width), m_Height(height), m_LabelType(type)

{
	setMouseTracking(true);
	SetupUI();
}


void CalendarDayLabel::SetupUI()
{
	resize(m_Width, m_Height);
	QFont font(this->font());
	QPalette pal = palette();
	pal.setColor(QPalette::WindowText, Qt::darkGray);
	font.setPointSize(15);
	font.setBold(false);
	setAlignment(Qt::AlignCenter);

	switch (m_LabelType) {
		case CalendarDayLabel::WEEKDAYS:
			{
				static int weekDayCounter = 1;
				QLocale locale(QLocale::system());
				QString dayName = locale.dayName(weekDayCounter,QLocale::ShortFormat);
				setText(dayName.toUpper());

				//hangi güne denk gelecek
				weekDayCounter++;
				if (weekDayCounter > 7) weekDayCounter = 1;
			}
			break;
		case CalendarDayLabel::DAY:
			{
				font.setPointSize(20);
				font.setBold(false);
			}
			break;

	}

	setFont(font);
	setPalette(pal);
}

void CalendarDayLabel::setIsSelectedDay(bool newIsSelectedDay)
{
	m_isSelectedDay = newIsSelectedDay;
}

void CalendarDayLabel::setDate(const QDate& newDate)
{
	m_Date = newDate;
}

CalendarDayLabel::TYPE CalendarDayLabel::LabelType() const
{
	return m_LabelType;
}

void CalendarDayLabel::setIsToday(bool newIsToday)
{
	isToday = newIsToday;
}

void CalendarDayLabel::paintEvent(QPaintEvent* event)
{

	QPainter painter(this);
//	painter.drawRect(this->rect());
	switch (m_LabelType) {
		case CalendarDayLabel::WEEKDAYS:
			{
				painter.save();
				painter.setPen(QPen(Qt::red, 5));
				int offsett = 5;
				QPoint left(offsett, this->height());
				QPoint right(this->width()-offsett, this->height());
				painter.drawLine(left, right);
				painter.restore();
				Super::paintEvent(event);
			}
			break;
		case CalendarDayLabel::DAY:
			{
				painter.save();
				int offsett = 2;
				QRect selectRect = this->rect();
				if(m_isClicked) {
					painter.fillRect(selectRect,Qt::lightGray);
				}
				if(m_isSelectedDay) {
					painter.fillRect(selectRect,Qt::red);


				}

				isToday ? painter.setPen(QPen(Qt::green, 7)) : painter.setPen(QPen(Qt::darkGray, 1));

				QPoint left(offsett, this->height());
				QPoint right(this->width()-offsett, this->height());
				painter.drawLine(left, right);
				painter.restore();

				if(m_mouseOverWidget) {
					painter.drawRect(selectRect);
				}

				Super::paintEvent(event);
			}
			break;
	}
}


void CalendarDayLabel::enterEvent(QEnterEvent*)
{
	m_mouseOverWidget = true;
	update();
}

void CalendarDayLabel::leaveEvent(QEvent*)
{
	m_mouseOverWidget = false;
	update();
}

void CalendarDayLabel::mousePressEvent(QMouseEvent*)
{
	m_isClicked = true;
	m_isSelectedDay = true;
	update();
	emit sendDatePresentation(this->m_Date);

}


void CalendarDayLabel::mouseMoveEvent(QMouseEvent*event)
{
	QString toolTipString(this->m_Date.toString());
	QToolTip::showText(mapToGlobal(event->pos()), toolTipString);

}


void CalendarDayLabel::mouseReleaseEvent(QMouseEvent*event)
{
	m_isClicked = false;
	update();
}
