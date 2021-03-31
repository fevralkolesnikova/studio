#ifndef STUDIO_MAIN_WINDOW_H
#define STUDIO_MAIN_WINDOW_H
#include <QMainWindow>
#include <ui_main_window.h>
#include "storage.h"
#include "order.h"
#include <QString>
#include <string>
#include <QtSql>
// Класс для визуализации основного окна программы
class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = nullptr);
private:
	Ui_MainWindow ui;
	QSqlQueryModel* dbModel;
	Storage *storageDialog;
	Order *orderDialog;
	QString search = "";
	void fillTables();
private slots:
	void tabChange();
	void newMaterial();
	void newOrder();
	void showWindow();
	void filterData();
	void handleStorageClick(const QModelIndex &clickedIndex);
};


#endif //STUDIO_MAIN_WINDOW_H
