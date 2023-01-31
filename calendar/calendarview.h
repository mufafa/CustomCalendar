#ifndef CALENDARVIEW_H
#define CALENDARVIEW_H

#include <QAbstractButton>
#include <QWidget>

namespace Ui {
	class CalendarView;
}
class CalendarDayLabel;
class CalendarView : public QWidget
{
		Q_OBJECT
		using Super = QWidget;

		QList<CalendarDayLabel*> dayLabels;
	public:

		explicit CalendarView(QWidget *parent = nullptr);
		void PopUp(QAbstractButton* senderButton);
		~CalendarView();
	private:
		Ui::CalendarView *ui;
		void initUI();
		void WalkMonts(int step);

		int m_LastYear;
		int m_LastMonth;

	private:
		void insertLabels();
		void populateMountDays(int firstDay, int lastDay);

	public slots:
		void showSelectedDate(const QDate& date);



		// QWidget interface
	protected:
		virtual void paintEvent(QPaintEvent*event) override;
};

#endif // CALENDARVIEW_H
