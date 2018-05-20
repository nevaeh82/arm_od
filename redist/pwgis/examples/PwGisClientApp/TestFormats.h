#ifndef TESTFORMATS_H
#define TESTFORMATS_H

#include <QDialog>

namespace Ui {
class TestFormats;
}

class TestFormats : public QDialog
{
	Q_OBJECT

public:
	explicit TestFormats(QWidget *parent = 0);
	~TestFormats();

private slots:

	void loadKmlWorldMap();

	void loadIngitMap();
	void loadInvalidAbakanMap();
	void loadIngitWorldMap();
	void loadIngitRussiaMap();
	void loadInvalidIngitMap();

	void testSxf();
	void testSit();
	void testMapPanorama();

	void loadMapinfoSaintPetersburgMap();
	void loadMapinfoMoscowMap();
	void loadMapinfoNizhnyNovgorodMap();
	void loadMapinfoMoscowUtmWgs84Map();
	void loadMapinfoWorldMap41001();
	void loadMapinfoRussiaMap41001();
	void loadMapinfoWorldMapQuasi3857();
	void loadMapinfoRussiaMapQuasi3857();
	void loadMapinfoTestM180P180();
	void loadKml();


private:
	Ui::TestFormats *ui;

	void openMap( QString mapfile );
};

#endif // TESTFORMATS_H
