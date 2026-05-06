#include "CommonDefs.h"

#include "MainWindow.h"
#include <QApplication>
#include <sstream>


#define DEFAULT_A (-5.0)
#define DEFAULT_B (5.0)
#define DEFAULT_N (10)
#define DEFAULT_K (0)



static
int parse_command_line (int argc, char **argv, double *pa, double *pb, int *pn, int *pk)
{
    if (argc == 1)
        return 0;

    if (argc != 5)
    {
        fprintf (stderr, "Wrong number of arguments!\n");
        return -1;
    }

    double a, b;
    int n, k;

	std::stringstream ss;
	for (int i = 1; i < argc; ++i)
		ss << argv[1] << " ";

    if (!(ss >> a))
    {
        fprintf (stderr, "Invalid value for a!\n");
        return -1;
    }

    if (!(ss >> b))
    {
        fprintf (stderr, "Invalid value for b!\n");
        return -1;
    }

    if (!(ss >> n))
    {
        fprintf (stderr, "Invalid value for n!\n");
        return -1;
    }
 
 	if (!(ss >> k))
    {
        fprintf (stderr, "Invalid value for k!\n");
        return -1;
    }


    double segment_length = b - a;
    if (segment_length < EPSILON_FOR_COMPARE)
    {
        fprintf (stderr, "Too short segment: %lf\n", segment_length);
        return -1;
    }

    if (n <= 0)
    {
        fprintf (stderr, "Non-positive number of points!\n");
        return -1;
    }
    if (k <= 0)
    {
        fprintf (stderr, "Non-positive number of function!\n");
        return -1;
    }


    *pa = a;
    *pb = b;
    *pn = n;
	*pk = k;

    return 0;
}

int main (int argc, char **argv) {
	double a = DEFAULT_A;
	double b = DEFAULT_B;
	int n = DEFAULT_N;
	int k = DEFAULT_K;

	int err = parse_command_line (argc, argv, &a, &b, &n, &k);
	if (err < 0)
		return -1;

	QApplication app (argc, argv);

	MainWindow w(a, b, n, k);

	w.show();
	return app.exec();
}
