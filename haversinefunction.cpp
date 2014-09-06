#include <assert.h>
#include <iostream>
#include <cmath>
using namespace std;

//theta must be in radians
double haversine( double theta )
{
	return ( 1.0 - cos( theta ) ) / 2.0;
}

//Longitude and Latitude values must be in radians
double haversineDistance( double long1, double lat1, double long2, double lat2 )
{
	const double eRadius = 6371.0;	//radius of the earth in km (approximation)
	double dist;

	double longHaversine = haversine( long2 - long1 );
	double latHaversine = haversine( lat2 - lat1 );

	//cout << "Long/Lat Haversine: ( " << longHaversine << ", " << latHaversine << " )" << endl;

	double h = latHaversine + cos( lat1 ) * cos ( lat2 ) * longHaversine;

	//Avoid undefined behavior when calculating distance (distance is only real on the interval h = [0, 1])
	if ( h > 1.0 )
	{
		h = 1.0;
	}
	else if ( h < 0.0 )
	{
		h = 0.0;
	}

	dist = 2.0 * eRadius * asin( sqrt( h ) );
	
	return dist;
}

//Latitude and longitude values must be given in decimal degree form, not seconds and minutes
//Returns 2 if the detour distance of driver 2 picking up and dropping off driver 1 is shorter
//Returns 1 if the detour distance of driver 1 picking up and dropping off driver 2 is shorter or if the two distances are equal
//Returns 0 if the two detour distances are equal
int calcShorterDetourDistance( double pointALong, double pointALat, double pointBLong, double pointBLat,
								double pointCLong, double pointCLat, double pointDLong, double pointDLat )
{
	const double radianConversion = atan2(0, -1) / 180;

	//Convert degrees to radians
	pointALong = pointALong * radianConversion;
	pointALat = pointALat * radianConversion;
	pointBLong = pointBLong * radianConversion;
	pointBLat = pointBLat * radianConversion;
	pointCLong = pointCLong * radianConversion;
	pointCLat = pointCLat * radianConversion;
	pointDLong = pointDLong * radianConversion;
	pointDLat = pointDLat * radianConversion;

	//Driver1 starts at point A and ends at point B
	//Driver2 starts at point C and ends at point D
	double distACDB;	//detour distance 1 (driver 1 picks up and drops off driver 2)
	double distCABD;	//detour distance 2 (driver 2 picks up and drops off driver 1)
	
	double distAC = haversineDistance( pointALong, pointALat, pointCLong, pointCLat );
	double distCD = haversineDistance( pointCLong, pointCLat, pointDLong, pointDLat );
	double distDB = haversineDistance( pointDLong, pointDLat, pointBLong, pointBLat );
	distACDB = distAC + distCD + distDB;
	
	double distCA = distAC;
	double distAB = haversineDistance( pointALong, pointALat, pointBLong, pointBLat );
	double distBD = distDB;
	distCABD = distCA + distAB + distBD;
	
	if ( distACDB > distCABD )
	{
		cout << "The detour distance of driver 2 picking up and dropping off driver 1 is shorter with a distance of " << distCABD << " km compared to " << distACDB << " km" << endl;
		return 2;
	}
	else if ( distACDB < distCABD )
	{
		cout << "The detour distance of driver 1 picking up and dropping off driver 2 is shorter with a distance of " << distACDB << " km compared to " << distCABD << " km" << endl;
		return 1;
	}
	else
	{
		cout << "The two detour distances are the same with a value of " << distACDB << endl;
		return 0;
	}
}


void test_haversineDistance()
{
	const double errorMargin = 0.01;
	const double radianConversion = atan2(0, -1) / 180;

	//Oakland to random point
	double lat1 = 37.804444 * radianConversion;
	double long1 = -122.270833 * radianConversion;
	double lat2 = 38.42953815 * radianConversion;
	double long2 = -124.56144015 * radianConversion;
	double expectedDist = 212.1536;
	double actualDist = haversineDistance( long1, lat1, long2, lat2 );
	assert( actualDist >= ( expectedDist * ( 1.0 - errorMargin ) ) && actualDist <= ( expectedDist * ( 1.0 + errorMargin ) ) );
	cout << "expectedDist: " << expectedDist << endl;
	cout << "actualDist: " << actualDist << endl;

	//San Francisco to random point
	lat1 = 37.783333 * radianConversion;
	long1 = -122.416667 * radianConversion;
	lat2 = 37.32750937 * radianConversion;
	long2 = -126.48310112 * radianConversion;
	expectedDist = 362.0991;
	actualDist = haversineDistance( long1, lat1, long2, lat2 );
	assert( actualDist >= ( expectedDist * ( 1.0 - errorMargin ) ) && actualDist <= ( expectedDist * ( 1.0 + errorMargin ) ) );
	cout << "expectedDist: " << expectedDist << endl;
	cout << "actualDist: " << actualDist << endl;

	//Sacramento to random point
	lat1 = 38.555556 * radianConversion;
	long1 = -121.468889 * radianConversion;
	lat2 = 37.69306415 * radianConversion;
	long2 = -116.64660595 * radianConversion;
	expectedDist = 432.6543;
	actualDist = haversineDistance( long1, lat1, long2, lat2 );
	assert( actualDist >= ( expectedDist * ( 1.0 - errorMargin ) ) && actualDist <= ( expectedDist * ( 1.0 + errorMargin ) ) );
	cout << "expectedDist: " << expectedDist << endl;
	cout << "actualDist: " << actualDist << endl;

	//Alameda to random point
	lat1 = 37.756111 * radianConversion;
	long1 = -122.274444 * radianConversion;
	lat2 = 36.71769119 * radianConversion;
	long2 = -123.7362221 * radianConversion;
	expectedDist = 173.4764;
	actualDist = haversineDistance( long1, lat1, long2, lat2 );
	assert( actualDist >= ( expectedDist * ( 1.0 - errorMargin ) ) && actualDist <= ( expectedDist * ( 1.0 + errorMargin ) ) );
	cout << "expectedDist: " << expectedDist << endl;
	cout << "actualDist: " << actualDist << endl;

	return;
}

void test_calcShorterDetourDistance()
{
	//Oakland to random point
	double pAlat = 37.804444;
	double pAlong = -122.270833;
	double pBlat = 38.42953815;
	double pBlong = -124.56144015;

	//San Francisco to random point
	double pClat = 37.783333;
	double pClong = -122.416667;
	double pDlat = 37.32750937;
	double pDlong = -126.48310112;

	int shorterDetourDistance = calcShorterDetourDistance(pAlong, pAlat, pBlong, pBlat, pClong, pClat, pDlong, pDlat);

	return;
}

int main()
{
	test_haversineDistance();

	test_calcShorterDetourDistance();

	return 0;
}

