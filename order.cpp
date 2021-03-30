#include "order.h"
#include <ctime>
#include <iostream>
#include <QDebug>

Order::Order(QWidget *parent) :
		QDialog(parent) {
	ui.setupUi(this);
	ui.spinBox->setMinimum(0);
	ui.spinBox->setMaximum(100000);
	ui.doubleSpinBox->setMinimum(0);
	ui.doubleSpinBox->setMaximum(1000);
	connect(ui.comboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(changeColor(const QString &)));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(changeOrder()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(deleteOrder()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(comeback()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(changePrice()));
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(completeOrder()));
}

void Order::setup(int modeVal) {
	mode = modeVal;
	ui.comboBox->clear();
	QSqlQuery query;
	query.exec((R"(SELECT DISTINCT Material from Fabrics)"));
	while (query.next()) {
		ui.comboBox->addItem(query.value(0).toString());
	}
	changeColor();
	if (modeVal == -1) {
		ui.pushButton_2->hide();
		ui.pushButton_5->hide();
		ui.label_15->setText("Стоимость материалов: ?");
		ui.lineEdit_5->setText("");
		ui.lineEdit_6->setText("");
		ui.lineEdit_7->setText("");
		ui.lineEdit_10->setText("");
		ui.spinBox->setValue(0);
		ui.doubleSpinBox->setValue(0);
	} else {
		ui.pushButton_2->show();
		ui.pushButton_5->show();
		query.prepare(R"(SELECT Name, Customer_name, Phone_number, Email, Product_material, Product_color, Size, Price, Orders.ID
			FROM Products
			INNER JOIN Orders ON Products.ID=ID_Product
			INNER JOIN Customers ON Customers.ID=ID_Customer
			WHERE Ready=0 AND Orders.ID=?)");
		query.addBindValue(mode);
		query.exec();
		query.first();
//		for (int i = 0; i < 8; i++) {
//			qDebug()<<query.value(i)<<endl;
//		}
		ui.lineEdit_6->setText(query.value(0).toString());
		ui.lineEdit_5->setText(query.value(1).toString());
		ui.lineEdit_7->setText(query.value(2).toString());
		ui.lineEdit_10->setText(query.value(3).toString());
		ui.comboBox->setCurrentText(query.value(4).toString());
		ui.comboBox_2->setCurrentText(query.value(5).toString());
		ui.doubleSpinBox->setValue(query.value(6).toFloat());
		ui.spinBox->setValue(query.value(7).toInt());
		changePrice();
	}
}

void Order::changeOrder() {
	QSqlQuery query;
	if (mode == -1) {
		query.prepare(R"(INSERT INTO Customers(Customer_name, Phone_number, Email) VALUES (:name, :number, :email))");
		query.bindValue(":name", ui.lineEdit_5->text());
		query.bindValue(":number", ui.lineEdit_7->text());
		query.bindValue(":email", ui.lineEdit_10->text());
		std::cout<<query.exec();
		int customerId = query.lastInsertId().toInt();

		query.prepare(R"(INSERT INTO Products(Name, Product_material, Product_color, Size) VALUES (:productName, :material, :color, :size))");
		query.lastError();
		query.bindValue(":productName", ui.lineEdit_6->text());
		query.bindValue(":material", ui.comboBox->currentText());
		query.bindValue(":color", ui.comboBox_2->currentText());
		query.bindValue(":size", ui.doubleSpinBox->value());
		std::cout<<query.exec();
		query.lastError();
		int productId = query.lastInsertId().toInt();

		query.prepare(R"(
		INSERT INTO Orders(ID_Product, ID_Customer, Order_date, Price, Ready)
		VALUES (:productId, :customerId, :date, :price, 0))");
		query.bindValue(":productId", productId);
		query.bindValue(":customerId", customerId);
		std::time_t t = std::time(nullptr);
		char buf[35];
		std::strftime(buf, std::size(buf), "%d.%m.%Y", localtime(&t));
		std::string date = std::string(buf);
//		std::cout<<date;
		query.bindValue(":date", date.c_str());
		query.bindValue(":price", ui.spinBox->value());
		std::cout<<query.exec();
	} else {
		query.prepare(R"(UPDATE Orders SET Price=:price WHERE ID=:id)");
		query.bindValue(":price", ui.spinBox->value());
		query.bindValue(":id", mode);
		query.exec();

		query.prepare(R"(
			UPDATE Customers SET Customer_name=:name, Phone_number=:number, Email=:email
			WHERE ID = (SELECT ID_Customer FROM Orders WHERE ID=:id))");
		query.bindValue(":name", ui.lineEdit_5->text());
		query.bindValue(":number", ui.lineEdit_7->text());
		query.bindValue(":email", ui.lineEdit_10->text());
		query.bindValue(":id", mode);
		query.exec();

		query.prepare(R"(
			UPDATE Products SET Name=:productName, Product_material=:material, Product_color=:color, Size=:size
			WHERE ID = (SELECT ID_Product From Orders WHERE ID=:id))");
		query.bindValue(":productName", ui.lineEdit_6->text());
		query.bindValue(":material", ui.comboBox->currentText());
		query.bindValue(":color", ui.comboBox_2->currentText());
		query.bindValue(":size", ui.doubleSpinBox->value());
		query.bindValue(":id", mode);
		query.exec();
	}
	comeback();
}

void Order::completeOrder() {
	QSqlQuery query;
	query.prepare(R"(
		SELECT Product_material, Product_color, Size FROM Products
		WHERE ID = (SELECT ID_Product FROM Orders WHERE ID=?))");
	qDebug()<<query.lastError();
	query.addBindValue(mode);
	query.exec();
	query.first();
	QString material = query.value(0).toString();
	QString color = query.value(1).toString();
	float size = query.value(2).toFloat();

	query.prepare(R"(UPDATE Fabrics SET Amount=Amount-:size WHERE Material=:material AND Color=:color)");
	query.bindValue(":size", size);
	query.bindValue(":material", material);
	query.bindValue(":color", color);
	query.exec();

	query.prepare(R"(UPDATE Orders SET Ready_date=:date, Ready=1 WHERE ID=:id)");
	std::time_t t = std::time(nullptr);
	char buf[35];
	std::strftime(buf, std::size(buf), "%d.%m.%Y", localtime(&t));
	std::string date = std::string(buf);
	query.bindValue(":date", date.c_str());
	query.bindValue(":id", mode);
	query.exec();
	comeback();
}

void Order::changePrice() {
	ui.label_15->setText("Стоимость материалов: " +
	                     countPrice(ui.doubleSpinBox->value(), ui.comboBox->currentText(), ui.comboBox_2->currentText()));
}

void Order::changeColor(const QString &) {
	QString material = ui.comboBox->currentText();
	QSqlQuery query;
	ui.comboBox_2->clear();
	qDebug()<<material;
	query.prepare((R"(SELECT DISTINCT Color from Fabrics WHERE Material=:material)"));
	query.bindValue(":material", material);
	query.exec();
	qDebug()<<query.size();
	while (query.next()) {
		ui.comboBox_2->addItem(query.value(0).toString());
	}
}

void Order::comeback() {
	emit backSignal();
	this->close();
}

void Order::deleteOrder() {
	QSqlQuery query;
	query.prepare(R"(SELECT FROM Orders ID_Product, ID_Customer WHERE ID=?)");
	query.addBindValue(mode);
	query.exec();
	query.first();
	int productId = query.value(0).toInt();
	int customerId = query.value(1).toInt();

	query.prepare(R"(DELETE FROM Orders WHERE ID=?)");
	query.addBindValue(mode);
	query.exec();

	query.prepare(R"(DELETE FROM Products WHERE ID=?)");
	query.addBindValue(productId);
	query.exec();

	query.prepare(R"(DELETE FROM Customers WHERE ID=?)");
	query.addBindValue(customerId);
	query.exec();
	comeback();
}
