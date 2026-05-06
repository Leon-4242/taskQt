#ifndef DATA_H
#define DATA_H

#include <string>

#include <QObject> 

class Data : public QObject
{
	Q_OBJECT
public:
	int k;
	std::string name;
	double originAbsMax;
	double polynomAbsMax;
	double piecePolynomAbsMax;
	double errorPolynomAbsMax;
	double errorPiecePolynomAbsMax;

	bool origin, polynom, piecePolynom, error;
	int s, n, p;

	Data(QObject *parent = nullptr);
signals:
	void changed();
};

#endif // DATA_H
