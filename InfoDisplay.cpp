#include "InfoDisplay.h"

InfoDisplay::InfoDisplay (QWidget *parent, Data *data):
	QWidget(parent),
	data(data),
	bgColor(Qt::lightGray)
{
	connect(data, &Data::changed, this, &InfoDisplay::dataChanged);

	table = new QVBoxLayout(this);

	label = new QLabel(this);
	label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
//	label->setTextInteractionFlags(Qt::TextSelectableByMouse);

	table->addWidget(label);
	table->addStretch();
}


void InfoDisplay::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	int w = width();
	int h = height();
	if (w == 0 || h == 0)
		return;

	painter.setBrush(QBrush(bgColor));
	painter.drawRect(0, 0, w, h);
	painter.setBrush(Qt::NoBrush);
	
	painter.setRenderHint(QPainter::Antialiasing, true);
	drawInformation(/*&painter*/);
	drawConsole();
	event->accept();
}

void InfoDisplay::drawConsole(void)
{
	static bool flag = false;
	static int lines = 0;
	if (flag) {

		std::cout << "\033[" << lines << "A";
		lines = 0;
		std::cout << "\033[2K\r" << "k = " << data->k << " " + data->name + "\n"; 
		++lines;
		if (data->origin) {
			std::cout << "\033[2K\r" << "max |f_origin(x)| = " << data->originAbsMax << "\n";
			++lines;
		}
		if (data->polynom) {
			std::cout << "\033[2K\r" << "max |f_polynom(x)| = " << data->polynomAbsMax << "\n";
			++lines;
		}
		if (data->piecePolynom) {
			std::cout << "\033[2K\r" << "max |f_piece_polynom(x)| = " << data->piecePolynomAbsMax << "\n";
			++lines;
		}
		if (data->error) {
			std::cout << "\033[2K\r" << "max |f_polynom_error(x)| = " << data->errorPolynomAbsMax << "\n";
			std::cout << "\033[2K\r" << "max |f_piece_polynom_error(x)| = " << data->errorPiecePolynomAbsMax << "\n";
			std::cout << "\033[2K\r";
			lines += 2;
		}

		std::cout.flush();
	} else {
		std::cout << "k = " << data->k << " " + data->name + "\n"; 
		++lines;
		if (data->origin) {
			std::cout << "max |f_origin(x)| = " << data->originAbsMax << "\n";
			++lines;
		}
		if (data->polynom) {
			std::cout << "max |f_polynom(x)| = " << data->polynomAbsMax << "\n";
			++lines;
		}
		if (data->piecePolynom) {
			std::cout << "max |f_piece_polynom(x)| = " << data->piecePolynomAbsMax << "\n";
			++lines;
		}
		if (data->error) {
			std::cout << "max |f_polynom_error(x)| = " << data->errorPolynomAbsMax << "\n";
			std::cout << "max |f_piece_polynom_error(x)| = " << data->errorPiecePolynomAbsMax << "\n";
			lines += 2;
		}

		flag = true;
	}

}
void InfoDisplay::drawInformation(void /*QPainter *painter*/) 
{

	QString text;

	text += QString("k = %1 %2\n")
		.arg(data->k)
		.arg(QString::fromStdString(data->name));

	if (data->origin)
		text += QString("max |f_origin(x)|:\n %1\n").arg(data->originAbsMax);

	if (data->polynom)
		text += QString("max |f_polynom(x)|:\n %1\n").arg(data->polynomAbsMax);

	if (data->piecePolynom)
		text += QString("max |f_piece_polynom(x)|:\n %1\n").arg(data->piecePolynomAbsMax);

	if (data->error) {
		text += QString("max |f_polynom_error(x)|:\n %1\n").arg(data->errorPolynomAbsMax);
		text += QString("max |f_piece_polynom_error(x)|:\n %1\n").arg(data->errorPiecePolynomAbsMax);
	}
	
	text += QString("\ns = %1\nn = %2\np = %3")
		.arg(data->s)
		.arg(data->n)
		.arg(data->p);

	label->setText(text);

	/*
	painter->setPen(Qt::blue);
	painter->drawText(0, 20, QString::fromStdString( "k = " + std::to_string(data->k) + " " + data->name));
	if (data->origin)
		painter->drawText(0, 40, QString::fromStdString("max |f_origin(x)| = " + std::to_string(data->originAbsMax)));
	if (data->polynom)
		painter->drawText(0, 60, QString::fromStdString("max |f_polynom(x)| = " + std::to_string(data->polynomAbsMax)));
	if (data->piecePolynom)
		painter->drawText(0, (data->polynom ? 80 : 60), QString::fromStdString("max |f_piece_polynom(x)| = " + std::to_string(data->piecePolynomAbsMax)));
	if (data->error) {
		painter->drawText(0, 40, QString::fromStdString("max |f_polynom_error(x)| = " + std::to_string(data->errorPolynomAbsMax)));
		painter->drawText(0, 60, QString::fromStdString("max |f_piece_polynom_error(x)| = " + std::to_string(data->errorPiecePolynomAbsMax)));
	}

	painter->drawText(0, data->origin && data->polynom && data->piecePolynom ? 100 : 80, QString::fromStdString( "s = " + std::to_string(data->s)));
	painter->drawText(0, data->origin && data->polynom && data->piecePolynom ? 120 : 100, QString::fromStdString( "n = " + std::to_string(data->n)));
	painter->drawText(0, data->origin && data->polynom && data->piecePolynom ? 140 : 120, QString::fromStdString( "p = " + std::to_string(data->p)));
	*/
}
