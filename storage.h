#ifndef STUDIO_STORAGE_H
#define STUDIO_STORAGE_H
#include <QDialog>
#include <QtSql>
#include <ui_storage.h>
// ����� ��� ���㠫���樨 ���� ���ਠ�� (����������/।�����)
class Storage: public QDialog {
	Q_OBJECT
public:
	Storage(QWidget *parent = nullptr);
	// ��������, ���� ��頥� ���� �� �맮�� �������
	void setup(int mode);
private:
	int mode = -1;
	Ui_Dialog ui;
private slots:
	// �������� ��� �������� ����� � ��
	void changeMaterials();
	// ����뢠�� ������ � �����頥� � �᭮���� ����
	void comeback();
	// ������ ���ਠ�
	void deleteMaterial();
signals:
	// ������ ��� �᭮����� ����. ������������, �� ���� ������ MainWindow
	void backSignal();
};


#endif //STUDIO_STORAGE_H
