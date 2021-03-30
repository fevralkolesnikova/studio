#include "main_window.h"
#include <iostream>
#include <map>
#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget* parent) :
		QMainWindow(parent) {
	setlocale(LC_ALL, "Russian");
	ui.setupUi(this);
	// db init
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("data.db");
	db.open();
	dbModel = new QSqlQueryModel;
	storageDialog = new Storage;
	orderDialog = new Order;
	tabChange();
	//connects
	connect(ui.tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(handleStorageClick(const QModelIndex &)));
	connect(ui.tableView_3, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(handleStorageClick(const QModelIndex&)));
	connect(storageDialog, &Storage::backSignal, this, &MainWindow::showWindow);
	connect(orderDialog, &Order::backSignal, this, &MainWindow::showWindow);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(filterData()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(newOrder()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(newMaterial()));
	connect(ui.tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChange);
}

void MainWindow::fillTables() {
	char index = ui.tabWidget->currentIndex();
	QTableView *currentTable;
	std::string currentQuery;
	std::map<QString, std::string> translation = {
			{"Ткань", "Material"},
			{"Цвет", "Color"},
			{"Название", "Name"},
			{"Покупатель", "Customer_name"},
			{"Дата", "Order_date"}
	};
	QSqlQuery query;
	if (index == 0) {
		query.exec(R"(SELECT DISTINCT ID FROM Products)");
		while(query.next()) {
			QSqlQuery newQuery;
			newQuery.prepare(R"(SELECT Product_material, Product_color, Size FROM Products WHERE ID=?)");
			newQuery.addBindValue(query.value(0).toInt());
			newQuery.exec();
			newQuery.first();
			QString material = newQuery.value(0).toString();
			QString color = newQuery.value(1).toString();
			float size = newQuery.value(2).toFloat();
			newQuery.prepare(R"(SELECT Amount FROM Fabrics WHERE Material=:material AND Color=:color)");
			newQuery.bindValue(":material", material);
			newQuery.bindValue(":color", color);
			newQuery.exec();
			newQuery.first();
			QString enough;
			if (size<=newQuery.value(0).toFloat()) {
				enough = "Да";
			} else {
				enough = "Нет";
			}
			newQuery.prepare(R"(UPDATE Products SET Enough=:enough WHERE ID=:id)");
			newQuery.bindValue(":enough", enough);
			newQuery.bindValue(":id", query.value(0).toInt());
			newQuery.exec();
		}
		currentQuery = R"(
			SELECT Orders.ID, Name as Название_заказа, Price as Стоимость, Order_date as Дата_начала, Customer_name as Имя_покупателя, Enough as Достаточно_материалов FROM Products
			INNER JOIN Orders ON Products.ID=ID_Product
			INNER JOIN Customers ON Customers.ID=ID_Customer
			WHERE Ready = 0 AND )" + translation[ui.comboBox->currentText()] + " like :search";
		currentTable = ui.tableView;
	} else if (index == 1) {
		currentQuery = R"(
			SELECT Orders.ID, Name as Название_заказа, Price as Стоимость, Ready_date as Дата_окончания, Customer_name as Имя_покупателя FROM Products
			INNER JOIN Orders ON Products.ID=ID_Product
			INNER JOIN Customers ON Customers.ID=ID_Customer
			WHERE Ready = 1 AND )" + translation[ui.comboBox->currentText()] + " like :search";
		currentTable = ui.tableView_2;
		query.exec(R"(SELECT COUNT(*), SUM(Price) FROM Products
			INNER JOIN Orders ON Products.ID=ID_Product
			INNER JOIN Customers ON Customers.ID=ID_Customer
			WHERE Ready = 1)");
		query.first();
		ui.label_3->setText("Количество выполненных заказов: "+query.value(0).toString());
		ui.label_2->setText("Общая прибыль: "+query.value(1).toString());
	} else {
		currentQuery = R"(
			SELECT ID, Material as Ткань, Color as Цвет, Price as Цена_за_метр, Amount as Количество
			from Fabrics where )" + translation[ui.comboBox->currentText()] + " like :search";
		currentTable = ui.tableView_3;
	}
	query.prepare(currentQuery.c_str());
	query.bindValue(":search", QString("%1%").arg(search));
	query.exec();
	dbModel->setQuery(query);
	currentTable->setModel(dbModel);
	currentTable->resizeColumnsToContents();
}

void MainWindow::tabChange() {
	char index = ui.tabWidget->currentIndex();
	ui.lineEdit->setText("");
	ui.comboBox->clear();
	if (index == 2) {
		ui.comboBox->addItems({"Ткань", "Цвет"});
	} else {
		ui.comboBox->addItems({"Название", "Покупатель", "Дата"});
	}
	fillTables();
}

void MainWindow::newMaterial() {
	storageDialog->setup(-1);
	storageDialog->show();
	this->hide();
}

void MainWindow::newOrder() {
	orderDialog->setup(-1);
	orderDialog->show();
	this->hide();
}

void MainWindow::showWindow() {
	fillTables();
	this->show();
}

void MainWindow::filterData() {
	search = ui.lineEdit->text();
	fillTables();
}

void MainWindow::handleStorageClick(const QModelIndex &clickedIndex) {
	if (ui.tabWidget->currentIndex()==0) {
		orderDialog->setup(ui.tableView->model()->data(ui.tableView->model()->index(clickedIndex.row(), 0)).toInt());
		orderDialog->show();
	} else {
		storageDialog->setup(ui.tableView_3->model()->data(ui.tableView_3->model()->index(clickedIndex.row(), 0)).toInt());
		storageDialog->show();
	}
	this->close();
}
