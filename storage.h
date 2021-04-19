#ifndef STUDIO_STORAGE_H
#define STUDIO_STORAGE_H
#include <QDialog>
#include <QtSql>
#include <ui_storage.h>
// Класс для визуализации окна материала (добавление/редактура)
class Storage: public QDialog {
	Q_OBJECT
public:
	Storage(QWidget *parent = nullptr);
	// Заполняет, либо очищает поля при вызове диалога
	void setup(int mode);
private:
	int mode = -1;
	Ui_Dialog ui;
private slots:
	// Добавляет или обновляет данные в бд
	void changeMaterials();
	// Закрывает диалог и возвращает в основное окно
	void comeback();
	// Удаляет материал
	void deleteMaterial();
signals:
	// Сигнал для основного окна. Сигнализирует, что надо открыть MainWindow
	void backSignal();
};


#endif //STUDIO_STORAGE_H
