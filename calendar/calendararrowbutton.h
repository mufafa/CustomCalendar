#ifndef CALENDARARROWBUTTON_H
#define CALENDARARROWBUTTON_H

#include <QPushButton>
#include <QWidget>

class CalendarArrowButton : public QPushButton
{
		Q_OBJECT
		using Super = QPushButton;
	public:
		CalendarArrowButton(QWidget *parent = nullptr);


	private:
		bool isInside{false};
		void SetupUI();
	protected:
		virtual void mouseMoveEvent(QMouseEvent*event) override;
		virtual void paintEvent(QPaintEvent*event) override;
		virtual void enterEvent(QEnterEvent*event) override;
		virtual void leaveEvent(QEvent*event) override;
};

#endif // CALENDARARROWBUTTON_H
