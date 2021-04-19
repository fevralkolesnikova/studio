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
	// Обновляет данные в таблицах
	void fillTables();
private slots:
	// Слот, отрабатывающий смену вкладки
	void tabChange();
	// Слот для открытия диалога нового материала
	void newMaterial();
	// Слот для открытия диалога нового заказа
	void newOrder();
	// Обновляет и открывает таблицы
	void showWindow();
	// Поиск по ключевому слову из textEdit
	void filterData();
	// Отслеживание двойного нажатия на таблицу
	void handleStorageClick(const QModelIndex &clickedIndex);
};


#endif //STUDIO_MAIN_WINDOW_H
