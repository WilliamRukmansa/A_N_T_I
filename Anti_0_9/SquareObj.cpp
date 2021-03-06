#include "SquareObj.hpp"

SquareObj::SquareObj(float x, float y, float side, float dtime) : Object2D(x, y, 4, dtime) {
	radius = side;
	this->redraw();
}
SquareObj::SquareObj(float x, float y, float side, float dtime, float mtime) : Object2D(x, y, 4, dtime, mtime) {
	radius = side;
	this->redraw();
}
SquareObj::SquareObj(float x, float y, float side, float angle, float dtime, float mtime) : Object2D(x, y, 4, angle, dtime, mtime) {
	radius = side;
	this->redraw();
}
SquareObj::~SquareObj() {
}
float SquareObj::getRadius() const {
	return radius;
}
void SquareObj::setRadius(float r) {
	radius = r;
}
void SquareObj::redraw() {
	pointData[0] = widthConvert(getX() - radius*cos(convertToRadian(angle+45))/2);
	pointData[1] = heightConvert(getY() + radius*sin(convertToRadian(angle+45))/2);
	
	pointData[2] = widthConvert(getX() - radius*sin(convertToRadian(angle+45))/2);
	pointData[3] = heightConvert(getY() - radius*cos(convertToRadian(angle+45))/2);
	
	pointData[4] = widthConvert(getX() + radius*cos(convertToRadian(angle+45))/2);
	pointData[5] = heightConvert(getY() - radius*sin(convertToRadian(angle+45))/2);
	
	pointData[6] = widthConvert(getX() + radius*sin(convertToRadian(angle+45))/2);
	pointData[7] = heightConvert(getY() + radius*cos(convertToRadian(angle+45))/2);
}
