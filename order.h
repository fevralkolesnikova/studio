#ifndef STUDIO_ORDER_H
#define STUDIO_ORDER_H
#include <QDialog>
#include <ui_order.h>
#include <QtSql>
#include <QString>
// ����� ��� ���㠫���樨 ���� ������ (����������/।�����)
class Order: public QDialog {
	Q_OBJECT
public:
	Order(QWidget * parent = nullptr);
	// ��������, ���� ��頥� ���� �� �맮�� �������
	void setup(int modeVal);
private:
	Ui_Order ui;
	int mode;
	// ������뢠�� �⮨����� ���ਠ���
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
	// ������뢠�� �⮨����� ���ਠ���
	void changePrice();
	// �����蠥� �����
	void completeOrder();
	// �������� ��� �������� ����� � ��
	void changeOrder();
	// ����뢠�� ������
	void comeback();
	// ������뢠��, 墠⠥� �� ���ਠ��� ��� �믮������ ������
	void changeColor(const QString & = nullptr);
	// ������ �����
	void deleteOrder();
signals:
	// ������������, �� �㦭� ������ MainWindow
	void backSignal();
};


#endif //STUDIO_ORDER_H
