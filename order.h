#ifndef STUDIO_ORDER_H
#define STUDIO_ORDER_H
#include <QDialog>
#include <ui_order.h>
#include <QtSql>
#include <QString>
// Класс для визуализации окна заказа (добавление/редактура)
class Order: public QDialog {
	Q_OBJECT
public:
	Order(QWidget * parent = nullptr);
	// Заполняет, либо очищает поля при вызове диалога
	void setup(int modeVal);
private:
	Ui_Order ui;
	int mode;
	// Рассчитывает стоимость материалов
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
	// Пересчитывает стоимость материалов
	void changePrice();
	// Завершает заказ
	void completeOrder();
	// Добавляет или обновляет записи в бд
	void changeOrder();
	// Закрывает диалог
	void comeback();
	// Рассчитывает, хватает ли материалов для выполнения заказа
	void changeColor(const QString & = nullptr);
	// Удаляет заказ
	void deleteOrder();
signals:
	// Сигнализирует, что нужно открыть MainWindow
	void backSignal();
};


#endif //STUDIO_ORDER_H
