#ifndef CALENDARDAYLABEL_H
#define CALENDARDAYLABEL_H

#include <QDate>
#include <QLabel>
#include <QWidget>

class CalendarDayLabel : public QLabel
{
		Q_OBJECT
		using Super = QLabel;
	public:
		enum TYPE {
			WEEKDAYS, //üst taraftaki etikler için
			DAY, // ayın günleri için
		};

	public:
		CalendarDayLabel(int width, int height, TYPE type, QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());
		TYPE LabelType() const;
		void setIsToday(bool newIsToday);
		void setDate(const QDate&newDate);
		void setIsSelectedDay(bool newIsSelectedDay);

	private:
		bool isToday{false};
		void SetupUI();

	private:
		int m_Width;
		int m_Height;
		bool m_mouseOverWidget{false};
		bool m_isClicked{false};
		bool m_isSelectedDay{false};
		TYPE m_LabelType;
		QDate m_Date;

	signals:
		void sendDatePresentation(const QDate& selectedDate);


		// QWidget interface
	protected:
		virtual void paintEvent(QPaintEvent*event) override;
		virtual void enterEvent(QEnterEvent*event) override;
		virtual void leaveEvent(QEvent*event) override;
		virtual void mousePressEvent(QMouseEvent*event) override;
		virtual void mouseMoveEvent(QMouseEvent*event) override;
		virtual void mouseReleaseEvent(QMouseEvent*event) override;
};

#endif // CALENDARDAYLABEL_H
