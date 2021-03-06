#ifndef STUDIO_MAIN_WINDOW_H
#define STUDIO_MAIN_WINDOW_H
#include <QMainWindow>
#include <ui_main_window.h>
#include "storage.h"
#include "order.h"
#include <QString>
#include <string>
#include <QtSql>
// ????? ??? ???㠫???樨 ?᭮????? ???? ?ணࠬ??
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
	// ????????? ?????? ? ⠡?????
	void fillTables();
private slots:
	// ????, ??ࠡ??뢠?騩 ᬥ?? ???????
	void tabChange();
	// ???? ??? ???????? ??????? ?????? ????ਠ??
	void newMaterial();
	// ???? ??? ???????? ??????? ?????? ??????
	void newOrder();
	// ????????? ? ????뢠?? ⠡????
	void showWindow();
	// ????? ?? ???祢??? ᫮?? ?? textEdit
	void filterData();
	// ??᫥??????? ???????? ??????? ?? ⠡????
	void handleStorageClick(const QModelIndex &clickedIndex);
};


#endif //STUDIO_MAIN_WINDOW_H
