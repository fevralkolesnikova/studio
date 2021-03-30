#ifndef STUDIO_ORDER_H
#define STUDIO_ORDER_H
#include <QDialog>
#include <ui_order.h>
#include <QtSql>
#include <QString>

class Order: public QDialog {
	Q_OBJECT
public:
	Order(QWidget * parent = nullptr);
	void setup(int modeVal);
private:
	Ui_Order ui;
	int mode;
	static QString countPrice(int size, QString material, QString color) {
		QSqlQuery query;
		query.prepare(R"(SELECT Price FROM Fabrics WHERE Material=:material AND Color=:color)");
		query.bindValue(":material", material);
		query.bindValue(":color", color);
		query.exec();
		query.first();
		return std::to_string(size*query.value(0).toInt()).c_str();
	}

private slots:
	void changePrice();
	void completeOrder();
	void changeOrder();
	void comeback();
	void changeColor(const QString & = nullptr);
	void deleteOrder();
signals:
	void backSignal();
};


#endif //STUDIO_ORDER_H
