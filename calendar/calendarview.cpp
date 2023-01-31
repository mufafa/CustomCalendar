#include "calendarview.h"
#include "ui_calendarview.h"

#include <QCalendar>
#include <QDate>
#include <QPainter>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "calendar/calendardaylabel.h"


static QPainterPath DrawPopWindow(QRect rect, int radius = 0, int inlineWeight = 0) {

	rect.adjust(inlineWeight,inlineWeight,-inlineWeight,-inlineWeight);
	int trigangleWidth = rect.width()/5;
	QPainterPath topTriangle;
	if(radius != 0) radius = radius/2;
	topTriangle.addRoundedRect(QRect(0,0,trigangleWidth,trigangleWidth), radius, radius);

//	qreal hip = sqrt((trigangleWidth * trigangleWidth) + (trigangleWidth * trigangleWidth));
	qreal hip = sqrt(pow(trigangleWidth,2) + pow(trigangleWidth,2));
	int location = rect.width()/2 - hip/2;
	topTriangle = QTransform()
				 .translate(location,(trigangleWidth/2 + trigangleWidth/5))
				 .rotate(-45)
				 .map(topTriangle);

	QPainterPath mainRect;
	mainRect.addRoundedRect(rect.adjusted(0,trigangleWidth/2,0,0), radius, radius);

	return mainRect + topTriangle;
}



struct Calendar {
	private:
		int fyear, fmonth, fday;
		QCalendar calendar;
		QDate date;
	public:
		Calendar(int year, int month, int day = 1) : fyear(year), fmonth(month), fday(day) {
			date = calendar.dateFromParts(fyear, fmonth, fday);
		}

		void setCalendarYearAndMonth(int year, int month) {
			this->fyear = year;
			this->fmonth = month;
		}

		//bir ay kaç gün çekiyor
		int DaysInMonth() {
			return calendar.daysInMonth(fmonth,fyear);
		}

		//ayın ilk günü hanqisine denk geliyor 1=monday 7=saturday
		int firstDayInMonth() {
			auto date = calendar.dateFromParts(fyear, fmonth, 1);

			return date.dayOfWeek();
		}

		//ayın son günü hangi güne denk geliyor
		int lastDayInMonth() {
			int daysImMonth = DaysInMonth();
			auto date = calendar.dateFromParts(fyear, fmonth, daysImMonth);
			return date.dayOfWeek();
		}


		QString GetDateLabelString() {
			QLocale locale(QLocale::system());
			auto monthName = locale.monthName(fmonth);
			return QString("%1, %2").arg(monthName, QString::number(fyear));
		}

		//verilen gün bügünmüdür
		static bool isToday(int year, int month, int day) {
			return QDate::currentDate().year() == year &&
					QDate::currentDate().month() == month &&
					QDate::currentDate().day() == day;
		}

		static QString dayName(int day) {
			if(day < 1 || day > 7 ) return QString("Hata");
			QCalendar calendar;
			QLocale locale(QLocale::system());
			auto dayName = locale.dayName(day);
			return QString("%1").arg(dayName);
		}

		static QString monthName(int month) {
			if(month < 1 || month > 12 ) return QString("Hata");
			QCalendar calendar;
			QLocale locale(QLocale::system());
			auto monthName = locale.monthName(month);
			return QString("%1").arg(monthName);
		}

};

static int ColumnCount(int day) {
	//TODO: localenin ilk günü gelmeli
	int firstDayLocale = 1;
	int result = day - firstDayLocale;
	if(result < 0) {
		result = result + 7;
	}
	return result;
}


CalendarView::CalendarView(QWidget *parent) :
//	QWidget(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint),
	QWidget(parent, Qt::Popup),
	ui(new Ui::CalendarView)
{
	setAttribute(Qt::WA_TranslucentBackground);
	ui->setupUi(this);


//	QPalette pal = palette();
//	//popup panelin arka plan rengini değiştirelim.
//	pal.setColor(QPalette::Window, Qt::white);
//	setPalette(pal);

	m_LastYear = QDate::currentDate().year();
	m_LastMonth = QDate::currentDate().month();


	ui->leftArrow->setText("<");
	ui->rightArrow->setText(">");

	connect(ui->leftArrow, &CalendarArrowButton::clicked, this, [=]() {
		WalkMonts(-1);
	});

	connect(ui->rightArrow, &CalendarArrowButton::clicked, this, [=]() {
		WalkMonts(1);
	});
	insertLabels();
	initUI();
}


void CalendarView::insertLabels()
{
	//label width for calculation
	int dayLabelWidth = 38;
	//left rignt margins
	qreal lrMargin = (this->width() - dayLabelWidth*7) / 2;

	//weekdays
	for(int i = 0 ; i < 7 ; i++) {
		CalendarDayLabel *tmp = new CalendarDayLabel(dayLabelWidth,30, CalendarDayLabel::WEEKDAYS, this);
		tmp->move(lrMargin+ i*dayLabelWidth, 75);
	}

	//label yüksekliği
	int vHeight = 65 + dayLabelWidth + lrMargin;
	for(int row = 0; row < 6;row++) {
		for(int col = 0; col < 7; col++) {
			CalendarDayLabel *tmp = new CalendarDayLabel(dayLabelWidth,dayLabelWidth, CalendarDayLabel::DAY,this);

			connect(tmp, &CalendarDayLabel::sendDatePresentation, this, &CalendarView::showSelectedDate);
			QPoint movePoint(lrMargin + col*dayLabelWidth, vHeight);
			tmp->move(movePoint);
			dayLabels.append(tmp);
		}
		vHeight += dayLabelWidth;
	}
}

void CalendarView::populateMountDays(int firstDay, int lastDay)
{
	//hide every label
	for(auto& l : dayLabels) {
		l->hide();
		l->setIsSelectedDay(false);
	}

	//kaçıncı kolumun kaçıncı günü
	int col = ColumnCount(firstDay);
	int day = 1;
	for(int row = 0; row < 6; row++) {
		for(; col < 7 ; col++) {
			CalendarDayLabel *tmp = dayLabels[row * 7 + col]; //row * columnCount + col;
			if(day <= lastDay) {
				tmp->show();
				tmp->setText(QString::number(day));
				tmp->setDate(QDate(m_LastYear, m_LastMonth, day));
				bool isToday = Calendar::isToday(m_LastYear, m_LastMonth, day);
				tmp->setIsToday(isToday);

				day++;
			}
		}
		col = 0;
	}
}

void CalendarView::showSelectedDate(const QDate& date)
{
	initUI();
	qDebug() << "Selected date : " << date;
	CalendarDayLabel* cLabel = qobject_cast<CalendarDayLabel*>(sender());
	cLabel->setIsSelectedDay(true);

//	close();
}


void CalendarView::initUI()
{
	Calendar calendar(m_LastYear, m_LastMonth);
	auto yearLabelText = calendar.GetDateLabelString();
	ui->yearLabel->setText(yearLabelText);


	int daysInMonth = calendar.DaysInMonth();
	int firstDay = calendar.firstDayInMonth();
	populateMountDays(firstDay, daysInMonth);

}







void CalendarView::WalkMonts(int step)
{
	m_LastMonth += step;
	if(m_LastMonth > 12) {
		m_LastMonth = 1;
		m_LastYear++;
	};
	if(m_LastMonth < 1) {
		m_LastMonth = 12;
		m_LastYear--;
	}
	initUI();
}


void CalendarView::PopUp(QAbstractButton*senderButton)
{
	QRect widgetRect = senderButton->geometry();
	widgetRect.moveTopLeft(senderButton->parentWidget()->mapToGlobal(widgetRect.topLeft()));
	this->move(widgetRect.center().x() - this->width()/2, widgetRect.center().y()+senderButton->height()/2);
	this->show();
}

CalendarView::~CalendarView()
{
	delete ui;
}



void CalendarView::paintEvent(QPaintEvent*event)
{

	QPainterPath mask = DrawPopWindow(this->rect(),16,0);


	QStyleOption opt;
	opt.initFrom(this);
	QPainter p(this);

	p.fillPath(mask, Qt::white);

//	p.fillRect(this->rect(), Qt::white);
//	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}
