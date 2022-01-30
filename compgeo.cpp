#include "compgeo.h"

Point2D points[MAXPOINTS];
Line2D lines[MAXPOINTS/2];
Polygon2D polygons[MAXPOINTS/3];
size_t numpoints = 0;
size_t numlines = 0;
size_t numpolygons = 0;

geotype adding=none;
size_t adding_start=0;
size_t adding_end=0;

void AddNewPoint(int x, int y){
	points[numpoints].x = x;
	points[numpoints].y = y;
	numpoints++;
	adding_end=numpoints-1;
}

void AddNewLine(size_t a, size_t b){
	lines[numlines].a = points[a];
	lines[numlines].b = points[b];
	numlines++;
}

void AddNewConvexPolygon(size_t start, size_t end){
	polygons[numpolygons] = ConvexHull(&points[start], end-start+1);
	numpolygons++;
}

double Sine(Point2D p1, Point2D p2, Point2D p3){
	int adx = p2.x - p1.x;
	int ady = p1.y - p2.y;
	int bdx = p3.x - p1.x;
	int bdy = p1.y - p3.y;
	return atan2(bdy, bdx) - atan2(ady, adx);
}

double Cross(Point2D p1, Point2D p2, Point2D p3){
	int adx = p2.x - p1.x;
	int ady = p1.y - p2.y;
	int bdx = p3.x - p1.x;
	int bdy = p1.y - p3.y;
	return (adx*bdy - ady*bdx);
}

void ClearGeometry(){
	for(int i = 0; i < numpolygons; i++){
		if(polygons[i].points){
			free( polygons[i].points );
			polygons[i].points = NULL;
		}
	}
	numpoints = 0;
	numpolygons = 0;
	numlines = 0;
	adding_start = 0;
	adding_end = 0;
}

int amodb(int a, int b){
	int r = a%b;
	return r < 0 ? r + b : r;
}

void AddGeometry(enum geotype newgeo){
	switch(newgeo){
	case line:
		if(adding_end){
			adding_start=adding_end+1;
			adding_end++;
		}
		AddNewLine(adding_start, adding_end);
		adding=newgeo;
		break;
	case polygon:
		adding_start=adding_end+1;
		adding_end++;
		AddNewConvexPolygon(adding_start, adding_end);
		adding=newgeo;
		break;
	case point:
	case none:
	default:
		return;
		break;
	}	
}

void UpdateGeometry(){
	switch(adding){
	case line:
		lines[numlines-1].b = points[adding_end];
		break;
	case polygon:
		polygons[numpolygons-1] = ConvexHull(&points[adding_start], adding_end - adding_start + 1);
		break;
	default:
		break;
	}
}
