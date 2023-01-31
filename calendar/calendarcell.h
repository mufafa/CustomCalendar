#ifndef CALENDARCELL_H
#define CALENDARCELL_H

#include <QLabel>
#include <QWidget>

class CalendarCell : public QLabel
{
		Q_OBJECT
		using Super = QLabel;
	public:
		explicit CalendarCell(QWidget *parent = nullptr);

};

#endif // CALENDARCELL_H
