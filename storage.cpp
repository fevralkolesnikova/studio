#include "storage.h"
#include <QDebug>
Storage::Storage(QWidget *parent) :
QDialog(parent) {
	ui.setupUi(this);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(changeMaterials()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(deleteMaterial()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(comeback()));
}

void Storage::setup(int modeVal) {
	mode = modeVal;
	if (mode == -1) {
		ui.pushButton_2->hide();
		ui.lineEdit->setText("");
		ui.lineEdit_2->setText("");
		ui.lineEdit_3->setText("");
		ui.lineEdit_4->setText("");
	} else {
		QSqlQuery query;
		query.prepare(R"(
			SELECT Material, Color, Price, Amount FROM Fabrics
			WHERE ID=?)");
		query.addBindValue(modeVal);
		query.exec();
		query.first();
		ui.lineEdit->setText(query.value(0).toString());
		ui.lineEdit_2->setText(query.value(1).toString());
		ui.lineEdit_3->setText(query.value(3).toString());
		ui.lineEdit_4->setText(query.value(2).toString());
	}
}

void Storage::changeMaterials() {
	QSqlQuery query;
	if (mode == -1) {
		query.prepare(R"(INSERT INTO Fabrics(Material, Color, Price, Amount) values (:material, :color, :price, :amount))");
	} else {
		query.prepare(R"(
			UPDATE Fabrics SET Material = :material, Color = :color, Price = :price, Amount = :amount
			WHERE ID = :id
)");
		query.bindValue(":id", mode);
	}
	query.bindValue(":material", ui.lineEdit->text());
	query.bindValue(":color", ui.lineEdit_2->text());
	query.bindValue(":price", ui.lineEdit_4->text());
	query.bindValue(":amount", ui.lineEdit_3->text());
	query.exec();
	comeback();
}

void Storage::comeback() {
	emit backSignal();
	this->close();
}

void Storage::deleteMaterial() {
	QSqlQuery query;
	query.prepare(R"(DELETE FROM FABRICS WHERE ID=:id)");
	query.bindValue(":id", mode);
	query.exec();
	comeback();
}