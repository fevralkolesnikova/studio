#ifndef STUDIO_STORAGE_H
#define STUDIO_STORAGE_H
#include <QDialog>
#include <QtSql>
#include <ui_storage.h>

class Storage: public QDialog {
	Q_OBJECT
public:
	Storage(QWidget *parent = nullptr);
	void setup(int mode);
private:
	int mode = -1;
	Ui_Dialog ui;
private slots:
	void changeMaterials();
	void comeback();
	void deleteMaterial();
signals:
	void backSignal();
};


#endif //STUDIO_STORAGE_H
