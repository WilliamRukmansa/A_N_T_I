///Compiling shortcut (Linux): g++ -o message message.cpp -lGL -lGLU -lglfw ///
#include <GLFW/glfw3.h>
#include <iostream>
#include "randomer.hpp"
using namespace std;

#include <fstream>
#include "Lists.hpp"
#include "Time.hpp"
#include <ctime>
#include "Letter.hpp"
#include "Spawner2D.hpp"
#include "SquareObj.hpp"
#include "QuadObj.hpp"
#include "TriangleObj.hpp"
#include "PatternObj.hpp"

///Global variables
int linesize;
double WINDOW_HEIGHT; //tested on 1000+ pixels
double WINDOW_WIDTH; //tested on 600+ pixels
const float CURSOR_SIZE = 4;

const float sqrtTwo = sqrt(2);

const float CURSOR_DIAG = CURSOR_SIZE*sqrtTwo;

//Game variables:
double xpos, ypos;
double newxpos, newypos;

Time timeeffect;
Time worldtime[5];
int worldn = 0;
Time delaytime;
bool disabled = false;
bool freeze = false;
bool gameover = false;
int checkl = 0;
bool spacepressed = false;

float SHOT_DELAY;
const float SHOT_DELAYA = 0.065;
const float SHOT_DELAYB = 0.105;
const float SHOT_DELAYC = 0.085;
const float SHOT_DELAYD = 0.365;

float SHOT_DAMAGE;
const float SHOT_DAMAGEA = 2;
const float SHOT_DAMAGEB = 1.5;
const float SHOT_DAMAGEC = 1;
const float SHOT_DAMAGED = 3;
const float SHOT_THRESHOLD = 60;
int bombcount;
bool typeB;
bool typeC;
bool typeD;
bool confirmcheck = false;

bool storymode = false;
bool autoshoot = false;
int rounds = -1;

//Texts but user too lazy to make a typedef struct instead lol
float pausetext[1000];
int pausemark[3];
const string pause1 = "PAUSED";
const string pause2 = "Press P to resume";

float gameovertext[1200];
int gameovermark[4];
const string gameover1 = "GAME OVER";

float mainmenutext[3000];
int mainmenumark[7];
const string mainmenu1 = "A N T I";
const string mainmenu2 = "Press S to start NEW game.";
const string mainmenu3 = "Press L to load checkpoint.";
//const string mainmenu4 = "Press A to start arcade mode.";
const string mainmenu4 = "Press A to start Beta Test.";
const string mainmenu5 = "Press H for help. Press Esc to quit.";
const string mainmenu6 = "Sure?";

float shottypetext[1500];
int shottypemark;
const string shottype_str = "Press W to switch weapons: ";
const string typeA_str = "Laser";
const string typeB_str = "Spread";
const string typeC_str = "Blitz";
const string typeD_str = "Echo";

float timetext[1500];
int timemark[2];

float wintext[1600];
int winmark[4];
const string wintext1 = "END OF GAME";
const string wintext2 = "Thanks for playing";
const string wintext3 = "Hold spacebar and keep pressing W in main menu...";

float helptext[9300];
int helpmark[10];
const string help1 = "Dodge all obstacles. Move with mouse.";
const string help2 = "Hold spacebar to shoot enemies. Press T to toggle autoshoot.";
const string help3 = "Press X to use bomb. You can carry MAXIMUM 2 bombs at a time.";
const string help4 = "Press B to toggle health bar display.";
const string help5 = "It is up to you to change your mouse sensitivity.";
const string help6 = "This game may not be completable by everyone. Stay calm.";
const string help7 = "In story mode: Bomb count will replenish after cutscenes.";
//const string help8 = "In arcade mode: Press P to pause. One bomb added every 10 rounds.";
const string help8 = "In beta mode: Press P to pause. One bomb added in every round.";
const string help9 = "Press anything on the keyboard to return to main menu.";


///Title texts
float title[500];
int titlemark;

const string title_LevelA_str = "Level A";
const string title_RapidShooter_str = "Rapid Shooter";
const string title_Danmaku1_str = "Danmaku Spiral";
const string title_DeathHearts_str = "Deadly Hearts";
const string title_RingShooter_str = "Ring Shooter";
const string title_Drone_str = "Droned Fighter";
const string title_Chaser_str = "Chaser";
const string title_Rippler_str = "Ripples";
const string title_Rain_str = "Bullet Rain";
const string title_LaserBloom_str = "Laser Bloom";
const string title_Radiance_str = "Radiance";
const string title_SpikeCage_str = "Spike Cage";
const string title_IceSpirit_str = "Ice Spirit";
const string title_EarthSpirit_str = "Earth Spirit";
const string title_Danmaku2_str = "Danmaku Fireworks";
const string title_PetalBarrage_str = "Petal Barrage";
const string title_Dasher_str = "Dasher";
const string title_LevelB_str = "Level B";
const string title_LevelC_str = "Level C";
const string title_AstralCage_str = "Astral Cage";
const string title_TimeSorcerer_str = "Time Sorcerer";
const string title_SpiralMadness_str = "Spiral Madness";
const string title_ArrowMaster_str = "Arrow Master";

///Story texts
const int STORY_LINESIZE = 6;
float story1text[1000];
int story1mark[3];
const string story1_str1 = "They are after me.";
const string story1_str2 = "I must run or fight.";

float story2text[500];
int story2mark[2];
const string story2_str1 = "They keep coming...";

float story3text[500];
int story3mark[2];
const string story3_str1 = "I am their target.";

float story4text[500];
int story4mark[2];
const string story4_str1 = "I will survive.";

float story5text[500];
int story5mark[2];
const string story5_str1 = "Their attacks get harder...";

float story6text[1000];
int story6mark[3];
const string story6_str1 = "Noone can stop me.";
const string story6_str2 = "I will destroy all.";

float story7text[500];
int story7mark[2];
const string story7_str1 = "When will this end?";

float story8text[500];
int story8mark[2];
const string story8_str1 = "Find the source.";

float story9text[500];
int story9mark[2];
const string story9_str1 = "This must be it...";

float story10text[100];
int story10mark[2];
const string story10_str1 = ". . .";

GLfloat borders[8] = {-1.0,-1.0, -1.0,1.0, 1.0,1.0, 1.0,-1.0};
GLfloat cursorpoint[16];
int gamemode = 0;

///Objects
List<SquareObj> Sq;
List<TriangleObj> Tr;
List<QuadObj> Qu;
List<Object2D*> Shots;

List<Spawner2D> defSpw;
List<Spawner2D> subSpw;
List<Spawner2D> Spw;
List<PatternObj> Ptt;

bool enableBar;
QuadObj healthBar(0, 0, 0, 0, 0);

//Helper procedure
void spawnerClear(List<Spawner2D>& Li) {
	Li.setBegin();
	for (int i = 0; i < Li.size(); i++) {
		delete Li.getIValue().getObject();
		Li.next();
	}
	Li.removeAll();
}
void patternClear(List<PatternObj>& Li) {
	Li.setBegin();
	for (int i = 0; i < Li.size(); i++) {
		delete[] Li.getIValue().getObjData();
		Li.next();
	}
	Li.removeAll();
}
void shotsClear(List<Object2D*>& Li) {
	Li.setBegin();
	for (int i = 0; i < Li.size(); i++) {
		delete Li.getIValue();
		Li.next();
	}
	Li.removeAll();
}

/** Making levels:
 * add into level difficulty
 * createLevel()
 * make level in main()
 * 
 * specific levels:
 * drawTitle
 * save zone
 * **/
 
///Stages
const int ID_GAMEOVER = -1;
const int ID_MAINMENU = 0;
const int ID_HELP = 998;
const int ID_WIN = 999;

//Story
const int ID_STORY1 = 997;
const int ID_STORY2 = 996;
const int ID_STORY3 = 995;
const int ID_STORY4 = 994;
const int ID_STORY5 = 993;
const int ID_STORY6 = 992;
const int ID_STORY7 = 991;
const int ID_STORY8 = 990;
const int ID_STORY9 = 989;
const int ID_STORY10 = 988;

const int ZONE_LIMIT = 987;
const int ZONE_TEST = 986;

const int LEVEL_LIMIT = 800;

const int ID_ZONE6 = 10;

//Easy
const int ID_ZONE1 = 1;
const int ID_ZONE2 = 2;
const int ID_ZONE3 = 3;
const int ID_SHOOTER1 = 6;
const int ID_ZONE4 = 8;
const int ID_ZONE5 = 9;
const int ID_ZONE6B = 11;
const int ID_SHOOTER2 = 12;
const int ID_ZONE7 = 13;
const int ID_SHOOTER3 = 14;
const int ID_SHOOTER5 = 16;
const int ID_ZONE8 = 21;

const int ID_RAPIDSHOOTER = 801;
const int ID_DRONEDFIGHTER = 806;
const int ID_CHASER = 807;
const int ID_LEVEL_B = 818;

//Medium
const int ID_ZONE1B = 4;
const int ID_BULLET1 = 5;
const int ID_SHOOTER4 = 15;
const int ID_ZONE2B = 17;
const int ID_SHOOTER7 = 20;
const int ID_SHOOTER8 = 24;

const int ID_LEVEL_A = 802;
const int ID_RINGSHOOTER = 804;
const int ID_DEADLYHEARTS = 805;
const int ID_RIPPLES = 808;
const int ID_LASERBLOOM = 810;
const int ID_SPIKECAGE = 812;
const int ID_DASHER = 817;
const int ID_TIMESORCERER = 821;

//Hard
const int ID_BULLET2 = 7;
const int ID_ZONE7B = 19;
const int ID_ZONE9 = 22;
const int ID_ZONE10 = 23;
const int ID_SHOOTER8B = 25;

const int ID_DANMAKU1 = 803;
const int ID_RADIANCE = 811;
const int ID_ICESPIRIT = 813;
const int ID_EARTHSPIRIT = 814;
const int ID_DANMAKU2 = 815;
const int ID_PETAL = 816;
const int ID_LEVEL_C = 819;
const int ID_ARROWMASTER = 823;

//Touhou (ez modo)
const int ID_SHOOTER6 = 18;

const int ID_RAIN = 809;
const int ID_ASTRALCAGE = 820;
const int ID_SPIRALMADNESS = 822;

List<int> changeID;
List<int> levelID;

void setZEasy() {
	changeID.removeAll();
	changeID.add(ID_ZONE1);
	changeID.add(ID_ZONE2);
	changeID.add(ID_SHOOTER1);
	changeID.add(ID_ZONE3);
	changeID.add(ID_ZONE4);
	changeID.add(ID_ZONE5);
	changeID.add(ID_ZONE6B);
	changeID.add(ID_SHOOTER2);
	changeID.add(ID_ZONE7);
	changeID.add(ID_SHOOTER3);
	changeID.add(ID_SHOOTER5);
	changeID.add(ID_ZONE8);
}

void setLEasy() {
	levelID.removeAll();
	levelID.add(ID_RAPIDSHOOTER);
	levelID.add(ID_DRONEDFIGHTER);
	levelID.add(ID_CHASER);
	levelID.add(ID_LEVEL_B);
}

void setZNormal() {
	changeID.removeAll();
	changeID.add(ID_BULLET1);
	changeID.add(ID_ZONE1B);
	changeID.add(ID_ZONE2B);
	changeID.add(ID_SHOOTER4);
	changeID.add(ID_SHOOTER7);
	changeID.add(ID_SHOOTER8);
}

void setLNormal() {
	levelID.removeAll();
	levelID.add(ID_LEVEL_A);
	levelID.add(ID_RINGSHOOTER);
	levelID.add(ID_DEADLYHEARTS);
	levelID.add(ID_RIPPLES);
	levelID.add(ID_LASERBLOOM);
	levelID.add(ID_SPIKECAGE);
	levelID.add(ID_DASHER);
	levelID.add(ID_TIMESORCERER);
}

void setZHard() {
	changeID.removeAll();
	changeID.add(ID_BULLET2);
	changeID.add(ID_ZONE7B);
	changeID.add(ID_ZONE9);
	changeID.add(ID_ZONE10);
	changeID.add(ID_SHOOTER8B);
}

void setLHard() {
	levelID.removeAll();
	levelID.add(ID_DANMAKU1);
	levelID.add(ID_RADIANCE);
	levelID.add(ID_ICESPIRIT);
	levelID.add(ID_EARTHSPIRIT);
	levelID.add(ID_DANMAKU2);
	levelID.add(ID_PETAL);
	levelID.add(ID_LEVEL_C);
	levelID.add(ID_ARROWMASTER);
}

void setZTouhou() {
	changeID.removeAll();
	changeID.add(ID_SHOOTER6);
}

void setLTouhou() {
	levelID.removeAll();
	levelID.add(ID_RAIN);
	levelID.add(ID_ASTRALCAGE);
	levelID.add(ID_SPIRALMADNESS);
}

int patterner;
Point pointmark;

/* convert pixel to openGL coordinates */
void convertPointData(float *arr, int start, int end) {
	for (int i = start; i < end; i += 2) {
		arr[i] = widthConvert(arr[i]);
	}
	for (int i = start+1; i < end; i += 2) {
		arr[i] = heightConvert(arr[i]);
	}
}

///Pattern shots
const int PATTERN_DEFAULT = 0;
const int PATTERN_TWINWAVE1 = 1; //DronedFighter
const int PATTERN_ROTATEC = 2;
const int PATTERN_ROTATEAC = 3;
const int PATTERN_TWINWAVE2 = 4; //Radiance
const int PATTERN_SLINGSHOT = 5;
const int PATTERN_SLINGSHOT2 = 6; ///Use this pls
const int PATTERN_ROTATEC2 = 7;
const int PATTERN_REDIRECT1 = 8; //ArrowMaster
const int PATTERN_FALL1 = 9;
const int PATTERN_OPPOSITE1 = 10;

//Pattern helper
void PatternObj::move() {
	setX(getX() + speed*cos(convertToRadian(direction)));
	setY(getY() - speed*sin(convertToRadian(direction)));
	switch (patterntype) {
		case PATTERN_DEFAULT: {
			for (int i = 0; i < n; i++) {
				if (objData[i] != nullptr) {
					objData[i]->setX(objData[i]->getX() + speed*cos(convertToRadian(direction)));
					objData[i]->setY(objData[i]->getY() - speed*sin(convertToRadian(direction)));
				}
			}
		} break;
		case PATTERN_TWINWAVE1: {
			for (int i = 0; i < n; i++) {
				if (objData[i] != nullptr) {
					objData[i]->setX(objData[i]->getX() + speed*cos(convertToRadian(direction)));
					objData[i]->setY(objData[i]->getY() - speed*sin(convertToRadian(direction)));
					objData[i]->setSpeed(8*(sin(objData[i]->getTimeElapsed()/1.5 + convertToRadian((float)i*360/n))));
					objData[i]->setAngle(360*objData[i]->getTimeElapsed());
				}
			}
		} break;
		case PATTERN_TWINWAVE2: {
			for (int i = 0; i < n; i++) {
				if (objData[i] != nullptr) {
					objData[i]->setX(objData[i]->getX() + speed*cos(convertToRadian(direction)));
					objData[i]->setY(objData[i]->getY() - speed*sin(convertToRadian(direction)));
					objData[i]->setSpeed(8*(cos(7*objData[i]->getTimeElapsed())));
				}
			}
		} break;
		case PATTERN_ROTATEC: {
			rotate(-0.9);
			for (int i = 0; i < n; i++) {
				if (objData[i] != nullptr) {
					objData[i]->setX(objData[i]->getX() + speed*cos(convertToRadian(direction)));
					objData[i]->setY(objData[i]->getY() - speed*sin(convertToRadian(direction)));
				}
			}
		} break;
		case PATTERN_ROTATEC2: {
			rotate(-2.6);
			for (int i = 0; i < n; i++) {
				if (objData[i] != nullptr) {
					objData[i]->setX(objData[i]->getX() + speed*cos(convertToRadian(direction)));
					objData[i]->setY(objData[i]->getY() - speed*sin(convertToRadian(direction)));
				}
			}
		} break;
		case PATTERN_ROTATEAC: {
			rotate(0.9);
			for (int i = 0; i < n; i++) {
				if (objData[i] != nullptr) {
					objData[i]->setX(objData[i]->getX() + speed*cos(convertToRadian(direction)));
					objData[i]->setY(objData[i]->getY() - speed*sin(convertToRadian(direction)));
				}
			}
		} break;
		case PATTERN_SLINGSHOT: {
			rotate(1);
			for (int i = 0; i < n; i++) {
				if (objData[i] != nullptr) {
					objData[i]->setX(objData[i]->getX() + speed*cos(convertToRadian(direction)) - (objData[i]->getSpeed()*7/5)*cos(convertToRadian(objData[i]->getDirection())));
					objData[i]->setY(objData[i]->getY() - speed*sin(convertToRadian(direction)) + (objData[i]->getSpeed()*7/5)*sin(convertToRadian(objData[i]->getDirection())));
				}
			}
		} break;
		case PATTERN_SLINGSHOT2: {
			for (int i = 0; i < n; i++) {
				if (objData[i] != nullptr) {
					objData[i]->setX(objData[i]->getX() + speed*cos(convertToRadian(direction)) - (objData[i]->getSpeed()*7/5)*cos(convertToRadian(objData[i]->getDirection())));
					objData[i]->setY(objData[i]->getY() - speed*sin(convertToRadian(direction)) + (objData[i]->getSpeed()*7/5)*sin(convertToRadian(objData[i]->getDirection())));
				}
			}
		} break;
		case PATTERN_REDIRECT1: {
			bool redir = false;
			for (int i = 0; i < n; i++) {
				if (objData[i] != nullptr) {
					objData[i]->setX(objData[i]->getX() + speed*cos(convertToRadian(direction)));
					objData[i]->setY(objData[i]->getY() - speed*sin(convertToRadian(direction)));
					if (!redir) {
						if (objData[i]->getColorData()[2] < 0.1 && objData[i]->getTimeElapsed() > 1.0) {
							redir = true;
							float ang = -this->angleTo(Point(xpos, ypos));
							rotate(ang - this->getDirection());
							this->setDirection(ang);
							objData[i]->setAngle(ang);
							objData[i]->getColorData()[2] = 0.4;
						}
					} else {
						objData[i]->setAngle(-this->angleTo(Point(xpos, ypos)));
						objData[i]->getColorData()[2] = 0.4;
					}
				}
			}
		} break;
		case PATTERN_FALL1: {
			bool foundone = false;
			float elapsedtime;
			for (int i = 0; i < n; i++) {
				if (objData[i] != nullptr) {
					if (!foundone) {
						foundone = true;
						elapsedtime = objData[i]->getTimeElapsed();
					}
					objData[i]->setX(objData[i]->getX() + speed*cos(convertToRadian(direction)));
					objData[i]->setY(objData[i]->getY() - speed*sin(convertToRadian(direction)) + elapsedtime*(float)(i/8 + 1));
				}
			}
		} break;
		case PATTERN_OPPOSITE1: {
			for (int i = 0; i < n; i++) {
				if (objData[i] != nullptr) {
					if (i == 0)
						objData[i]->setX(objData[i]->getX() + speed*cos(convertToRadian(direction)));
					else
						objData[i]->setX(objData[i]->getX() - speed*cos(convertToRadian(direction)));
					objData[i]->setY(objData[i]->getY() - speed*sin(convertToRadian(direction)));
				}
			}
		} break;
	}
}

///Spawn shots
const int NULL_SHOT = 0;
const int SQUARE_ANGLESHOT1 = 1;
const int SQUARE_ANGLESHOT2 = 2;
const int SQUARE_ANGLESHOT3 = 3;
const int QUAD_ANGLESHOT1 = 4;
const int SQUARE_RAPIDSHOT1 = 5; //RapidShooter
const int SQUARE_AIMSHOT1 = 6;
const int SQUARE_AIMSHOT2 = 7;
const int SQUARE_SPECIAL1 = 8; //Danmaku1
const int SQUARE_RANDOMSHOT1 = 9; //Danmaku1 (random Angle)
const int SQUARE_SPECIAL2 = 10; //RingShooter
const int TRIANGLE_SPECIAL5 = 11; //DeadlyHearts
const int SQUARE_ANGLESHOT4 = 12;
const int QUAD_ANGLESHOT2 = 13;
const int SQUARE_SPECIAL4 = 14; //DronedFighter
const int SQUARE_RAPIDSHOT2 = 15; //Chaser
const int SQUARE_ROTATEC = 16;
const int SQUARE_ROTATEAC = 17;
const int SQUARE_AIMSHOT3 = 18;
const int QUAD_SPECIAL1 = 19; //Rain
const int TRIANGLE_SPECIAL1 = 20; //Rain
const int QUAD_SPECIAL2 = 21; //LaserBlaze
const int SQUARE_SPECIAL5 = 22; //Radiance
const int TRIANGLE_SPECIAL2 = 23; //SpikeCage
const int TRIANGLE_SPECIAL3 = 24; //IceSpirit
const int SQUARE_SPECIAL6 = 25; //IceSpirit
const int TRIANGLE_RANDOMSHOT1 = 26; //EarthSpirit
const int SQUARE_AIMSHOT4 = 27;
const int SQUARE_RANDOMSHOT2 = 28; //(Random Speed)
const int SQUARE_SPECIAL7 = 29; //Danmaku2
const int SQUARE_SPECIAL8 = 30; //PetalBarrage
const int TRIANGLE_SPECIAL4 = 31; //Dasher
const int TRIANGLE_RANDOMSHOT2 = 32;
const int SQUARE_ANGLESHOT5 = 33;
const int SQUARE_LASER1 = 34; ///lasers
const int SQUARE_SPECIAL9 = 35; //AstralCage
const int SQUARE_SPECIAL10 = 36; //TimeSorcerer
const int QUAD_SPECIAL3 = 37;
const int SQUARE_ANGLESHOT6 = 39; ///90 degrees
const int SQUARE_ANGLESHOT6B = 40; ///90 degrees
const int SQUARE_ANGLESHOT6C = 41; ///90 degrees
const int QUAD_SPECIAL4 = 42; //ArrowMaster
const int SQUARE_SPECIAL11 = 43;

const int LBOUND_EXPLOSION = 80;
const int SQUARE_EXPLODE1 = 81;
const int SQUARE_EXPLODE2 = 82;
const int SQUARE_EXPLODE3 = 83;

void Spawner2D::spawnObject() {
	switch(spawntype) {
		case SQUARE_ANGLESHOT1 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 40, 7));
			Sq[0].setDirection(spawner->getAngle());
			Sq[0].setSpeed(6);
			Sq[0].setAngle(spawner->getAngle());
			Sq[0].getColorData()[2] = 0.0;
		} break;
		case SQUARE_ANGLESHOT2 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 60, 7));
			Sq[0].setSpeed(4);
			Sq[0].setAngle(spawner->getAngle());
			Sq[0].setDirection(spawner->getAngle());
			Sq[0].getColorData()[2] = randomFloat(0, 1);
		} break;
		case SQUARE_ANGLESHOT3 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 50, 6));
			Sq[0].setSpeed(6);
			Sq[0].setAngle(spawner->getAngle());
			Sq[0].setDirection(spawner->getAngle());
			Sq[0].getColorData()[1] = randomFloat(0, 1);
			Sq[0].getColorData()[2] = randomFloat(0, 1);
		} break;
		case SQUARE_SPECIAL2 : {
			for (int i = 0; i < 24; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 20, 13));
				Sq[0].setSpeed(3 + 1.5*cos(convertToRadian((float)i/24 * 360)));
				Sq[0].setAngle((float)i/24*360);
				Sq[0].setDirection(spawner->getAngle() + 40*sin(convertToRadian((float)i/24*360)));
				Sq[0].getColorData()[1] = 0.0;
			}
		} break;
		case SQUARE_ANGLESHOT4 : {
			Ptt.add(PatternObj(spawner->getX(), spawner->getY(), 36, PATTERN_DEFAULT));
			Ptt[0].setSpeed(2);
			Ptt[0].setDirection(spawner->getAngle());
			for (int i = 0; i < 36; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 20, randomFloat(3.0,4.5) - randomFloat(0.0, 2.5)));
				Sq[0].setSpeed(1.5);
				Sq[0].setAngle((float)i*10);
				Sq[0].setDirection((float)i*10);
				Sq[0].getColorData()[0] = 0.5;
				Sq[0].getColorData()[1] = 0.5;
				Ptt[0].add(&Sq[0]);
			}
		} break;
		case SQUARE_ANGLESHOT5 : {
			subSpw.add(Spawner2D(new SquareObj(spawner->getX(), spawner->getY(), 110, 2), TYPE_SQUARE, 999, 1, SQUARE_EXPLODE3));
			subSpw[0].getObject()->setSpeed(randomFloat(2, 6));
			subSpw[0].getObject()->setAngle(spawner->getAngle());
			subSpw[0].getObject()->setDirection(spawner->getAngle());
			subSpw[0].getObject()->getColorData()[0] = 0.9;
			subSpw[0].getObject()->getColorData()[1] = 0.9;
			subSpw[0].getObject()->getColorData()[2] = 0.9;
		} break;
		case SQUARE_ANGLESHOT6 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 45, 8));
			Sq[0].setSpeed(3.5);
			Sq[0].setAngle(spawner->getAngle());
			Sq[0].setDirection(Sq[0].getAngle());
			Sq[0].getColorData()[1] = 0.0;
		} break;
		case SQUARE_ANGLESHOT6B : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 32, 15));
			Sq[0].setSpeed(2.5);
			Sq[0].setAngle(spawner->getAngle());
			Sq[0].setDirection(Sq[0].getAngle());
			Sq[0].getColorData()[1] = 0.0;
		} break;
		case SQUARE_ANGLESHOT6C : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 20, 21));
			Sq[0].setSpeed(2);
			Sq[0].setAngle(spawner->getAngle());
			Sq[0].setDirection(Sq[0].getAngle());
			Sq[0].getColorData()[1] = 0.0;
		} break;
		case SQUARE_LASER1 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 60, 6));
			Sq[0].setSpeed(7);
			Sq[0].setAngle(spawner->getAngle());
			Sq[0].setDirection(spawner->getAngle());
			Sq[0].getColorData()[0] = 0.0;
			Sq[0].getColorData()[1] = 0.9;
		} break;
		case SQUARE_SPECIAL5 : {
			for (int i = 0; i < 24; i++) {
				Ptt.add(PatternObj(spawner->getX(), spawner->getY(), 2, PATTERN_TWINWAVE2));
				Ptt[0].setSpeed(6);
				Ptt[0].setDirection((float)i/24 * 360);
				for (int j = 0; j < 2; j++) {
					Sq.add(SquareObj(spawner->getX(), spawner->getY(), 32, 3.5));
					Sq[0].setDirection(Ptt[0].getDirection() - 90 + (float)j*180);
					Sq[0].setAngle(Ptt[0].getDirection());
					Sq[0].getColorData()[0] = 0.8;
					Sq[0].getColorData()[1] = 0.7;
					Sq[0].getColorData()[2] = 0.2;
					Ptt[0].add(&Sq[0]);
				}
			}
		} break;
		case SQUARE_SPECIAL4 : {
			int num = randomInt(17, 18);
			Ptt.add(PatternObj(spawner->getX(), spawner->getY(), num, PATTERN_TWINWAVE1));
			Ptt[0].setSpeed(2);
			Ptt[0].setDirection(spawner->getAngle());
			for (int i = 0; i < num; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 40, 9));
				Sq[0].setDirection(spawner->getAngle() + 90);
				if (i < num/2) {
					Sq[0].getColorData()[0] = 0.4;
				} else {
					Sq[0].getColorData()[2] = 0.4;
				}
				Sq[0].getColorData()[1] = 0.2;
				Ptt[0].add(&Sq[0]);
			}
		} break;
		case SQUARE_SPECIAL1 : {
			for (int i = 0; i < 4; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 6));
				Sq[0].setSpeed(4);
				Sq[0].setAngle(patterner+90*i);
				Sq[0].setDirection(patterner+90*i);
				Sq[0].getColorData()[0] = sin(timeeffect.getElapsed());
				Sq[0].getColorData()[1] = sin(timeeffect.getElapsed()+convertToRadian(120));
				Sq[0].getColorData()[2] = sin(timeeffect.getElapsed()+convertToRadian(240));
			}
		} break;
		case SQUARE_AIMSHOT1 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 10));
			Sq[0].setSpeed(4);
			Sq[0].setAngle(-spawner->angleTo(Point(xpos, ypos)));
			Sq[0].setDirection(-spawner->angleTo(Point(xpos, ypos)));
			Sq[0].getColorData()[0] = 0.5;
			Sq[0].getColorData()[1] = 0.0;
			Sq[0].getColorData()[2] = 0.5;
		} break;
		case SQUARE_AIMSHOT2 : {
			for (int i = 0; i < 3; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 20, 10));
				Sq[0].setSpeed(4.5);
				Sq[0].setAngle(-spawner->angleTo(Point(xpos, ypos)) + 25*(i - 1));
				Sq[0].setDirection(-spawner->angleTo(Point(xpos, ypos)) + 25*(i - 1));
				Sq[0].getColorData()[0] = 0.5;
				Sq[0].getColorData()[1] = 0.0;
				Sq[0].getColorData()[2] = 0.5;
			}
		} break;
		case SQUARE_AIMSHOT3 : {
			for (int i = 0; i < 5; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 15));
				Sq[0].setSpeed(6-(float)i/2);
				Sq[0].setAngle(-spawner->angleTo(Point(xpos, ypos)));
				Sq[0].setDirection(-spawner->angleTo(Point(xpos, ypos)));
				Sq[0].getColorData()[2] = 0.4;
			}
		} break;
		case SQUARE_AIMSHOT4 : {
			subSpw.add(Spawner2D(new SquareObj(spawner->getX(), spawner->getY(), 90, 8), TYPE_SQUARE, 0.04, 1, SQUARE_RANDOMSHOT2));
			subSpw[0].getObject()->setSpeed(7);
			subSpw[0].getObject()->setAngle(-spawner->angleTo(Point(xpos, ypos)));
			subSpw[0].getObject()->setDirection(-spawner->angleTo(Point(xpos, ypos)));
			subSpw[0].getObject()->getColorData()[1] = 0.0;
		} break;
		case QUAD_SPECIAL3 : {
			Ptt.add(PatternObj(randomFloat(-50, WINDOW_WIDTH+50), -120, 2, PATTERN_DEFAULT));
			Ptt[0].setSpeed(randomFloat(5.0, 8.0));
			Ptt[0].setDirection(randomFloat(250.0, 290.0));
			Qu.add(QuadObj(Ptt[0].getX(), Ptt[0].getY(), 70, 12, 10));
			Qu[0].setAngle(Ptt[0].getDirection());
			Qu[0].getColorData()[0] = 0.5;
			Qu[0].getColorData()[1] = 0.0;
			Qu[0].getColorData()[2] = 0.5;
			Tr.add(TriangleObj(Qu[0].getX() + 45*cos(convertToRadian(Qu[0].getAngle())), Qu[0].getY() - 45*sin(convertToRadian(Qu[0].getAngle())), 30, 40, 10));
			Tr[0].setAngle(Qu[0].getAngle());
			Tr[0].getColorData()[0] = 0.5;
			Tr[0].getColorData()[1] = 0.0;
			Tr[0].getColorData()[2] = 0.5;
			Ptt[0].add(&Qu[0]);
			Ptt[0].add(&Tr[0]);
		} break;
		case QUAD_SPECIAL4 : {
			float color[3] = {0.5, 0.0, 0.5};
			if (randomInt(0,6) == 0) {
				Qu.add(QuadObj(randomFloat(50, WINDOW_WIDTH-50), -120, 70, 12, 10));
				Ptt.add(PatternObj(Qu[0].getX(), Qu[0].getY(), 2, PATTERN_REDIRECT1));
				color[0] = 0.8;
				color[1] = 0.9;
				color[2] = 0.0;
			} else {
				Qu.add(QuadObj(randomFloat(-50, WINDOW_WIDTH+50), -120, 70, 12, 10));
				Ptt.add(PatternObj(Qu[0].getX(), Qu[0].getY(), 2, PATTERN_DEFAULT));
			}
			Qu[0].setAngle(randomFloat(250.0, 290.0));
			Ptt[0].setSpeed(randomFloat(5.0, 8.0));
			Ptt[0].setDirection(Qu[0].getAngle());
			
			Tr.add(TriangleObj(Qu[0].getX() + 45*cos(convertToRadian(Qu[0].getAngle())), Qu[0].getY() - 45*sin(convertToRadian(Qu[0].getAngle())), 30, 40, 10));
			Tr[0].setAngle(Qu[0].getAngle());
			
			for (int i = 0; i < 3; i++) {
				Qu[0].getColorData()[i] = color[i];
				Tr[0].getColorData()[i] = color[i];
			}
			Ptt[0].add(&Qu[0]);
			Ptt[0].add(&Tr[0]);
		} break;
		case TRIANGLE_SPECIAL5 : {
			float r = randomFloat(0.6,1);
			float t = randomFloat(0.0,0.5);
			for (int i = 0; i < 30; i++) {
				Tr.add(TriangleObj(spawner->getX(), spawner->getY(), 25, 15, 13));
				float ang = convertToRadian((float)i/30 * 360);
				Tr[0].setSpeed(3 + cos(ang) - abs(sin(ang)));
				Tr[0].setAngle(-spawner->angleTo(Point(xpos, ypos)));
				Tr[0].setDirection(-spawner->angleTo(Point(xpos, ypos)) + 40*sin(convertToRadian((float)i/30*360)));
				Tr[0].getColorData()[0] = r;
				Tr[0].getColorData()[1] = t;
				Tr[0].getColorData()[2] = t;
			}
		} break;
		case SQUARE_RANDOMSHOT1 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 20, 15));
			Sq[0].setSpeed(2);
			float ang = randomFloat(0, 360);
			Sq[0].setAngle(ang);
			Sq[0].setDirection(ang);
		} break;
		case SQUARE_RANDOMSHOT2 : {
			Sq.add(SquareObj(spawner->getX() + randomFloat(-25, 25), spawner->getY() + randomFloat(-25, 25), 28, 18));
			Sq[0].setSpeed(randomFloat(1.5, 5.0));
			Sq[0].setAngle(spawner->getAngle());
			Sq[0].setDirection(spawner->getAngle());
			Sq[0].getColorData()[0] = 0.8;
			Sq[0].getColorData()[1] = 0.0;
			Sq[0].getColorData()[2] = 0.8;
		} break;
		case TRIANGLE_RANDOMSHOT1 : {
			Tr.add(TriangleObj(spawner->getX(), spawner->getY(), 38, 38, 12));
			Tr[0].setSpeed(randomFloat(2.5, 5.5));
			float ang = randomFloat(0, 360);
			Tr[0].setAngle(ang);
			Tr[0].setDirection(ang);
			Tr[0].getColorData()[1] = 0.4;
			Tr[0].getColorData()[1] = 0.3;
			Tr[0].getColorData()[2] = 0.0;
		} break;
		case TRIANGLE_RANDOMSHOT2 : {
			float ang = randomFloat(0.0, 120.0);
			for (int i = 0; i < 3; i++) {
				Tr.add(TriangleObj(spawner->getX(), spawner->getY(), 25, 14, 12));
				Tr[0].setSpeed(4);
				Tr[0].setAngle(ang + 120*i);
				Tr[0].setDirection(ang + 120*i);
				Tr[0].getColorData()[1] = 0.5;
				Tr[0].getColorData()[1] = 0.0;
				Tr[0].getColorData()[2] = 0.5;
			}
		} break;
		case QUAD_ANGLESHOT1 : {
			Qu.add(QuadObj(spawner->getX(), spawner->getY(), 100, 25, 6, 0.3));
			Qu[0].setSpeed(4);
			Qu[0].setAngle(spawner->getAngle());
			Qu[0].setDirection(spawner->getAngle());
			Qu[0].getColorData()[0] = 0.0;
			Qu[0].getColorData()[1] = 0.0;
		} break;
		case QUAD_ANGLESHOT2 : {
			Qu.add(QuadObj(spawner->getX(), spawner->getY(), 80, 15, 6, 0.2));
			Qu[0].setSpeed(5);
			Qu[0].setAngle(spawner->getAngle());
			Qu[0].setDirection(spawner->getAngle());
			Qu[0].getColorData()[0] = 0.1;
			Qu[0].getColorData()[1] = 0.7;
		} break;
		case SQUARE_RAPIDSHOT1 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 6));
			float a = randomFloat(-45,45);
			Sq[0].setSpeed(7);
			Sq[0].setAngle(spawner->getAngle()+a);
			Sq[0].setDirection(spawner->getAngle()+a);
		} break;
		case SQUARE_RAPIDSHOT2 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 20, 30));
			float a = randomFloat(-15,15);
			Sq[0].setSpeed(-randomFloat(0.8,spawner->getSpeed()));
			Sq[0].setAngle(spawner->getAngle()+a);
			Sq[0].setDirection(spawner->getAngle()+a);
		} break;
		case SQUARE_ROTATEC : {
			Ptt.add(PatternObj(spawner->getX(), spawner->getY(), 24, PATTERN_ROTATEC));
			float ex = randomFloat(0.0, 360/24);
			for (int i = 0; i < 24; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 40, 9));
				Sq[0].setDirection((float)i/24 * 360 + ex);
				Sq[0].setAngle(Sq[0].getDirection());
				Sq[0].setSpeed(2);
				Sq[0].getColorData()[0] = 0.4;
				Sq[0].getColorData()[2] = 0.2;
				Ptt[0].add(&Sq[0]);
			}
		} break;
		case SQUARE_ROTATEAC : {
			Ptt.add(PatternObj(spawner->getX(), spawner->getY(), 24, PATTERN_ROTATEAC));
			float ex = randomFloat(0.0, 360/24);
			for (int i = 0; i < 24; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 40, 9));
				Sq[0].setDirection((float)i/24 * 360 + ex);
				Sq[0].setAngle(Sq[0].getDirection());
				Sq[0].setSpeed(2);
				Sq[0].getColorData()[0] = 0.2;
				Sq[0].getColorData()[1] = 0.4;
				Ptt[0].add(&Sq[0]);
			}
		} break;
		case QUAD_SPECIAL1 : {
			float ang = randomFloat(-80.0, -30.0);
			Qu.add(QuadObj(spawner->getX() + randomFloat(0.0, WINDOW_WIDTH/10), spawner->getY(), 25, 7, 12));
			Qu[0].setSpeed(4);
			Qu[0].setAngle(ang);
			Qu[0].setDirection(ang);
			Qu[0].getColorData()[0] = 0.2;
			Qu[0].getColorData()[1] = 0.2;
		} break;
		case QUAD_SPECIAL2 : {
			Ptt.add(PatternObj(spawner->getX(), spawner->getY(), 48, PATTERN_DEFAULT));
			float ex = randomFloat(0.0, 360/42);
			for (int i = 0; i < 42; i++) {
				Qu.add(QuadObj(spawner->getX(), spawner->getY(), 120, 20, 5));
				Qu[0].setDirection((float)i/42 * 360 + ex);
				Qu[0].setAngle(Qu[0].getDirection());
				Qu[0].getColorData()[0] = 0.4;
				Qu[0].getColorData()[2] = 0.6;
				Ptt[0].add(&Qu[0]);
			}
		} break;
		case TRIANGLE_SPECIAL1 : {
			for (int i = 0; i < 13; i++) {
				Tr.add(TriangleObj(spawner->getX(), spawner->getY(), 50, 35, 5));
				Tr[0].setSpeed(7);
				Tr[0].setAngle(270 + 14*(i-6));
				Tr[0].setDirection(270 + 14*(i-6));
				Tr[0].getColorData()[1] = 0.8;
				Tr[0].getColorData()[2] = 0.0;
			}
			for (int i = 0; i < 20; i++) {
				Tr.add(TriangleObj(spawner->getX(), spawner->getY(), 40, 20, 8));
				Tr[0].setSpeed(5.5);
				Tr[0].setAngle(265.5 + 9*(i-9));
				Tr[0].setDirection(265.5 + 9*(i-9));
				Tr[0].getColorData()[1] = 0.8;
				Tr[0].getColorData()[2] = 0.0;
			}
			for (int i = 0; i < 33; i++) {
				Tr.add(TriangleObj(spawner->getX(), spawner->getY(), 30, 14, 12));
				Tr[0].setSpeed(4.25);
				Tr[0].setAngle(270 + 5.5*(i-16));
				Tr[0].setDirection(270 + 5.5*(i-16));
				Tr[0].getColorData()[1] = 0.8;
				Tr[0].getColorData()[2] = 0.0;
			}
			for (int i = 0; i < 46; i++) {
				Tr.add(TriangleObj(spawner->getX(), spawner->getY(), 20, 9, 16));
				Tr[0].setSpeed(3);
				Tr[0].setAngle(268 + 4*(i-22));
				Tr[0].setDirection(268 + 4*(i-22));
				Tr[0].getColorData()[1] = 0.8;
				Tr[0].getColorData()[2] = 0.0;
			}
		} break;
		case TRIANGLE_SPECIAL2 : {
			Tr.add(TriangleObj(spawner->getX(), spawner->getY(), 15, 15, 999));
			float ang = randomFloat(0.0, 360.0);
			Tr[0].setDirection(ang);
			Tr[0].setAngle(ang);
			Tr[0].setSpeed(2.7);
			Tr[0].getColorData()[0] = 0.7;
			Tr[0].getColorData()[1] = 0.4;
			Tr[0].getColorData()[2] = 0.5;
		} break;
		case TRIANGLE_SPECIAL3 : {
			float spd = randomFloat(1.5, 4.0);
			float ang = randomFloat(0.0, 360/3);
			for(int i = 0; i < 3; i++) { 
				Tr.add(TriangleObj(spawner->getX(), spawner->getY(), 30, 30, 16));
				Tr[0].setDirection(120*i + ang);
				Tr[0].setAngle(120*i + ang);
				Tr[0].setSpeed(spd);
				Tr[0].getColorData()[0] = 0.5;
				Tr[0].getColorData()[1] = 0.4;
				Tr[0].getColorData()[2] = 0.8;
			}
		} break;
		case TRIANGLE_SPECIAL4 : {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 5; k++) {
						Tr.add(TriangleObj(spawner->getX(), spawner->getY(), 45, 35, 12));
						Tr[0].setSpeed(3+1.8*k);
						Tr[0].setDirection(spawner->getAngle() + 180*i + 6*(j-1));
						Tr[0].setAngle(Tr[0].getDirection());
						switch (patterner%8) {
							case 1: Tr[0].getColorData()[0] = 1.0; Tr[0].getColorData()[1] = 0.2; Tr[0].getColorData()[2] = 0.2;
							break;
							case 2: Tr[0].getColorData()[0] = 0.9; Tr[0].getColorData()[1] = 0.5; Tr[0].getColorData()[2] = 0.2;
							break;
							case 3: Tr[0].getColorData()[0] = 0.9; Tr[0].getColorData()[1] = 0.9; Tr[0].getColorData()[2] = 0.2;
							break;
							case 4: Tr[0].getColorData()[0] = 0.2; Tr[0].getColorData()[1] = 1.0; Tr[0].getColorData()[2] = 0.2;
							break;
							case 5: Tr[0].getColorData()[0] = 0.2; Tr[0].getColorData()[1] = 0.9; Tr[0].getColorData()[2] = 0.9;
							break;
							case 6: Tr[0].getColorData()[0] = 0.2; Tr[0].getColorData()[1] = 0.2; Tr[0].getColorData()[2] = 1.0;
							break;
							case 7: Tr[0].getColorData()[0] = 0.9; Tr[0].getColorData()[1] = 0.2; Tr[0].getColorData()[2] = 0.9;
							break;
						}
					}
				}
			}
		} break;
		case SQUARE_SPECIAL6 : {
			defSpw.add(Spawner2D(new SquareObj(spawner->getX(), spawner->getY(), 10, 999), TYPE_SQUARE, 0.04, 1, TRIANGLE_SPECIAL3));
			float ang = randomFloat(40, 80);
			if (randomInt(0,1) == 0)
				ang = -ang;
			defSpw[0].getObject()->setAngle(270 + ang);
			defSpw[0].getObject()->setDirection(270 + ang);
			defSpw[0].getObject()->setSpeed(7.5);
		} break;
		case SQUARE_SPECIAL7 : {
			for (int i = 0; i < 72; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 24, 15));
				Sq[0].setSpeed(2);
				Sq[0].setAngle(-spawner->angleTo(Point(xpos, ypos)) +5*i);
				Sq[0].setDirection(-spawner->angleTo(Point(xpos, ypos)) +5*i);
				Sq[0].getColorData()[0] = sin(timeeffect.getElapsed()+convertToRadian(240));
				Sq[0].getColorData()[1] = sin(timeeffect.getElapsed()+convertToRadian(120));
				Sq[0].getColorData()[2] = sin(timeeffect.getElapsed());
			}
		} break;
		case SQUARE_SPECIAL8 : {
			float ang = randomFloat(0.0, 60.0);
			Ptt.add(PatternObj(spawner->getX(), spawner->getY(), 96, PATTERN_SLINGSHOT));
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 6; i++) {
						Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 9));
						Sq[0].setDirection((float)i*360/6 + ang);
						Sq[0].setAngle((float)i*360/6 + ang + 45);
						Sq[0].setSpeed(3+6*j);
						Sq[0].getColorData()[1] = 0.8;
						Sq[0].getColorData()[2] = 0.0;
						Ptt[0].add(&Sq[0]);
					}
				for (int k = 0; k < 2; k++) {
					for (int i = 0; i < 6; i++) {
						Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 9));
						Sq[0].setDirection((float)i*360/6 + ang + (9-18*k) - 5*j*(1-2*k));
						Sq[0].setAngle((float)i*360/6 + ang + 45);
						Sq[0].setSpeed(3.75+4.5*j);
						Sq[0].getColorData()[1] = 0.8;
						Sq[0].getColorData()[2] = 0.0;
						Ptt[0].add(&Sq[0]);
					}
					for (int i = 0; i < 6; i++) {
						Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 9));
						Sq[0].setDirection((float)i*360/6 + ang + (13-26*k) - 4.5*j*(1-2*k));
						Sq[0].setAngle((float)i*360/6 + ang + 45);
						Sq[0].setSpeed(4.5+3*j);
						Sq[0].getColorData()[1] = 0.8;
						Sq[0].getColorData()[2] = 0.0;
						Ptt[0].add(&Sq[0]);
					}
					for (int i = 0; i < 6; i++) {
						Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 9));
						Sq[0].setDirection((float)i*360/6 + ang + (14.5-29*k) - 3*j*(1-2*k));
						Sq[0].setAngle((float)i*360/6 + ang + 45);
						Sq[0].setSpeed(5.25+1.5*j);
						Sq[0].getColorData()[1] = 0.8;
						Sq[0].getColorData()[2] = 0.0;
						Ptt[0].add(&Sq[0]);
					}
				}
			}
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 6; i++) {
					Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 9));
					Sq[0].setDirection((float)i*360/6 + ang + 15-30*j);
					Sq[0].setAngle((float)i*360/6 + ang + 45);
					Sq[0].setSpeed(6);
					Sq[0].getColorData()[1] = 0.8;
					Sq[0].getColorData()[2] = 0.0;
					Ptt[0].add(&Sq[0]);
				}
			}
			for (int i = 0; i < 48; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 20, 5));
				Sq[0].setSpeed(3.8);
				Sq[0].setAngle(ang+(float)i/24*180+3.75);
				Sq[0].setDirection(Sq[0].getAngle());
				Sq[0].getColorData()[0] = 0.3;
				Sq[0].getColorData()[2] = 0.0;
			}
		} break;
		case SQUARE_SPECIAL9 : {
			switch (randomInt(1,4)) {
				case 1:
					Sq.add(SquareObj(randomFloat(50, WINDOW_WIDTH-50), -50, 25, 14));
					Sq[0].setDirection(randomFloat(190, 350));
				break;
				case 2:
					Sq.add(SquareObj(randomFloat(50, WINDOW_WIDTH-50), WINDOW_HEIGHT+50, 25, 14));
					Sq[0].setDirection(randomFloat(10, 170));
				break;
				case 3:
					Sq.add(SquareObj(-50, randomFloat(50, WINDOW_HEIGHT-50), 25, 14));
					Sq[0].setDirection(randomFloat(-80, 80));
				break;
				case 4:
					Sq.add(SquareObj(WINDOW_WIDTH+50, randomFloat(50, WINDOW_HEIGHT-50), 25, 14));
					Sq[0].setDirection(randomFloat(100, 260));
				break;
			}
			Sq[0].setSpeed(randomFloat(2.0, 2.9));
			Sq[0].setAngle(Sq[0].getDirection());
			int col = randomInt(0,2);
			for (int i = 0; i < 3; i++) {
				if (i == col)
					Sq[0].getColorData()[i] = randomFloat(0.8, 1.0);
				else
					Sq[0].getColorData()[i] = randomFloat(0.3, 0.6);
			}
		} break;
		case SQUARE_SPECIAL10 : {
			for (int i = 0; i < 6; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 20));
				Sq[0].setSpeed(7);
				float ang = randomFloat(0, 360);
				Sq[0].setAngle(ang);
				Sq[0].setDirection(ang);
				Sq[0].getColorData()[0] = 0.9;
				Sq[0].getColorData()[1] = 0.6;
				Sq[0].getColorData()[2] = 0.6;
			}
		} break;
		case SQUARE_SPECIAL11 : {
			Ptt.add(PatternObj(spawner->getX(), spawner->getY(), 32, PATTERN_FALL1));
			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 8; i++) {
					Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 9));
					Sq[0].setSpeed(2 + j);
					Sq[0].setDirection((float)i*45);
					Sq[0].getColorData()[2] = 0.0;
					Ptt[0].add(&Sq[0]);
				}
			}
		} break;
		case SQUARE_EXPLODE1 : {
			for (int i = 0; i < 12; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 20, 10));
				Sq[0].setSpeed(4.2);
				Sq[0].setAngle(spawner->getAngle()+(float)i/12*360);
				Sq[0].setDirection(spawner->getAngle()+(float)i/12*360);
			}
		} break;
		case SQUARE_EXPLODE2 : {
			float ang = -spawner->angleTo(Point(xpos, ypos));
			for (int i = 0; i < 12; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 20, 10));
				Sq[0].setSpeed(4.2);
				Sq[0].setAngle(ang+(float)i/12*360);
				Sq[0].setDirection(ang+(float)i/12*360);
				Sq[0].getColorData()[1] = 0.5;
				Sq[0].getColorData()[2] = 0.3;
			}
		} break;
		case SQUARE_EXPLODE3 : {
			float ang = -spawner->angleTo(Point(xpos, ypos));
			for (int i = 0; i < 19; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 12));
				Sq[0].setSpeed(3);
				Sq[0].setAngle(ang+(float)i/19*360);
				Sq[0].setDirection(ang+(float)i/19*360);
			}
		} break;
	}
	spawntimer.resetTime();

}

//Level init
void createLevel() {
	linesize = 2;
	titlemark = 0;
	switch (gamemode) {
		case ID_LEVEL_A : {
			for (int i = 0; i < 10; i++) {
				if (i%2 == 0)
					defSpw.add(Spawner2D(new SquareObj(-400, WINDOW_HEIGHT/9*(i), 150 - 10*(i), 999), TYPE_SQUARE, 999, 1, NULL_SHOT));
				else
					defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH+400, WINDOW_HEIGHT/9*(i), 50 + 10*(i-1), 999), TYPE_SQUARE, 999, 1, NULL_SHOT));
			}
			defSpw.setBegin();
			for (int i = 0; i < 10; i++) {
				defSpw.getIValue().getObject()->setSpeed(6);
				if (i%2 == 0)
					defSpw.getIValue().getObject()->setDirection(0);
				else
					defSpw.getIValue().getObject()->setDirection(180);
				defSpw.next();
			}
			float x = randomFloat(1,3.2);
			for (int i = 0; i < 10; i++) {
				defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/9*(i), -100, 70, 999, 300), TYPE_SQUARE, 999, 1, NULL_SHOT));
			}
			defSpw.setBegin();
			for (int i = 0; i < 10; i++) {
				defSpw.getIValue().getObject()->setSpeed(x);
				defSpw.getIValue().getObject()->setDirection(90);
				defSpw.next();
			}
			
			Spw.add(Spawner2D(new SquareObj(-150, -150, 180, 999), TYPE_SQUARE, 999, 280 + SHOT_THRESHOLD, NULL_SHOT));
			Spw[0].getObject()->getColorData()[0] = 0.0;
			Spw[0].getObject()->getColorData()[1] = 0.1;
			Spw[0].getObject()->getColorData()[2] = 0.7;
			
			for (int i = 0; i < 9; i++) {
				defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/9*(i+0.5), -100, 70, 999, 300), TYPE_SQUARE, 999, 1, NULL_SHOT));
			}
			
			float y = randomFloat(1,3.2);
			defSpw.setBegin();
			for (int i = 0; i < 9; i++) {
				defSpw.getIValue().getObject()->setSpeed(y);
				defSpw.getIValue().getObject()->setDirection(270);
				defSpw.next();
			}
			worldn = 1;

			convertStringToLetterData(title_LevelA_str, 10, 10, linesize, title, titlemark, true);	
		} break;
		case ID_ZONE1: {
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -100, 1, 7), TYPE_SQUARE, 0.32, 0, SQUARE_ANGLESHOT2));
			worldn = 2;
		} break;
		case ID_LEVEL_B: {
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -100, 1, 999), TYPE_SQUARE, 999, 0, SQUARE_ANGLESHOT2));
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, WINDOW_WIDTH + 100, 1, 999), TYPE_SQUARE, 999, 0, SQUARE_ANGLESHOT2));
			defSpw.add(Spawner2D(new SquareObj(-100, WINDOW_HEIGHT/2, 1, 999), TYPE_SQUARE, 999, 0, SQUARE_ANGLESHOT2));
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH + 100, WINDOW_HEIGHT/2, 1, 999), TYPE_SQUARE, 999, 0, SQUARE_ANGLESHOT2));
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -180, 200, 999, 1), TYPE_SQUARE, 1.0, 250 + SHOT_THRESHOLD, SQUARE_ANGLESHOT2));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.4;
			Spw[0].getObject()->getColorData()[2] = 0.4;
			worldn = 2;
			
			convertStringToLetterData(title_LevelB_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_ZONE1B: {
			defSpw.add(Spawner2D(new SquareObj(-100, WINDOW_HEIGHT/2, 1, 7), TYPE_SQUARE, 0.32, 0, SQUARE_ANGLESHOT2));
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH+100, WINDOW_HEIGHT/2, 1, 7), TYPE_SQUARE, 0.32, 0, SQUARE_ANGLESHOT2));
			worldn = 2;
		} break;
		case ID_ZONE7: {
			worldn = 2;
		} break;
		case ID_ZONE7B: {
			worldn = 3;
		} break;
		case ID_ZONE8: {
			for (int i = 0; i < 3; i++) {
				Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH*(i+1)/4, randomFloat(-380, -240), 110, 100, 9), TYPE_TRIANGLE, 999, 30, SQUARE_LASER1));
				Spw[0].getObject()->setSpeed(7);
				Spw[0].getObject()->setDirection(270);
				Spw[0].getObject()->getColorData()[0] = 0.0;
				Spw[0].getObject()->getColorData()[2] = 0.0;
			}
			worldn = 1;
		} break;
		case ID_SHOOTER1: {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -100, 150, 999), TYPE_SQUARE, 0.04, 100, SQUARE_ANGLESHOT1));
			Spw[0].getObject()->setSpeed(4);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.0;
			Spw[0].getObject()->getColorData()[2] = 0.0;
			worldn = 1;
		} break;
		case ID_BULLET1: {
			defSpw.add(Spawner2D(new SquareObj(-100, WINDOW_HEIGHT+100, 1, 5), TYPE_SQUARE, 0.6, 0, SQUARE_ANGLESHOT3));
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH+100, WINDOW_HEIGHT+100, 1, 5), TYPE_SQUARE, 0.6, 0, SQUARE_ANGLESHOT3));
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH+100, -100, 1, 5), TYPE_SQUARE, 0.6, 0, SQUARE_ANGLESHOT3));
			defSpw.add(Spawner2D(new SquareObj(-100, -100, 1, 5), TYPE_SQUARE, 0.6, 0, SQUARE_ANGLESHOT3));
			worldn = 1;
		} break;
		case ID_LEVEL_C: {
			Spw.add(Spawner2D(new SquareObj(-200, -200, 160, 999, 1), TYPE_SQUARE, 0.4, 110, SQUARE_ANGLESHOT3));
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH+200, -200, 160, 999, 1), TYPE_SQUARE, 0.4, 110, SQUARE_ANGLESHOT3));
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH+200, WINDOW_HEIGHT+200, 160, 999, 1), TYPE_SQUARE, 0.4, 110, SQUARE_ANGLESHOT3));
			Spw.add(Spawner2D(new SquareObj(-200, WINDOW_HEIGHT+200, 160, 999, 1), TYPE_SQUARE, 0.4, 110, SQUARE_ANGLESHOT3));
			Spw.setBegin();
			for (int i = 0; i < 4; i++) {
				Spw.getIValue().getObject()->setSpeed(6);
				Spw.getIValue().getObject()->setDirection(90*i + 45);
				Spw.getIValue().getObject()->getColorData()[1] = 0.0;
				Spw.next();
			}
			worldn = 1;
			
			convertStringToLetterData(title_LevelC_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_BULLET2: {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -180, 175, 23, 1), TYPE_SQUARE, 0.01, 180, QUAD_ANGLESHOT1));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.0;
			Spw[0].getObject()->getColorData()[2] = 0.0;
			worldn = 2;
		} break;
		case ID_SHOOTER2: {
			for (int i = 0; i < 3; i++) {
				Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/4*(i+1), -180, 150, 130, 10, 1), TYPE_TRIANGLE, 1, 36, QUAD_ANGLESHOT1));
				Spw[0].getObject()->setSpeed(6);
				Spw[0].getObject()->setDirection(270);
				Spw[0].getObject()->getColorData()[0] = 0.0;
				Spw[0].getObject()->getColorData()[2] = 0.4;
			}
			worldn = 1;
		} break;
		case ID_SHOOTER3: {
			float b;
			for (int i = 0; i < 5; i++) {
				b = randomFloat(-250, 0);
				Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/6*(i+1) + randomFloat(-60, 60), -180 + b, 130, 9-(b + 250)/100, 1), TYPE_SQUARE, 2, 8, SQUARE_AIMSHOT2));
				Spw[0].getObject()->setSpeed(7);
				Spw[0].getObject()->setDirection(270);
				Spw[0].getObject()->getColorData()[0] = 0.4;
				Spw[0].getObject()->getColorData()[2] = 0.0;
			}
			worldn = 1;
		} break;
		case ID_SHOOTER4: {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -180, 150, 22.5, 1), TYPE_SQUARE, 0.3, 150, SQUARE_ANGLESHOT4));
			Spw[0].getObject()->setSpeed(5);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.0;
			Spw[0].getObject()->getColorData()[2] = 0.3;
			worldn = 2;
		} break;
		case ID_SHOOTER5: {
			for (int i = 0; i < 3; i++) {
				Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/4*(i+1), -180 + 30*abs(1-i), 140, 100, 14, 1), TYPE_TRIANGLE, 1.2, 30, SQUARE_AIMSHOT3));
				Spw[0].getObject()->setSpeed(6);
				Spw[0].getObject()->setDirection(270);
				Spw[0].getObject()->setAngle(270);
				Spw[0].getObject()->getColorData()[0] = 0.2;
				Spw[0].getObject()->getColorData()[2] = 0.2;
			}
			worldn = 1;
		} break;
		case ID_RIPPLES : {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -120, 100, 25, 2), TYPE_SQUARE, 2, 320, SQUARE_ROTATEC));
			Spw[0].getObject()->setSpeed(3);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.7;
			Spw[0].getObject()->getColorData()[2] = 0.7;
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -120, 1, 25, 2), TYPE_SQUARE, 2, 1, SQUARE_ROTATEAC));
			defSpw[0].getObject()->setSpeed(3);
			defSpw[0].getObject()->setDirection(270);
			defSpw[0].getObject()->getColorData()[0] = 0.0;
			defSpw[0].getObject()->getColorData()[1] = 0.0;
			defSpw[0].getObject()->getColorData()[2] = 0.0;
			worldn = 1;
			
			convertStringToLetterData(title_Rippler_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_SHOOTER7 : {
			Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -180, 180, 180, 999, 1), TYPE_TRIANGLE, 2.0, 200, SQUARE_ANGLESHOT5));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.0;
			Spw[0].getObject()->getColorData()[2] = 0.0;
			worldn = 1;
		} break;
		case ID_RAPIDSHOOTER : {
			Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -180, 200, 180, 999, 1), TYPE_TRIANGLE, 1, 280, SQUARE_RAPIDSHOT1));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.0;
			worldn = 1;
			
			convertStringToLetterData(title_RapidShooter_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_DRONEDFIGHTER : {
			Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -180, 120, 160, 999, 2), TYPE_TRIANGLE, 2.6, 250, SQUARE_SPECIAL4));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setAngle(270);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.0;
			Spw[0].getObject()->getColorData()[2] = 0.5;
			worldn = 1;
			for (int i = 0; i < 2; i++) {
				defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -180, 70, 999, 2), TYPE_SQUARE, 2.0, 1, QUAD_ANGLESHOT2));
				defSpw[0].getObject()->setSpeed(6);
				defSpw[0].getObject()->setDirection(270);
			}
			convertStringToLetterData(title_Drone_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_RINGSHOOTER : {
			Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -180, 160, 150, 999, 1), TYPE_TRIANGLE, 0.9, 350, SQUARE_SPECIAL2));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.0;
			Spw[0].getObject()->getColorData()[1] = 0.7;
			worldn = 1;
			
			convertStringToLetterData(title_RingShooter_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_DEADLYHEARTS : {
			for (int i = 0; i < 2; i++) {
				Spw.add(Spawner2D(new SquareObj(150+(WINDOW_WIDTH-300)*i, -180, 280, 999, 1.5), TYPE_SQUARE, 1.5, 240, TRIANGLE_SPECIAL5));
				Spw[0].getObject()->setSpeed(4);
				Spw[0].getObject()->setDirection(270);
				Spw[0].getObject()->getColorData()[0] = 0.8;
				Spw[0].getObject()->getColorData()[1] = 0.4;
				Spw[0].getObject()->getColorData()[2] = 0.4;
			}
			worldn = 1;
			
			convertStringToLetterData(title_DeathHearts_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_CHASER : {
			Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -200, 200, 150, 999), TYPE_TRIANGLE, 0.1, 180, SQUARE_RAPIDSHOT2));
			Spw[0].getObject()->setSpeed(4);
			Spw[0].getObject()->setDirection(-90);
			Spw[0].getObject()->getColorData()[0] = 0.2;
			Spw[0].getObject()->getColorData()[1] = 0.6;
			Spw[0].getObject()->getColorData()[2] = 0.3;
			worldn = 1;

			convertStringToLetterData(title_Chaser_str, 10, 10, linesize, title, titlemark, true);	
		} break;
		case ID_DANMAKU1 : {
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -180, 50, 999, 1), TYPE_SQUARE, 0.02, 1, SQUARE_RANDOMSHOT1));
			defSpw[0].getObject()->setSpeed(6);
			defSpw[0].getObject()->setDirection(270);
			defSpw[0].getObject()->getColorData()[0] = 0.1;
			defSpw[0].getObject()->getColorData()[1] = 0.6;
			defSpw[0].getObject()->getColorData()[2] = 0.5;
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -180, 150, 999, 1), TYPE_SQUARE, 0.02, 480, SQUARE_SPECIAL1));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.1;
			Spw[0].getObject()->getColorData()[1] = 0.6;
			Spw[0].getObject()->getColorData()[2] = 0.5;
			worldn = 1;
			
			convertStringToLetterData(title_Danmaku1_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_RAIN : {
			for (int i = 0; i < 15; i++) {
				defSpw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/10*((float)i-5), -100, 2, 2, 999), TYPE_TRIANGLE, 0.25, 1, QUAD_SPECIAL1));
				defSpw[0].getSpawnTimer().setTime(randomFloat(0.0, 250.0));
			}
			Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -180, 200, 180, 999, 1), TYPE_TRIANGLE, 15, 540, TRIANGLE_SPECIAL1));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->setAngle(270);
			Spw[0].getObject()->getColorData()[0] = 0.1;
			Spw[0].getObject()->getColorData()[1] = 0.0;
			Spw[0].getObject()->getColorData()[2] = 0.6;
			worldn = 1;
			
			Sq.add(SquareObj(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 2000, 0.5, 2));
			Sq[0].getColorData()[0] = 0.0;
			Sq[0].getColorData()[1] = 0.0;
			
			convertStringToLetterData(title_Rain_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_LASERBLOOM : {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 240, 999, 1.5), TYPE_SQUARE, 0.6, 420, QUAD_SPECIAL2));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->setAngle(270);
			Spw[0].getObject()->getColorData()[0] = 0.1;
			Spw[0].getObject()->getColorData()[1] = 0.5;
			Spw[0].getObject()->getColorData()[2] = 0.4;
			worldn = 1;
			
			convertStringToLetterData(title_LaserBloom_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_RADIANCE : {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 200, 999, 1.5), TYPE_SQUARE, 10, 240, SQUARE_SPECIAL5));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.1;
			Spw[0].getObject()->getColorData()[1] = 0.7;
			Spw[0].getObject()->getColorData()[2] = 0.1;
			worldn = 2;
			
			convertStringToLetterData(title_Radiance_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_SPIKECAGE : {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 120, 999, 1.5), TYPE_SQUARE, 70, 420, TRIANGLE_SPECIAL2));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->setAngle(45);
			Spw[0].getObject()->getColorData()[0] = 1.0;
			Spw[0].getObject()->getColorData()[1] = 0.6;
			Spw[0].getObject()->getColorData()[2] = 0.3;
			Qu.add(QuadObj(0, WINDOW_HEIGHT/2, 20, WINDOW_HEIGHT, 998, 999));
			Qu.add(QuadObj(WINDOW_WIDTH, WINDOW_HEIGHT/2, 20, WINDOW_HEIGHT, 998, 999));
			Qu.add(QuadObj(WINDOW_WIDTH/2, 0, WINDOW_WIDTH, 20, 998, 999));
			Qu.add(QuadObj(WINDOW_WIDTH/2, WINDOW_HEIGHT, WINDOW_WIDTH, 20, 998, 999));
			Qu.add(QuadObj(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT, 0.5, 1));
			Qu.setBegin();
			for (int i = 0; i < Qu.size(); i++) {
				Qu.getIValue().getColorData()[0] = 0.5;
				Qu.getIValue().getColorData()[1] = 0.0;
				Qu.getIValue().getColorData()[2] = 0.0;
				Qu.next();
			}
			worldn = 1;
			
			convertStringToLetterData(title_SpikeCage_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_ICESPIRIT : {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 160, 999, 1.0), TYPE_SQUARE, 999, 500 + SHOT_THRESHOLD, SQUARE_SPECIAL6));
			Spw[0].getObject()->setAngle(45);
			Spw[0].getObject()->getColorData()[0] = 0.2;
			Spw[0].getObject()->getColorData()[1] = 0.4;
			Spw[0].getObject()->getColorData()[2] = 0.7;
			
			Ptt.add(PatternObj(WINDOW_WIDTH/2, -240, 5, PATTERN_DEFAULT));
			Ptt[0].setSpeed(6);
			Ptt[0].setDirection(270);
			Ptt[0].add(Spw[0].getObject());
			for (int i = 0; i < 4; i++) {
				defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2 + 70*cos(convertToRadian(90*i)), -240 - 70*sin(convertToRadian(90*i)), 45, 999, 1.0), TYPE_SQUARE, 999, 1, NULL_SHOT));
				defSpw[0].getObject()->setAngle(45);
				defSpw[0].getObject()->getColorData()[0] = 0.0;
				Ptt[0].add(defSpw[0].getObject());
			}
			
			Spw[0].getSpawnTimer().setTime(999000);
			
			worldn = 2;
			
			convertStringToLetterData(title_IceSpirit_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_EARTHSPIRIT : {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 160, 999, 1.0), TYPE_SQUARE, 999, 560 + SHOT_THRESHOLD, NULL_SHOT));
			Spw[0].getObject()->setAngle(45);
			Spw[0].getObject()->getColorData()[0] = 0.3;
			Spw[0].getObject()->getColorData()[1] = 0.2;
			Spw[0].getObject()->getColorData()[2] = 0.1;
			
			Ptt.add(PatternObj(WINDOW_WIDTH/2, -240, 5, PATTERN_DEFAULT));
			Ptt[0].setSpeed(6);
			Ptt[0].setDirection(270);
			Ptt[0].add(Spw[0].getObject());
			for (int i = 0; i < 4; i++) {
				defSpw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2 + 60*cos(convertToRadian(90*i + 45)), -240 - 60*sin(convertToRadian(90*i + 45)), 65, 60, 999, 1.0), TYPE_TRIANGLE, 0.04, 1, TRIANGLE_RANDOMSHOT1));
				defSpw[0].getObject()->setAngle(90*i + 45);
				defSpw[0].getObject()->getColorData()[0] = 0.4;
				defSpw[0].getObject()->getColorData()[1] = 0.3;
				Ptt[0].add(defSpw[0].getObject());
			}
			worldn = 1;
			
			convertStringToLetterData(title_EarthSpirit_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_SHOOTER6 : { ///Edit pre-available enemies here
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/3, -240, 180, 21, 0.9), TYPE_SQUARE, 1, 70, SQUARE_AIMSHOT4));
			Spw[0].getObject()->setDirection(270);
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH*2/3, -240, 180, 21, 0.9), TYPE_SQUARE, 1, 70, SQUARE_AIMSHOT4));
			Spw[0].getObject()->setDirection(270);
			Spw.add(Spawner2D(new SquareObj(-240, 180, 180, 21, 0.9), TYPE_SQUARE, 1, 70, SQUARE_AIMSHOT4));
			Spw[0].getObject()->setDirection(0);
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH + 240, 180, 180, 21, 0.9), TYPE_SQUARE, 1, 70, SQUARE_AIMSHOT4));
			Spw[0].getObject()->setDirection(180);
			Spw.setBegin();
			for (int i = 0; i < Spw.size(); i++) {
				Spw.getIValue().getObject()->setSpeed(6);
				Spw.getIValue().getObject()->getColorData()[0] = 0.1;
				Spw.getIValue().getObject()->getColorData()[2] = 0.0;
				Spw.next();
			}
			worldn = 2;
		} break;
		case ID_DANMAKU2 : {
			Ptt.add(PatternObj(WINDOW_WIDTH/2, -240, 2, PATTERN_DEFAULT));
			Ptt[0].setSpeed(6);
			Ptt[0].setDirection(270);
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 50, 999, 1.6), TYPE_SQUARE, 999, 999, NULL_SHOT));
			Spw[0].getObject()->setAngle(45);
			Spw[0].getObject()->getColorData()[0] = 0.0;
			Spw[0].getObject()->getColorData()[1] = 0.0;
			Spw[0].getObject()->getColorData()[2] = 0.0;
			Ptt[0].add(Spw[0].getObject());
			
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 250, 999, 1.5), TYPE_SQUARE, 0.65, 330, SQUARE_SPECIAL7));
			Spw[0].getObject()->getColorData()[0] = 0.5;
			Spw[0].getObject()->getColorData()[1] = 0.5;
			Spw[0].getObject()->getColorData()[2] = 0.3;
			Ptt[0].add(Spw[0].getObject());
			
			worldn = 2;
			
			convertStringToLetterData(title_Danmaku2_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_PETAL : {
			Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -240, 173.205, 200, 999, 6.1), TYPE_TRIANGLE, 6, 480, SQUARE_SPECIAL8));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.2;
			Spw[0].getObject()->getColorData()[1] = 0.4;
			Spw[0].getObject()->getColorData()[2] = 0.8;
			worldn = 2;
			
			convertStringToLetterData(title_PetalBarrage_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_DASHER : {
			Spw.add(Spawner2D(new QuadObj(WINDOW_WIDTH/2, -240, 210, 100, 999, 1.5), TYPE_QUAD, 999, 400, TRIANGLE_SPECIAL4));
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->setAngle(270);
			Spw[0].getObject()->getColorData()[0] = 0.2;
			Spw[0].getObject()->getColorData()[1] = 0.5;
			Spw[0].getObject()->getColorData()[2] = 0.6;
			worldn = 3;
			
			pointmark = Point(WINDOW_WIDTH/2, 200);
			
			convertStringToLetterData(title_Dasher_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_ASTRALCAGE : {
			Ptt.add(PatternObj(WINDOW_WIDTH/2, 500, 9, PATTERN_DEFAULT));
			pointmark = Point(WINDOW_WIDTH/2, 500);
			
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -1000, 80, 999, 3.5), TYPE_SQUARE, 0.03, 660, SQUARE_SPECIAL9));
			Spw[0].getObject()->setSpeed(10);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->setAngle(45);
			Spw[0].getObject()->getColorData()[0] = 0.5;
			Spw[0].getObject()->getColorData()[1] = 0.2;
			Spw[0].getObject()->getColorData()[2] = 0.9;
			Ptt[0].add(Spw[0].getObject());
			for (int i = 0; i < 4; i++) {
				defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2 + 1500*cos(convertToRadian(90*i)), 500 + 1500*sin(convertToRadian(90*i)), 80, 999), TYPE_SQUARE, 999, 1, NULL_SHOT));
				defSpw[0].getObject()->setDirection(180 - 90*i);
				defSpw[0].getObject()->setSpeed(10);
				defSpw[0].getObject()->setAngle(45);
				defSpw[0].getObject()->getColorData()[0] = 0.6;
				defSpw[0].getObject()->getColorData()[1] = 0.6;
				defSpw[0].getObject()->getColorData()[2] = 0.2;
				Ptt[0].add(defSpw[0].getObject());
			}
			for (int i = 0; i < 4; i++) {
				defSpw.add(Spawner2D(new QuadObj(WINDOW_WIDTH/2 + 750*sqrtTwo*cos(convertToRadian(90*i + 45)), 500 + 750*sqrtTwo*sin(convertToRadian(90*i + 45)), 250, 30, 999), TYPE_QUAD, 999, 1, NULL_SHOT));
				defSpw[0].getObject()->setSpeed(5*sqrtTwo);
				defSpw[0].getObject()->setDirection(135 - 90*i);
				defSpw[0].getObject()->setAngle(defSpw[0].getObject()->getDirection() + 90);
				defSpw[0].getObject()->getColorData()[0] = 0.0;
				Ptt[0].add(defSpw[0].getObject());
			}
			for (int i = 0; i < 4; i++) {
				defSpw.add(Spawner2D(new QuadObj(WINDOW_WIDTH/2 + 750*sqrtTwo*cos(convertToRadian(90*i + 45)), 500 + 750*sqrtTwo*sin(convertToRadian(90*i + 45)), 1800, 30, 5), TYPE_QUAD, 999, 1, NULL_SHOT));
				defSpw[0].getObject()->setSpeed(5*sqrtTwo);
				defSpw[0].getObject()->setDirection(135 - 90*i);
				defSpw[0].getObject()->setAngle(defSpw[0].getObject()->getDirection() + 90);
				defSpw[0].getObject()->getColorData()[0] = 0.0;
			}
			worldn = 1;
			
			convertStringToLetterData(title_AstralCage_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_TIMESORCERER : {
			Ptt.add(PatternObj(WINDOW_WIDTH/2, -240, 3, PATTERN_DEFAULT));
			Ptt[0].setSpeed(6);
			Ptt[0].setDirection(270);
			
			for (int i = 0; i < 2; i++) {
				Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 70, 999, 1.6), TYPE_SQUARE, 999, 999, NULL_SHOT));
				Spw[0].getObject()->setAngle(45*i);
				Spw[0].getObject()->getColorData()[0] = 0.0;
				Spw[0].getObject()->getColorData()[1] = 0.0;
				Spw[0].getObject()->getColorData()[2] = 0.0;
				Ptt[0].add(Spw[0].getObject());
			}
			
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 150, 999, 1.5), TYPE_SQUARE, 60, 500, SQUARE_SPECIAL10));
			Ptt[0].add(Spw[0].getObject());
			Spw[0].getObject()->getColorData()[0] = 0.2;
			Spw[0].getObject()->getColorData()[1] = 0.2;
			Spw[0].getObject()->getColorData()[2] = 0.5;
			
			worldn = 2;
			
			convertStringToLetterData(title_TimeSorcerer_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_SPIRALMADNESS : {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -200, 200, 999, 1.5), TYPE_SQUARE, 999, 640, NULL_SHOT));
			Spw[0].getObject()->setSpeed(4);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.0;
			Spw[0].getObject()->getColorData()[1] = 0.6;
			Spw[0].getObject()->getColorData()[2] = 0.0;
			Ptt.add(PatternObj(WINDOW_WIDTH/2, -200, 6, PATTERN_ROTATEC2));
			Ptt[0].setSpeed(4);
			Ptt[0].setDirection(270);
			for (int i = 0; i < 6; i++) {
				switch (i%3) {
					case 0: defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2 - 50 + 100*(i/3), -200, 45, 999, 1.5), TYPE_SQUARE, 0.055, 1, SQUARE_ANGLESHOT6));
					break;
					case 1: defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2 - 50 + 100*(i/3), -200, 45, 999, 1.5), TYPE_SQUARE, 0.035, 1, SQUARE_ANGLESHOT6B));
					break;
					case 2: defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2 - 50 + 100*(i/3), -200, 45, 999, 1.5), TYPE_SQUARE, 0.025, 1, SQUARE_ANGLESHOT6C));
					break;
				}
				defSpw[0].getObject()->setDirection(-90 + 180*(i/3));
				defSpw[0].getObject()->getColorData()[1] = 0.0;
				Ptt[0].add(defSpw[0].getObject());
			}
			pointmark = Point(WINDOW_WIDTH/2, 135);
			worldn = 1;
			
			convertStringToLetterData(title_SpiralMadness_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_ZONE10 : {
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 200, 6), TYPE_SQUARE, 0.028, 1, QUAD_SPECIAL3));
			worldn = 1;
		} break;
		case ID_ARROWMASTER : {
			Ptt.add(PatternObj(WINDOW_WIDTH/2, -240, 3, PATTERN_DEFAULT));
			Ptt[0].setSpeed(6);
			Ptt[0].setDirection(270);
			
			for (int i = 0; i < 2; i++) {
				if (i == 0)
					Spw.add(Spawner2D(new QuadObj(WINDOW_WIDTH/2, -250, 40, 15, 999, 1.1), TYPE_QUAD, 999, 999, NULL_SHOT));
				else
					Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -220, 30, 40, 999, 1.1), TYPE_TRIANGLE, 999, 999, NULL_SHOT));
				Spw[0].getObject()->setAngle(270);
				Spw[0].getObject()->getColorData()[0] = 0.0;
				Spw[0].getObject()->getColorData()[1] = 0.0;
				Spw[0].getObject()->getColorData()[2] = 0.0;
				Ptt[0].add(Spw[0].getObject());
			}
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 190, 999, 1.0), TYPE_SQUARE, 0.038, 410, QUAD_SPECIAL4));
			Spw[0].getObject()->setAngle(45);
			Spw[0].getObject()->getColorData()[0] = 0.8;
			Spw[0].getObject()->getColorData()[1] = 0.9;
			Spw[0].getObject()->getColorData()[2] = 0.2;
			Ptt[0].add(Spw[0].getObject());
			worldn = 1;
			pointmark = Point(WINDOW_WIDTH/2, 135);
			
			convertStringToLetterData(title_ArrowMaster_str, 10, 10, linesize, title, titlemark, true);
		} break;
		case ID_SHOOTER8 : {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -200, 160, 19, 1.0), TYPE_SQUARE, 0.9, 120, SQUARE_SPECIAL11));
			Spw[0].getObject()->setSpeed(7);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.1;
			Spw[0].getObject()->getColorData()[2] = 0.0;
			worldn = 2;
			pointmark = Point(WINDOW_WIDTH/2, 120);
		} break;
		case ID_SHOOTER8B : {
			Ptt.add(PatternObj(WINDOW_WIDTH/5, -240, 2, PATTERN_OPPOSITE1));
			Ptt[0].setSpeed(7);
			Ptt[0].setDirection(270);
			bool decider;
			if (randomInt(0,1) == 0)
				decider = false;
			else
				decider = true;
			for (int i = 0; i < 2; i++) {
				if (decider)
					Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/5 + WINDOW_WIDTH*3/5*i, -240 + 130*i, 160, 25 - i, 1.0), TYPE_SQUARE, 1.5, 120, SQUARE_SPECIAL11));
				else
					Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/5 + WINDOW_WIDTH*3/5*i, -110 - 130*i, 160, 24 + i, 1.0), TYPE_SQUARE, 1.5, 120, SQUARE_SPECIAL11));
				Spw[0].getObject()->getColorData()[0] = 0.2;
				Spw[0].getObject()->getColorData()[2] = 0.0;
				Ptt[0].add(Spw[0].getObject());
			}
			worldn = 2;
			pointmark = Point(WINDOW_WIDTH/5, 110);
		} break;
		case ZONE_TEST : {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 200, 999, 1.5), TYPE_SQUARE, 1, 700, SQUARE_SPECIAL11));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.7;
			Spw[0].getObject()->getColorData()[1] = 0.7;
			Spw[0].getObject()->getColorData()[2] = 0.7;
			worldn = 1;
			pointmark = Point(WINDOW_WIDTH/2, 135);
			
			convertStringToLetterData("Zone Test", 10, 10, linesize, title, titlemark, true);
		} break;
		/**case ZONE_TEST : {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -240, 200, 999, 1.5), TYPE_SQUARE, 1, 700, QUAD_SPECIAL3));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.7;
			Spw[0].getObject()->getColorData()[1] = 0.7;
			Spw[0].getObject()->getColorData()[2] = 0.7;
			worldn = 1;
			pointmark = Point(WINDOW_WIDTH/2, 135);
			
			convertStringToLetterData("Zone Test", 10, 10, linesize, title, titlemark, true);
		} break; **/
		default:
			worldn = 1;
	}
	convertPointData(title, 0, titlemark);
	for (int i = 0; i < worldn; i++)
		worldtime[i].resetTime();
	healthBar.setDim1(-1);
}

//Fungsi pause
bool paused() {
	if (disabled) { //Pause draw
		if (!freeze) {
			timeeffect.markTime();
			for (int i = 0; i < worldn; i++)
				worldtime[i].markTime();
			Sq.setBegin();
			for (int i = 0; i < Sq.size(); i++) {
				Sq.getIValue().markTime();
				Sq.next();
			}
			Tr.setBegin();
			for (int i = 0; i < Tr.size(); i++) {
				Tr.getIValue().markTime();
				Tr.next();
			}
			Qu.setBegin();
			for (int i = 0; i < Qu.size(); i++) {
				Qu.getIValue().markTime();
				Qu.next();
			}
			Shots.setBegin();
			for (int i = 0; i < Shots.size(); i++) {
				Shots.getIValue()->markTime();
				Shots.next();
			}
			defSpw.setBegin();
			for (int i = 0; i < defSpw.size(); i++) {
				defSpw.getIValue().getSpawnTimer().markTime();
				defSpw.getIValue().getObject()->markTime();
				defSpw.next();
			}
			subSpw.setBegin();
			for (int i = 0; i < subSpw.size(); i++) {
				subSpw.getIValue().getSpawnTimer().markTime();
				subSpw.getIValue().getObject()->markTime();
				subSpw.next();
			}
			Spw.setBegin();
			for (int i = 0; i < Spw.size(); i++) {
				Spw.getIValue().getSpawnTimer().markTime();
				Spw.getIValue().getObject()->markTime();
				Spw.next();
			}
			freeze = true;
		}
		glVertexPointer (2, GL_FLOAT, 0, pausetext);
		glLineWidth(7);
		glDrawArrays(GL_LINES, 0, pausemark[2]/2);
		glLineWidth(5);
		glDrawArrays(GL_LINES, pausemark[1]/2, (pausemark[2]-pausemark[1])/2);
		return true;
	} else
		return false;
}

//Fungsi resumer
void unpause() {
	disabled = false;
	freeze = false;
	timeeffect.continueTime();
	for (int i = 0; i < worldn; i++)
		worldtime[i].continueTime();
	Sq.setBegin();
	for (int i = 0; i < Sq.size(); i++) {
		Sq.getIValue().continueTime();
		Sq.next();
	}
	Tr.setBegin();
	for (int i = 0; i < Tr.size(); i++) {
		Tr.getIValue().continueTime();
		Tr.next();
	}
	Qu.setBegin();
	for (int i = 0; i < Qu.size(); i++) {
		Qu.getIValue().continueTime();
		Qu.next();
	}
	Shots.setBegin();
	for (int i = 0; i < Shots.size(); i++) {
		Shots.getIValue()->continueTime();
		Shots.next();
	}
	defSpw.setBegin();
	for (int i = 0; i < defSpw.size(); i++) {
		defSpw.getIValue().getSpawnTimer().continueTime();
		defSpw.getIValue().getObject()->continueTime();
		defSpw.next();
	}
	subSpw.setBegin();
	for (int i = 0; i < subSpw.size(); i++) {
		subSpw.getIValue().getSpawnTimer().continueTime();
		subSpw.getIValue().getObject()->continueTime();
		subSpw.next();
	}
	Spw.setBegin();
	for (int i = 0; i < Spw.size(); i++) {
		Spw.getIValue().getSpawnTimer().continueTime();
		Spw.getIValue().getObject()->continueTime();
		Spw.next();
	}
}

//Fungsi show timer
void drawTimer() {
	glColor4f(0.7,0.7,0.7,1.0);
	linesize = 2;
	timemark[0] = 0;
	timemark[1] = timemark[0];
	convertStringToLetterData(timeeffect.toString(), WINDOW_WIDTH - getStringWidth(timeeffect.toString(), linesize), 10, linesize, timetext, timemark[1], false);
	convertPointData(timetext, 0, timemark[1]);
	glLineWidth(linesize);
	glVertexPointer (2, GL_FLOAT, 0, timetext);
	glDrawArrays(GL_LINES, 0, timemark[1]/2);
}

void drawTitle() {
	glColor4f(0.7,0.7,0.7,1.0);
	glLineWidth(2);
	glVertexPointer (2, GL_FLOAT, 0, title);
	glDrawArrays(GL_LINES, 0, titlemark/2);
}

//Game over
void gameOver() {
	if (!storymode) {
		string dummy;
		timeeffect.markTime();
		double timescore = timeeffect.getElapsed();
		double data;
		ifstream RecordData;
		ofstream Record;
		RecordData.open("highscore.txt");
		RecordData >> dummy >> dummy >> dummy >> dummy;
		RecordData >> data;
		if (rounds > data) {
			RecordData.close();
			Record.open("highscore.txt");
			Record << "World: ";
			switch (gamemode) {
				case ID_LEVEL_A :
					Record << "Level_A";
				break;
				case ID_RAPIDSHOOTER :
					Record << "Rapid_Shooter";
				break;
				case ID_DANMAKU1 :
					Record << "Danmaku_Spiral";
				break;
				case ID_RINGSHOOTER :
					Record << "Ring_Shooter";
				break;
				case ID_DEADLYHEARTS :
					Record << "Death_Hearts";
				break;
				case ID_DRONEDFIGHTER :
					Record << "Droned_Fighter";
				break;
				case ID_CHASER :
					Record << "Chaser";
				break;
				case ID_RIPPLES :
					Record << "Ripples";
				break;
				case ID_RAIN :
					Record << "Bullet_Rain";
				break;
				case ID_LASERBLOOM :
					Record << "Laser_Bloom";
				break;
				case ID_RADIANCE :
					Record << "Radiance";
				break;
				case ID_SPIKECAGE :
					Record << "Spike_Cage";
				break;
				case ID_ICESPIRIT :
					Record << "Ice_Spirit";
				break;
				case ID_EARTHSPIRIT :
					Record << "Earth_Spirit";
				break;
				case ID_DANMAKU2 :
					Record << "Danmaku_Fireworks";
				break;
				case ID_PETAL :
					Record << "Petal_Barrage";
				break;
				case ID_DASHER :
					Record << "Dasher";
				break;
				case ID_LEVEL_B :
					Record << "Level_B";
				break;
				case ID_LEVEL_C :
					Record << "Level_C";
				break;
				case ID_ASTRALCAGE :
					Record << "Astral_Cage";
				break;
				case ID_TIMESORCERER :
					Record << "Time_Sorcerer";
				break;
				case ID_SPIRALMADNESS :
					Record << "Spiral_Madness";
				break;
				case ID_ARROWMASTER :
					Record << "Arrow_Master";
				break;
				default:
					Record << "Zone";
			}
			Record << endl;
			Record << "Rounds completed: " << rounds << endl;
			Record << "Date: " << getNowTime();
			Record << "Shot type: ";
			if (typeB) {
				Record << typeB_str << endl;
			} else if (typeC) {
				Record << typeC_str << endl;
			} else if (typeD) {
				Record << typeD_str << endl;
			} else {
				Record << typeA_str << endl;
			}
			Record.close();
		}
		Record.open("record.txt", ios::out | ios::app);
		Record << "World: ";
		switch (gamemode) {
			case ID_LEVEL_A :
				Record << title_LevelA_str;
			break;
			case ID_RAPIDSHOOTER :
				Record << title_RapidShooter_str;
			break;
			case ID_DANMAKU1 :
				Record << title_Danmaku1_str;
			break;
			case ID_RINGSHOOTER :
				Record << title_RingShooter_str;
			break;
			case ID_DEADLYHEARTS :
				Record << title_DeathHearts_str;
			break;
			case ID_DRONEDFIGHTER :
				Record << title_Drone_str;
			break;
			case ID_CHASER :
				Record << title_Chaser_str;
			break;
			case ID_RIPPLES :
				Record << title_Rippler_str;
			break;
			case ID_RAIN :
				Record << title_Rain_str;
			break;
			case ID_LASERBLOOM :
				Record << title_LaserBloom_str;
			break;
			case ID_RADIANCE :
				Record << title_Radiance_str;
			break;
			case ID_SPIKECAGE :
				Record << title_SpikeCage_str;
			break;
			case ID_ICESPIRIT :
				Record << title_IceSpirit_str;
			break;
			case ID_EARTHSPIRIT :
				Record << title_EarthSpirit_str;
			break;
			case ID_DANMAKU2 :
				Record << title_Danmaku2_str;
			break;
			case ID_PETAL :
				Record << title_PetalBarrage_str;
			break;
			case ID_DASHER :
				Record << title_Dasher_str;
			break;
			case ID_LEVEL_B :
				Record << title_LevelB_str;
			break;
			case ID_LEVEL_C :
				Record << title_LevelC_str;
			break;
			case ID_ASTRALCAGE :
				Record << title_AstralCage_str;
			break;
			case ID_TIMESORCERER :
				Record << title_TimeSorcerer_str;
			break;
			case ID_SPIRALMADNESS :
				Record << title_SpiralMadness_str;
			break;
			case ID_ARROWMASTER :
				Record << title_ArrowMaster_str;
			break;
			default:
				Record << "Zone";
		}
		Record << endl;
		Record << "Rounds completed: " << rounds << endl;
		Record << "Survival time: " << timescore << endl;
		Record << "Bombs remaining: " << bombcount << endl;
		Record << "Date: " << getNowTime();
		Record << "Shot type: ";
		if (typeB) {
			Record << typeB_str << endl;
		} else if (typeC) {
			Record << typeC_str << endl;
		} else if (typeD) {
			Record << typeD_str << endl;
		} else {
			Record << typeA_str << endl;
		}
		Record << endl;
		Record.close();
		
		linesize = 4;
		gameovermark[2] = gameovermark[1];
		timeeffect.continueTime();
		string str = "Time: " + timeeffect.toString();
		convertStringToLetterData(str, WINDOW_WIDTH/2 - getStringWidth(str, linesize)/2, WINDOW_HEIGHT*5/7, linesize, gameovertext, gameovermark[2], true);
		string str2 = "Rounds completed: " + to_string(rounds);
		gameovermark[3] = gameovermark[2];
		convertStringToLetterData(str2, WINDOW_WIDTH/2 - getStringWidth(str2, linesize)/2, WINDOW_HEIGHT*6/7, linesize, gameovertext, gameovermark[3], true);
		convertPointData(gameovertext, gameovermark[1], gameovermark[3]);
	}
	rounds = -1;
	gameover = true;
	delaytime.resetTime();
}

//Fungsi clear level
void clearLevel(int gmode) {
	pointmark = Point(0,0);
	if (gmode <= 0 || gamemode <= 0 || gmode > LEVEL_LIMIT || gamemode > LEVEL_LIMIT) {
		Sq.removeAll();
		Tr.removeAll();
		Qu.removeAll();
		spawnerClear(subSpw);
		patternClear(Ptt);
		if (gmode <= 0 || gmode > ZONE_LIMIT)
			shotsClear(Shots);
	}
	spawnerClear(defSpw);
	spawnerClear(Spw);
	worldn = 0;
	if (gmode == ID_GAMEOVER)
		glColor4f(1.0,0.0,0.0,1.0);
	else
		glColor4f(1.0,1.0,1.0,1.0);
	glVertexPointer (2, GL_FLOAT, 0, borders);
	glDrawArrays(GL_QUADS, 0, 4);
}

//Bomb effect, mirip clear level
void bomb() {
	Sq.setBegin();
	for (int i = 0; i < Sq.size(); i++) {
		if (Sq.getIValue().isMaterialised())
			Sq.getIValue().getLifeTimer().setTime(999000);
		Sq.next();
	}
	
	Tr.setBegin();
	for (int i = 0; i < Tr.size(); i++) {
		if (Tr.getIValue().isMaterialised())
			Tr.getIValue().getLifeTimer().setTime(999000);
		Tr.next();
	}
	
	Qu.setBegin();
	for (int i = 0; i < Qu.size(); i++) {
		if (Qu.getIValue().isMaterialised())
			Qu.getIValue().getLifeTimer().setTime(999000);
		Qu.next();
	}
	subSpw.setBegin();
	for (int i = 0; i < subSpw.size(); i++) {
		if (subSpw.getIValue().getObject()->isMaterialised())
			subSpw.getIValue().getObject()->getLifeTimer().setTime(999000);
		subSpw.next();
	}
	Spw.setBegin();
	for (int i = 0; i < Spw.size(); i++) {
		if (Spw.getIValue().getObject()->isMaterialised())
			Spw.getIValue().damage(1);
		Spw.next();
	}
	Sq.add(SquareObj(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 2000, 0.05, 1));
	Sq[0].getColorData()[1] = 0.0;
	Sq[0].getColorData()[2] = 0.0;
}

//Switch levels
void switchLevel(int gmode) {
	if (gmode == ID_MAINMENU)
		confirmcheck = false;
	clearLevel(gmode);
	gamemode = gmode;
	createLevel();
	patterner = 0;
	if ((gamemode <= 0) || (gamemode > ZONE_LIMIT)) {
		autoshoot = false;
		enableBar = false;
		bombcount = 2;
	}
	if (gmode > ZONE_LIMIT)
		delaytime.resetTime();
	if (storymode) {
		bool hitcheck = true;
		switch(gamemode) {
			case ID_STORY2:
			case ID_STORY3:
			case ID_STORY4:
			case ID_STORY5:
			case ID_STORY6:
			case ID_STORY7:
			case ID_STORY8:
			case ID_STORY9:
			case ID_STORY10:
				checkl = gamemode;
			break;
			case ID_MAINMENU:
				storymode = false;
			default:
				hitcheck = false;
		}
		if (hitcheck) {
			ofstream saveFile;
			saveFile.open("savedata");
			saveFile << checkl << endl;
			saveFile.close();
		}
	} else if (gmode > 0 && gmode < ZONE_LIMIT) {
		rounds++;
	}
}

void switchArcade() { /**
	if (rounds%10 == 8) {
		int x = randomInt(0, levelID.size()-1);
		switchLevel(levelID[x]);
		levelID.removeAt(x);
		
		switch (rounds) {
			case 9 : setZNormal(); setLNormal();
			break;
			case 19 : setLHard();
			break;
			case 29 : setZHard();
			break;
			case 39 : setLTouhou();
			break;
			case 49 : setZTouhou();
			break;
			default: setZTouhou(); setLTouhou();
		}
	} else {
		if (rounds%10 == 9 && bombcount < 2)
			bombcount++;
		
		int x = randomInt(0, changeID.size()-1);
		switchLevel(changeID[x]);
		changeID.removeAt(x);
	} **/
	
	if (bombcount < 2)
		bombcount++;
	
	switch (rounds) {
		case 1 : setLNormal();
		break;
		case 5 : setLHard();
		break;
		case 9 : setLTouhou();
		break;
		case 11: rounds = 8;
			setLTouhou();
		break;
	}
	
	int x = randomInt(0, levelID.size()-1);
	switchLevel(levelID[x]);
	levelID.removeAt(x);
}


/* Subprogram berhubungan dengan penggunaan OpenGL (dan GLFW) */
void errCallback(int error, const char* desc) {
	cerr << "Error: " << desc << endl;
}

void keyCallback(GLFWwindow* window, int key, int scanc, int act, int mods) {
	if (act == GLFW_PRESS) {
		switch (gamemode) {
			case ID_MAINMENU :
				switch(key) {
					case GLFW_KEY_S : {
						if (confirmcheck || checkl == ID_MAINMENU || checkl == ID_STORY1) {
							storymode = true;
							switchLevel(ID_STORY1);
							ofstream saveFile;
							saveFile.open("savedata");
							checkl = ID_STORY1;
							saveFile << checkl << endl;
							saveFile.close();
						} else if (!confirmcheck) {
							confirmcheck = true;
						}
					} break;
					case GLFW_KEY_L :
						storymode = true;
						switchLevel(checkl);
					break;
					case GLFW_KEY_A :
						setZEasy();
						setLEasy();
						switchArcade();
						timeeffect.resetTime();
					break;
					case GLFW_KEY_H :
						switchLevel(ID_HELP);
					break;
					case GLFW_KEY_W :
						linesize = 2;
						shottypemark = 0;
						if (typeC && spacepressed) {
							typeC = false;
							typeD = true;
							SHOT_DAMAGE = SHOT_DAMAGED;
							SHOT_DELAY = SHOT_DELAYD;
							convertStringToLetterData((shottype_str + typeD_str), WINDOW_WIDTH/2 - getStringWidth((shottype_str + typeD_str), linesize)/2, WINDOW_HEIGHT/2, linesize, shottypetext, shottypemark, true);
						} else if (typeC || typeD) {
							typeC = false;
							typeD = false;
							SHOT_DAMAGE = SHOT_DAMAGEA;
							SHOT_DELAY = SHOT_DELAYA;
							convertStringToLetterData((shottype_str + typeA_str), WINDOW_WIDTH/2 - getStringWidth((shottype_str + typeA_str), linesize)/2, WINDOW_HEIGHT/2, linesize, shottypetext, shottypemark, true);
						} else if (typeB) {
							typeB = false;
							typeC = true;
							SHOT_DAMAGE = SHOT_DAMAGEC;
							SHOT_DELAY = SHOT_DELAYC;
							convertStringToLetterData((shottype_str + typeC_str), WINDOW_WIDTH/2 - getStringWidth((shottype_str + typeC_str), linesize)/2, WINDOW_HEIGHT/2, linesize, shottypetext, shottypemark, true);
						} else {
							typeB = true;
							SHOT_DAMAGE = SHOT_DAMAGEB;
							SHOT_DELAY = SHOT_DELAYB;
							convertStringToLetterData((shottype_str + typeB_str), WINDOW_WIDTH/2 - getStringWidth((shottype_str + typeB_str), linesize)/2, WINDOW_HEIGHT/2, linesize, shottypetext, shottypemark, true);
						}
						convertPointData(shottypetext, 0, shottypemark);
					break;
					case GLFW_KEY_ESCAPE :
						glfwSetWindowShouldClose(window, GLFW_TRUE);
					break;
					case GLFW_KEY_SPACE :
						spacepressed = true;
					break;
				}
			break;
			case ID_HELP :
				switchLevel(ID_MAINMENU);
			break;
			case ID_GAMEOVER :
			case ID_WIN :
				switch (key) {
					case GLFW_KEY_ESCAPE :
						glfwSetWindowShouldClose(window, GLFW_TRUE);
					break;
					default :
						if (delaytime.getElapsed() > 1) {
							switchLevel(ID_MAINMENU);
						}
				}
			break;
			default :
				if (key == GLFW_KEY_T) {
					autoshoot = !autoshoot;
				} else if (key == GLFW_KEY_B) {
					enableBar = !enableBar;
				} else if (gamemode < ZONE_LIMIT && key == GLFW_KEY_X) {
					if (bombcount > 0) {
						bomb();
						bombcount--;
					}
				}
				if (key == GLFW_KEY_P && !storymode) {
					if (!disabled && !gameover) {
						disabled = true;
					} else {
						unpause();
					}
				} else if (key == GLFW_KEY_SPACE && !gameover && !disabled) {
					spacepressed = true;
				}
		}
	} else if (key == GLFW_KEY_SPACE && act == GLFW_RELEASE) {
		spacepressed = false;
	}
}

void enterCallback(GLFWwindow* window, int entered) {
	if (!entered && !disabled && !storymode && !gameover && (gamemode > 0) && (gamemode <= ZONE_LIMIT)) { //Strict limited
		disabled = true;
	}
}

void drawCursor() {
	glColor3f(1.0,0.0,0.0);
	cursorpoint[0] = widthConvert(xpos-CURSOR_SIZE);
	cursorpoint[1] = heightConvert(ypos-CURSOR_SIZE);
	cursorpoint[2] = widthConvert(xpos);
	cursorpoint[3] = heightConvert(ypos-CURSOR_DIAG);
	cursorpoint[4] = widthConvert(xpos+CURSOR_SIZE);
	cursorpoint[5] = heightConvert(ypos-CURSOR_SIZE);
	cursorpoint[6] = widthConvert(xpos+CURSOR_DIAG);
	cursorpoint[7] = heightConvert(ypos);
	cursorpoint[8] = widthConvert(xpos+CURSOR_SIZE);
	cursorpoint[9] = heightConvert(ypos+CURSOR_SIZE);
	cursorpoint[10] = widthConvert(xpos);
	cursorpoint[11] = heightConvert(ypos+CURSOR_DIAG);
	cursorpoint[12] = widthConvert(xpos-CURSOR_SIZE);
	cursorpoint[13] = heightConvert(ypos+CURSOR_SIZE);
	cursorpoint[14] = widthConvert(xpos-CURSOR_DIAG);
	cursorpoint[15] = heightConvert(ypos);
	
	glVertexPointer (2, GL_FLOAT, 0, cursorpoint);
	glDrawArrays(GL_POLYGON, 0, 8);
}

void checkDamaged() {
	///Check shot damage
	int i = 0;
	bool check;
	float healthdisplay = -10;
	Shots.setBegin();
	while (i < Shots.size()) {
		check = false;
		int j = 0;
		Spw.setBegin();
		while (j < Spw.size() && !check) {
			if (Spw.getIValue().getObject()->isMaterialised() && Spw.getIValue().getObject()->isIn(*(Shots.getIValue()))) {
				check = true;
				Spw.getIValue().damage(SHOT_DAMAGE);
				healthdisplay = Spw.getIValue().getHealth()/Spw.getIValue().getMaxHealth();
			} else {
				j++;
				Spw.next();
			}
		}
		if (check) {
			delete Shots.getIValue();
			Shots.removeIValue();
		} else {
			i++;
			Shots.next();
		}
	}
	if (healthdisplay > -10) {
		healthBar.setDim1(healthdisplay*(WINDOW_WIDTH-10));
		healthBar.setX(healthBar.getDim1()/2 + 5);
	}
}

void drawObjects() {
	///Removal check then draw
	
	int i = 0;
	defSpw.setBegin();
	while (i < defSpw.size()) {
		if (defSpw.getIValue().getObject()->isDead()) {
			if (defSpw.getIValue().getSpawnType() > LBOUND_EXPLOSION)
				defSpw.getIValue().spawnObject();
			delete defSpw.getIValue().getObject();
			defSpw.removeIValue();
		} else {
			defSpw.getIValue().getObject()->redraw();
			glVertexPointer (2, GL_FLOAT, 0, defSpw.getIValue().getObject()->getPointData());
			if (defSpw.getIValue().getObject()->isMaterialised())
				glColor4f(defSpw.getIValue().getObject()->getColorData()[0],defSpw.getIValue().getObject()->getColorData()[1],defSpw.getIValue().getObject()->getColorData()[2],1.0);
			else
				glColor4f(defSpw.getIValue().getObject()->getColorData()[0],defSpw.getIValue().getObject()->getColorData()[1],defSpw.getIValue().getObject()->getColorData()[2],0.3);
			switch (defSpw.getIValue().getObjType()) {
				case TYPE_SQUARE:
				case TYPE_QUAD:
					glDrawArrays(GL_QUADS, 0, 4);
				break;
				case TYPE_TRIANGLE:
					glDrawArrays(GL_TRIANGLES, 0, 3);
				break;
			}
			if (defSpw.getIValue().getObject()->isMaterialised() && defSpw.getIValue().getSpawnTimer().getElapsed() > defSpw.getIValue().getSpawnTime())
				defSpw.getIValue().spawnObject();	
			defSpw.next();
			i++;
		}
	}
	
	i = 0;
	Spw.setBegin();
	while (i < Spw.size()) {
		if (Spw.getIValue().getObject()->isDead()) {
			delete Spw.getIValue().getObject();
			Spw.removeIValue();
		} else if (Spw.getIValue().getHealth() <= 0) {
			if (Spw.getIValue().getSpawnType() > LBOUND_EXPLOSION)
				Spw.getIValue().spawnObject();
			delete Spw.getIValue().getObject();
			Spw.removeIValue();
		} else {
			Spw.getIValue().getObject()->redraw();
			glVertexPointer (2, GL_FLOAT, 0, Spw.getIValue().getObject()->getPointData());
			if (Spw.getIValue().isDamaged())
				glColor4f(0.6,0.0,0.0,1.0);
			else if (Spw.getIValue().getObject()->isMaterialised())
				glColor4f(Spw.getIValue().getObject()->getColorData()[0],Spw.getIValue().getObject()->getColorData()[1],Spw.getIValue().getObject()->getColorData()[2],1.0);
			else
				glColor4f(Spw.getIValue().getObject()->getColorData()[0],Spw.getIValue().getObject()->getColorData()[1],Spw.getIValue().getObject()->getColorData()[2],0.3);
			switch (Spw.getIValue().getObjType()) {
				case TYPE_SQUARE:
				case TYPE_QUAD:
					glDrawArrays(GL_QUADS, 0, 4);
				break;
				case TYPE_TRIANGLE:
					glDrawArrays(GL_TRIANGLES, 0, 3);
				break;
			}
			if (Spw.getIValue().getObject()->isMaterialised() && (Spw.getIValue().getSpawnTimer().getElapsed() > Spw.getIValue().getSpawnTime()))
				Spw.getIValue().spawnObject();
			Spw.next();
			i++;
		}
	}
	
	i = 0;
	Qu.setBegin();
	while (i < Qu.size()) {
		if (Qu.getIValue().isDead()) {
			Qu.removeIValue();
		} else {
			Qu.getIValue().redraw();
			glVertexPointer (2, GL_FLOAT, 0, Qu.getIValue().getPointData());
			if (Qu.getIValue().isMaterialised())
				glColor4f(Qu.getIValue().getColorData()[0],Qu.getIValue().getColorData()[1],Qu.getIValue().getColorData()[2],1.0);
			else
				glColor4f(Qu.getIValue().getColorData()[0],Qu.getIValue().getColorData()[1],Qu.getIValue().getColorData()[2],0.3);
			glDrawArrays(GL_QUADS, 0, 4);
			Qu.next();
			i++;
		}
	}
	
	i = 0;
	Sq.setBegin();
	while (i < Sq.size()) {
		if (Sq.getIValue().isDead()) {
			Sq.removeIValue();
		} else {
			Sq.getIValue().redraw();
			glVertexPointer (2, GL_FLOAT, 0, Sq.getIValue().getPointData());
			if (Sq.getIValue().isMaterialised())
				glColor4f(Sq.getIValue().getColorData()[0],Sq.getIValue().getColorData()[1],Sq.getIValue().getColorData()[2],1.0);
			else
				glColor4f(Sq.getIValue().getColorData()[0],Sq.getIValue().getColorData()[1],Sq.getIValue().getColorData()[2],0.3);
			glDrawArrays(GL_QUADS, 0, 4);
			Sq.next();
			i++;
		}
	}
	
	i = 0;
	Tr.setBegin();
	while (i < Tr.size()) {
		if (Tr.getIValue().isDead()) {
			Tr.removeIValue();
		} else {
			Tr.getIValue().redraw();
			glVertexPointer (2, GL_FLOAT, 0, Tr.getIValue().getPointData());
			if (Tr.getIValue().isMaterialised())
				glColor4f(Tr.getIValue().getColorData()[0],Tr.getIValue().getColorData()[1],Tr.getIValue().getColorData()[2],1.0);
			else
				glColor4f(Tr.getIValue().getColorData()[0],Tr.getIValue().getColorData()[1],Tr.getIValue().getColorData()[2],0.3);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			Tr.next();
			i++;
		}
	}
	
	subSpw.setBegin();
	while (i < subSpw.size()) {
		if (subSpw.getIValue().getObject()->isDead()) {
			if (subSpw.getIValue().getSpawnType() > LBOUND_EXPLOSION)
				subSpw.getIValue().spawnObject();
			delete subSpw.getIValue().getObject();
			subSpw.removeIValue();
		} else {
			subSpw.getIValue().getObject()->redraw();
			glVertexPointer (2, GL_FLOAT, 0, subSpw.getIValue().getObject()->getPointData());
			if (subSpw.getIValue().getObject()->isMaterialised())
				glColor4f(subSpw.getIValue().getObject()->getColorData()[0],subSpw.getIValue().getObject()->getColorData()[1],subSpw.getIValue().getObject()->getColorData()[2],1.0);
			else
				glColor4f(subSpw.getIValue().getObject()->getColorData()[0],subSpw.getIValue().getObject()->getColorData()[1],subSpw.getIValue().getObject()->getColorData()[2],0.3);
			switch (subSpw.getIValue().getObjType()) {
				case TYPE_SQUARE:
				case TYPE_QUAD:
					glDrawArrays(GL_QUADS, 0, 4);
				break;
				case TYPE_TRIANGLE:
					glDrawArrays(GL_TRIANGLES, 0, 3);
				break;
			}
			if (subSpw.getIValue().getObject()->isMaterialised() && subSpw.getIValue().getSpawnTimer().getElapsed() > subSpw.getIValue().getSpawnTime())
				subSpw.getIValue().spawnObject();	
			subSpw.next();
			i++;
		}
	}
	
	i = 0;
	Ptt.setBegin();
	while (i < Ptt.size()) {
		Ptt.getIValue().checkMemberDead();
		if (Ptt.getIValue().isDead()) {
			delete[] Ptt.getIValue().getObjData();
			Ptt.removeIValue();
		} else {
			Ptt.next();
			i++;
		}
	}
	
	i = 0;
	Shots.setBegin();
	while (i < Shots.size()) {
		if (Shots.getIValue()->isDead()) {
			delete Shots.getIValue();
			Shots.removeIValue();
		} else {
			Shots.getIValue()->redraw();
			glVertexPointer (2, GL_FLOAT, 0, Shots.getIValue()->getPointData());
			glColor4f(0,1,1,0.8);
			if (typeB || typeC || typeD)
				glDrawArrays(GL_QUADS, 0, 4);
			else
				glDrawArrays(GL_TRIANGLES, 0, 3);
			Shots.next();
			i++;
		}
	}
	
	if (enableBar && !Spw.isEmpty() && healthBar.getDim1() >= 1) {
		healthBar.redraw();
		glVertexPointer (2, GL_FLOAT, 0, healthBar.getPointData());
		glColor4f(0.0, 1.0, 0.0, 0.7);
		glDrawArrays(GL_QUADS, 0, 4);
		glLineWidth(1);
		glColor4f(1.0, 1.0, 1.0, 0.7);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}
}

void moveObjects() {
	Ptt.setBegin();
	for (int i = 0; i < Ptt.size(); i++) {
		Ptt.getIValue().move();
		Ptt.next();
	}
	Sq.setBegin();
	for (int i = 0; i < Sq.size(); i++) {
		Sq.getIValue().move();
		Sq.next();
	}
	Tr.setBegin();
	for (int i = 0; i < Tr.size(); i++) {
		Tr.getIValue().move();
		Tr.next();
	}
	Qu.setBegin();
	for (int i = 0; i < Qu.size(); i++) {
		Qu.getIValue().move();
		Qu.next();
	}
	Shots.setBegin();
	for (int i = 0; i < Shots.size(); i++) {
		Shots.getIValue()->move();
		Shots.next();
	}
	defSpw.setBegin();
	for (int i = 0; i < defSpw.size(); i++) {
		defSpw.getIValue().getObject()->move();
		defSpw.next();
	}
	subSpw.setBegin();
	for (int i = 0; i < subSpw.size(); i++) {
		subSpw.getIValue().getObject()->move();
		subSpw.next();
	}
	Spw.setBegin();
	for (int i = 0; i < Spw.size(); i++) {
		Spw.getIValue().getObject()->move();
		Spw.next();
	}
}

void normalDraw() {
	checkDamaged();
	drawObjects();
}
						

int main() {
	
	//Inisialisasi GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	
	//Enable Switch Task
	glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	
	//Get Monitor
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	
	//Set parameters
	WINDOW_HEIGHT = mode->height;
	WINDOW_WIDTH = mode->width;
	Dimensions(WINDOW_WIDTH, WINDOW_HEIGHT);
	
	//Create window
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "A N T I (v_0.8)", glfwGetPrimaryMonitor(), NULL);
	
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	//Set context window yang ingin di-apakan
	glfwMakeContextCurrent(window);
	
	//Set callback untuk input keyboard
	glfwSetKeyCallback(window, keyCallback);
	
	//Set enter/leave callback
	glfwSetCursorEnterCallback(window, enterCallback);
	
	//Hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	
	//interval per swap buffer
	glfwSwapInterval(1);
	
	//Enable stuff
	glEnableClientState (GL_VERTEX_ARRAY);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Text initialize
	linesize = 7;
	gameovermark[0] = 0;
	gameovermark[1] = gameovermark[0];
	convertStringToLetterData(gameover1, WINDOW_WIDTH/2 - getStringWidth(gameover1, linesize)/2, WINDOW_HEIGHT/2-8, linesize, gameovertext, gameovermark[1], false);
	convertPointData(gameovertext, 0, gameovermark[1]);
	
	winmark[0] = 0;
	winmark[1] = winmark[0];
	convertStringToLetterData(wintext1, WINDOW_WIDTH/2 - getStringWidth(wintext1, linesize)/2, WINDOW_HEIGHT/4, linesize, wintext, winmark[1], false);
	winmark[2] = winmark[1];
	convertStringToLetterData(wintext2, WINDOW_WIDTH/2 - getStringWidth(wintext2, linesize)/2, WINDOW_HEIGHT*2/3, linesize, wintext, winmark[2], false);
	winmark[3] = winmark[2];
	convertStringToLetterData(wintext3, WINDOW_WIDTH/2 - getStringWidth(wintext3, 2)/2, WINDOW_HEIGHT*9/10, 2, wintext, winmark[3], false);
	convertPointData(wintext, 0, winmark[3]);
	
	pausemark[0] = 0;
	pausemark[1] = pausemark[0];
	convertStringToLetterData(pause1, WINDOW_WIDTH/2 - getStringWidth(pause1, linesize)/2, WINDOW_HEIGHT/3, linesize, pausetext, pausemark[1], false);
	linesize = 5;
	pausemark[2] = pausemark[1];
	convertStringToLetterData(pause2, WINDOW_WIDTH/2 - getStringWidth(pause2, linesize)/2, WINDOW_HEIGHT*2/3, linesize, pausetext, pausemark[2], true);
	convertPointData(pausetext, 0, pausemark[2]);
	
	linesize = 7;
	mainmenumark[0] = 0;
	mainmenumark[1] = mainmenumark[0];
	convertStringToLetterData(mainmenu1, WINDOW_WIDTH/2 - getStringWidth(mainmenu1, linesize)/2, WINDOW_HEIGHT/4, linesize, mainmenutext, mainmenumark[1], false);
	linesize = 3;
	mainmenumark[2] = mainmenumark[1];
	convertStringToLetterData(mainmenu2, WINDOW_WIDTH/2 - getStringWidth(mainmenu2, linesize)/2, WINDOW_HEIGHT*6/10, linesize, mainmenutext, mainmenumark[2], true);
	mainmenumark[3] = mainmenumark[2];
	convertStringToLetterData(mainmenu3, WINDOW_WIDTH/2 - getStringWidth(mainmenu3, linesize)/2, WINDOW_HEIGHT*7/10, linesize, mainmenutext, mainmenumark[3], true);
	mainmenumark[4] = mainmenumark[3];
	convertStringToLetterData(mainmenu4, WINDOW_WIDTH/2 - getStringWidth(mainmenu4, linesize)/2, WINDOW_HEIGHT*8/10, linesize, mainmenutext, mainmenumark[4], true);
	mainmenumark[5] = mainmenumark[4];
	convertStringToLetterData(mainmenu5, WINDOW_WIDTH/2 - getStringWidth(mainmenu5, linesize)/2, WINDOW_HEIGHT*9/10, linesize, mainmenutext, mainmenumark[5], true);
	mainmenumark[6] = mainmenumark[5];
	convertStringToLetterData(mainmenu6, WINDOW_WIDTH/2 + getStringWidth(mainmenu2, linesize)/2 + 5, WINDOW_HEIGHT*6/10, linesize, mainmenutext, mainmenumark[6], true);
	convertPointData(mainmenutext, 0, mainmenumark[6]);
	
	linesize = 2;
	helpmark[0] = 0;
	helpmark[1] = helpmark[0];
	convertStringToLetterData(help1, 30, WINDOW_HEIGHT/10, linesize, helptext, helpmark[1], true);
	helpmark[2] = helpmark[1];
	convertStringToLetterData(help2, 30, WINDOW_HEIGHT*2/10, linesize, helptext, helpmark[2], true);
	helpmark[3] = helpmark[2];
	convertStringToLetterData(help3, 30, WINDOW_HEIGHT*3/10, linesize, helptext, helpmark[3], true);
	helpmark[4] = helpmark[3];
	convertStringToLetterData(help4, 30, WINDOW_HEIGHT*4/10, linesize, helptext, helpmark[4], true);
	helpmark[5] = helpmark[4];
	convertStringToLetterData(help5, 30, WINDOW_HEIGHT*5/10, linesize, helptext, helpmark[5], true);
	helpmark[6] = helpmark[5];
	convertStringToLetterData(help6, 30, WINDOW_HEIGHT*6/10, linesize, helptext, helpmark[6], true);
	helpmark[7] = helpmark[6];
	convertStringToLetterData(help7, 30, WINDOW_HEIGHT*7/10, linesize, helptext, helpmark[7], true);
	helpmark[8] = helpmark[7];
	convertStringToLetterData(help8, 30, WINDOW_HEIGHT*8/10, linesize, helptext, helpmark[8], true);
	helpmark[9] = helpmark[8];
	convertStringToLetterData(help9, 30, WINDOW_HEIGHT*9/10, linesize, helptext, helpmark[9], true);
	convertPointData(helptext, 0, helpmark[9]);
	
	//Story texts
	linesize = STORY_LINESIZE;
	
	story1mark[0] = 0;
	story1mark[1] = story1mark[0];
	convertStringToLetterData(story1_str1, WINDOW_WIDTH/2 - getStringWidth(story1_str1, linesize)/2, WINDOW_HEIGHT*2/5, linesize, story1text, story1mark[1], true);
	story1mark[2] = story1mark[1];
	convertStringToLetterData(story1_str2, WINDOW_WIDTH/2 - getStringWidth(story1_str2, linesize)/2, WINDOW_HEIGHT*3/5, linesize, story1text, story1mark[2], true);
	convertPointData(story1text, 0, story1mark[2]);
	
	story2mark[0] = 0;
	story2mark[1] = story2mark[0];
	convertStringToLetterData(story2_str1, WINDOW_WIDTH/2 - getStringWidth(story2_str1, linesize)/2, WINDOW_HEIGHT/2, linesize, story2text, story2mark[1], true);
	convertPointData(story2text, 0, story2mark[1]);
	
	story3mark[0] = 0;
	story3mark[1] = story3mark[0];
	convertStringToLetterData(story3_str1, WINDOW_WIDTH/2 - getStringWidth(story3_str1, linesize)/2, WINDOW_HEIGHT/2, linesize, story3text, story3mark[1], true);
	convertPointData(story3text, 0, story3mark[1]);
	
	story4mark[0] = 0;
	story4mark[1] = story4mark[0];
	convertStringToLetterData(story4_str1, WINDOW_WIDTH/2 - getStringWidth(story4_str1, linesize)/2, WINDOW_HEIGHT/2, linesize, story4text, story4mark[1], true);
	convertPointData(story4text, 0, story4mark[1]);
	
	story5mark[0] = 0;
	story5mark[1] = story5mark[0];
	convertStringToLetterData(story5_str1, WINDOW_WIDTH/2 - getStringWidth(story5_str1, linesize)/2, WINDOW_HEIGHT/2, linesize, story5text, story5mark[1], true);
	convertPointData(story5text, 0, story5mark[1]);
	
	story6mark[0] = 0;
	story6mark[1] = story6mark[0];
	convertStringToLetterData(story6_str1, WINDOW_WIDTH/2 - getStringWidth(story6_str1, linesize)/2, WINDOW_HEIGHT*2/5, linesize, story6text, story6mark[1], true);
	story6mark[2] = story6mark[1];
	convertStringToLetterData(story6_str2, WINDOW_WIDTH/2 - getStringWidth(story6_str2, linesize)/2, WINDOW_HEIGHT*3/5, linesize, story6text, story6mark[2], true);
	convertPointData(story6text, 0, story6mark[2]);
	
	story7mark[0] = 0;
	story7mark[1] = story7mark[0];
	convertStringToLetterData(story7_str1, WINDOW_WIDTH/2 - getStringWidth(story7_str1, linesize)/2, WINDOW_HEIGHT/2, linesize, story7text, story7mark[1], true);
	convertPointData(story7text, 0, story7mark[1]);
	
	story8mark[0] = 0;
	story8mark[1] = story8mark[0];
	convertStringToLetterData(story8_str1, WINDOW_WIDTH/2 - getStringWidth(story8_str1, linesize)/2, WINDOW_HEIGHT/2, linesize, story8text, story8mark[1], true);
	convertPointData(story8text, 0, story8mark[1]);
	
	story9mark[0] = 0;
	story9mark[1] = story9mark[0];
	convertStringToLetterData(story9_str1, WINDOW_WIDTH/2 - getStringWidth(story9_str1, linesize)/2, WINDOW_HEIGHT/2, linesize, story9text, story9mark[1], true);
	convertPointData(story9text, 0, story9mark[1]);
	
	story10mark[0] = 0;
	story10mark[1] = story10mark[0];
	convertStringToLetterData(story10_str1, WINDOW_WIDTH/2 - getStringWidth(story10_str1, linesize)/2, WINDOW_HEIGHT/2, linesize, story10text, story10mark[1], true);
	convertPointData(story10text, 0, story10mark[1]);
	
	//Initialize
	int iter;
	xpos = WINDOW_WIDTH/2;
	ypos = WINDOW_HEIGHT/2;
	
	ifstream loadFile;
	loadFile.open("savedata");
	loadFile >> checkl;
	loadFile.close();
	
	if (checkl == 0)
		gamemode = ID_HELP;
	else
		gamemode = ID_MAINMENU;
	
	bombcount = 2;
	SHOT_DAMAGE = SHOT_DAMAGEA;
	SHOT_DELAY = SHOT_DELAYA;
	typeB = false;
	typeC = false;
	typeD = false;
	
	linesize = 2;
	shottypemark = 0;
	convertStringToLetterData((shottype_str + typeA_str), WINDOW_WIDTH/2 - getStringWidth((shottype_str + typeA_str), linesize)/2, WINDOW_HEIGHT/2, linesize, shottypetext, shottypemark, true);
	convertPointData(shottypetext, 0, shottypemark);
	
	healthBar.setY(3);
	healthBar.setDim2(4);
	
	while (!glfwWindowShouldClose(window)) {
		
		//Membersihkan buffer
		if (!gameover || gamemode == ID_GAMEOVER)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Menghapus segala transformasi
		glLoadIdentity();
		
		//Proses
		
		///Proses mouse
		if (!paused()) {
			glfwGetCursorPos(window, &newxpos, &newypos);
			float x = newxpos - xpos;
			float y = newypos - ypos;
			float dist = sqrt(x*x + y*y);
			if (dist > 1) {
				float val = atan2(y, x);
				xpos = xpos + dist/4*cos(val);
				ypos = ypos + dist/4*sin(val);
			} else {
				xpos = newxpos;
				ypos = newypos;
			}
		}
		
		///Shooting
		if (!disabled && !gameover && gamemode > 0 && gamemode < ZONE_LIMIT && (spacepressed || autoshoot) && delaytime.getElapsed() > SHOT_DELAY) {
			if (typeB) {
				for (int i = 0; i < 3; i++) {
					Shots.add(new QuadObj(xpos, ypos-CURSOR_SIZE-3, 20, 7, 2));
					Shots[0]->setDirection(75 + 15*i);
					Shots[0]->setAngle(75 + 15*i);
					Shots[0]->setSpeed(10);
				}
			} else if (typeC) {
				for (int i = 0; i < 3; i++) {
					Shots.add(new SquareObj(xpos, ypos-CURSOR_SIZE-3, 11+3*i, 1+0.7*i));
					Shots[0]->setDirection(90);
					Shots[0]->setAngle(90);
					Shots[0]->setSpeed(14-5*i);
				}
			} else if (typeD) {
				for (int i = 0; i < 32; i++) {
					Shots.add(new SquareObj(xpos, ypos-CURSOR_SIZE-3, 13, 3));
					Shots[0]->setDirection(11.25*i);
					Shots[0]->setAngle(11.25*i);
					Shots[0]->setSpeed(8);
				}
			} else {
				Shots.add(new TriangleObj(xpos, ypos-CURSOR_SIZE-3, 19, 11, 2));
				Shots[0]->setDirection(90);
				Shots[0]->setAngle(90);
				Shots[0]->setSpeed(11.5);
			}
			delaytime.resetTime();
		}
		
		switch (gamemode) {
			case ID_MAINMENU: 
				glVertexPointer (2, GL_FLOAT, 0, mainmenutext);
				glLineWidth(7);
				glDrawArrays(GL_LINES, 0, mainmenumark[1]/2);
				glLineWidth(3);
				glDrawArrays(GL_LINES, mainmenumark[1]/2, (mainmenumark[5]-mainmenumark[1])/2);
				if (confirmcheck) {
					glColor3f(1.0, 0.1, 0.1);
					glDrawArrays(GL_LINES, mainmenumark[5]/2, (mainmenumark[6]-mainmenumark[5])/2);
					glColor3f(1.0,1.0,1.0);
				}
				glLineWidth(2);
				glVertexPointer(2, GL_FLOAT, 0, shottypetext);
				glDrawArrays(GL_LINES, 0, shottypemark/2);
				drawCursor();
			break;
			case ID_ZONE1: {
				if (!paused() && !gameover && worldtime[0].getElapsed() > 7) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_ZONE3);
				} else if (!gameover) {
					if (!paused()) {
						double y;
						if (worldtime[1].getElapsed() > 0.19) {
							int x = randomInt(1,4);
							switch (x) {
								case 1:
									defSpw[0].getObject()->setX(WINDOW_WIDTH/2);
									defSpw[0].getObject()->setY(-100);
								break;
								case 2:
									defSpw[0].getObject()->setX(WINDOW_WIDTH/2);
									defSpw[0].getObject()->setY(WINDOW_HEIGHT+100);
								break;
								case 3:
									defSpw[0].getObject()->setX(-100);
									defSpw[0].getObject()->setY(WINDOW_HEIGHT/2);
								break;
								case 4:
									defSpw[0].getObject()->setX(WINDOW_WIDTH+100);
									defSpw[0].getObject()->setY(WINDOW_HEIGHT/2);
								break;
							}
							if (worldtime[0].getElapsed() > 4)
								y = randomFloat(-25,25);
							else
								y = 0;
							worldtime[1].resetTime();
						}
						if (worldtime[0].getElapsed() > 4) {
							defSpw[0].setSpawnTime(0.22);
						}
						defSpw[0].getObject()->setAngle(-defSpw[0].getObject()->angleTo(Point(xpos, ypos))+y);
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_LEVEL_B: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if ((Spw[0].getObject()->distanceSquaredTo(pointmark) < 4) || (Spw[0].getObject()->getSpeed() > 5 && Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5)) {
							if ((randomInt(0,1) == 0 && Spw[0].getObject()->getX() < WINDOW_WIDTH-500) || Spw[0].getObject()->getX() < 500)
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()+150, Spw[0].getObject()->getX()+220), randomFloat(100, 200));
							else
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()-150, Spw[0].getObject()->getX()-220), randomFloat(100, 200));
							Spw[0].getObject()->setSpeed(sqrt(Spw[0].getObject()->distanceSquaredTo(pointmark))/200);
							Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(pointmark));
						}
						if (worldtime[1].getElapsed() > 0.4) {
							if (Spw[0].getHealth() > 100) {
								int x = randomInt(0,3);
								defSpw[x].getObject()->setAngle(-defSpw[x].getObject()->angleTo(Point(xpos, ypos)));
								defSpw[x].spawnObject();
							} else {
								int x = randomInt(0,1);
								for (int i = 0; i < 2; i++) {
									defSpw[x + 2*i].getObject()->setAngle(-defSpw[x + 2*i].getObject()->angleTo(Point(xpos, ypos)));
									defSpw[x + 2*i].spawnObject();
								}
							}
							worldtime[1].resetTime();
						}
						Spw[0].getObject()->setAngle(-Spw[0].getObject()->angleTo(Point(xpos, ypos)));
						
						if (Spw[0].getHealth() <= 100 + SHOT_THRESHOLD && Spw[0].getHealth() > 100) {
							Spw[0].damage(SHOT_THRESHOLD);
							glColor4f(1.0,1.0,1.0,1.0);
							glVertexPointer (2, GL_FLOAT, 0, borders);
							glDrawArrays(GL_QUADS, 0, 4);
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE1B: {
				if (!paused() && !gameover && worldtime[0].getElapsed() > 8) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_BULLET1);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() < 7) {
							if (worldtime[1].getElapsed() >= defSpw[0].getSpawnTime()) {
								if (worldtime[0].getElapsed() > 0.8) {
									int x = randomInt(1,2);
									switch (x) {
										case 1:
											defSpw[0].getObject()->setX(WINDOW_WIDTH+100);
											defSpw[0].getObject()->setY(WINDOW_HEIGHT/2);
											defSpw[1].getObject()->setX(-100);
											defSpw[1].getObject()->setY(WINDOW_HEIGHT/2);
										break;
										case 2:
											defSpw[0].getObject()->setX(WINDOW_WIDTH/2);
											defSpw[0].getObject()->setY(-100);
											defSpw[1].getObject()->setX(WINDOW_WIDTH/2);
											defSpw[1].getObject()->setY(WINDOW_HEIGHT+100);
										break;
									}
								}
								double y[2];
								if (worldtime[0].getElapsed() > 4) {
									y[0] = randomFloat(-40,40);
									y[1] = randomFloat(-40,40);
								} else {
									y[0] = 0;
									y[1] = 0;
								}
								defSpw.setBegin();
								for (int i = 0; i < defSpw.size(); i++) {
									defSpw.getIValue().getObject()->setAngle(-defSpw.getIValue().getObject()->angleTo(Point(xpos, ypos))+y[i]);
									if (worldtime[0].getElapsed() > 4 && worldtime[0].getElapsed() < 4.1) {
										defSpw.getIValue().setSpawnTime(0.24);
									}
									defSpw.next();
								}
								worldtime[1].resetTime();
							}
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_LEVEL_A: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode) {
						switchArcade();
					} else {
						switchLevel(ID_STORY3);
					}
				} else if (!gameover) {
					if (!paused()) {
						defSpw.setBegin();
						for (int i = 0; i < 9; i++) {
							defSpw.getIValue().getObject()->setAngle(-worldtime[0].getElapsed()*72);
							defSpw.next();
						}
						Spw[0].getObject()->setAngle(worldtime[0].getElapsed()*108);
						for (int i = 0; i < 10; i++) {
							defSpw.getIValue().getObject()->setAngle(-worldtime[0].getElapsed()*36);
							defSpw.next();
						}
						for (int i = 0; i < 10; i++) {
							defSpw.getIValue().getObject()->setAngle(worldtime[0].getElapsed()*36);
							defSpw.next();
						}						
						
						defSpw.setBegin();
						for (int i = 0; i < 9; i++) {
							if (defSpw.getIValue().getObject()->getY() > WINDOW_HEIGHT+50*2)
								defSpw.getIValue().getObject()->setY(-50*2);
							defSpw.next();
						}
						Spw[0].getObject()->setSpeed(sqrt(Spw[0].getObject()->distanceSquaredTo(Point(xpos, ypos)))/100+1.5);
						Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(Point(xpos, ypos))); //Karena y terbalik menurut terminal

						for (int i = 0; i < 10; i++) {
							if (defSpw.getIValue().getObject()->getY() < -50*2)
								defSpw.getIValue().getObject()->setY(WINDOW_HEIGHT+50*2);
							defSpw.next();
						}
						for (int i = 0; i < 10; i++) {
							if (i%2 == 0) {
								if (defSpw.getIValue().getObject()->getX() > WINDOW_WIDTH+(150 - 10*i)*2)
									defSpw.getIValue().getObject()->setX(-(150 - 10*i)*2);
							} else {
								if (defSpw.getIValue().getObject()->getX() < -(50 + 10*(i-1))*2)
									defSpw.getIValue().getObject()->setX(WINDOW_WIDTH+(50 + 10*(i-1))*2);
							}
							defSpw.next();
						}
						
						if (Spw[0].getHealth() <= 120 + SHOT_THRESHOLD && Spw[0].getHealth() > 120) {
							Spw[0].damage(SHOT_THRESHOLD);
							defSpw.setBegin();
							for (int i = 0; i < 19; i++) {
								defSpw.getIValue().getObject()->setTime(301000);
								defSpw.next();
							}
							glColor4f(1.0,1.0,1.0,1.0);
							glVertexPointer (2, GL_FLOAT, 0, borders);
							glDrawArrays(GL_QUADS, 0, 4);
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_CHASER: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode) {
						switchArcade();
					} else {
						switchLevel(0);
					}
				} else if (!gameover) {
					if (!paused()) {
						float a = (xpos - Spw[0].getObject()->getX())*(-sin(convertToRadian(Spw[0].getObject()->getDirection()))) - (ypos - Spw[0].getObject()->getY())*(cos(convertToRadian(Spw[0].getObject()->getDirection())));
						if (abs(a) > 90) {
							Spw[0].getObject()->setSpeed(Spw[0].getObject()->getSpeed()*199/200);
						} else {
							Spw[0].getObject()->setSpeed(Spw[0].getObject()->getSpeed()*201/200 + 0.05);
						}
						if (a > 20)
							Spw[0].getObject()->setDirection(Spw[0].getObject()->getDirection() + 2);
						else if (a < -20)
							Spw[0].getObject()->setDirection(Spw[0].getObject()->getDirection() - 2);
						else if (a > 10)
							Spw[0].getObject()->setDirection(Spw[0].getObject()->getDirection() + 0.1);
						else if (a < -10)
							Spw[0].getObject()->setDirection(Spw[0].getObject()->getDirection() - 0.1);
						Spw[0].getObject()->setAngle(Spw[0].getObject()->getDirection());
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_BULLET1: {
				if (!paused() && !gameover && worldtime[0].getElapsed() > 6) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_BULLET2);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() < 4.99) {
							if (worldtime[0].getElapsed() > 3 && worldtime[0].getElapsed() < 3.1) {
								defSpw.setBegin();
								for (int i = 0; i < defSpw.size(); i++) {
									defSpw.getIValue().setSpawnTime(0.3);
									defSpw.next();
								}
							}
							defSpw.setBegin();
							for (int i = 0; i < defSpw.size(); i++) {
								defSpw.getIValue().getObject()->setAngle(-defSpw[i].getObject()->angleTo(Point(xpos, ypos)));
								defSpw.next();
							}
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE2B : {
				if (!paused() && !gameover && worldtime[0].getElapsed() > 4) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_ZONE4);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[1].getElapsed() > 0.1) {
							float x = randomFloat(0.8,1.5);
							Sq.add(SquareObj(xpos + randomDouble(-200, 200), ypos + randomDouble(-250, 250), randomInt(150, 300), x+0.5, x));
							Sq[0].setAngle(randomFloat(0, 360));
							worldtime[1].resetTime();
						}
						Sq.setBegin();
						for (int i = 0; i < Sq.size(); i++) {
							if (!Sq.getIValue().isMaterialised())
								Sq.getIValue().setAngle(Sq.getIValue().getAngle() + 1);
							Sq.next();
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE2 : {
				if (!paused() && !gameover && worldtime[0].getElapsed() > 4) {
					switchArcade();
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[1].getElapsed() > 0.25) {
							float x = randomFloat(1.4,2);
							Sq.add(SquareObj(xpos + randomDouble(-200, 200), ypos + randomDouble(-200, 200), randomInt(150, 300), x+0.5, x));
							Sq[0].setAngle(randomFloat(0, 360));
							worldtime[1].resetTime();
						}
						Sq.setBegin();
						for (int i = 0; i < Sq.size(); i++) {
							if (!Sq.getIValue().isMaterialised())
								Sq.getIValue().setAngle(Sq.getIValue().getAngle() + 2);
							Sq.next();
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_SHOOTER1: {
				if (!paused() && !gameover && (Spw.isEmpty() || worldtime[0].getElapsed() > 11)) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_ZONE6);
				} else if (!gameover) {
					if (!paused()) {
						Spw[0].getObject()->setAngle(worldtime[0].getElapsed()*830);
						if (worldtime[0].getElapsed() < 4 && Spw[0].getObject()->getY() >= WINDOW_HEIGHT/3) {
							Spw[0].getObject()->setSpeed(0);
							Spw[0].getObject()->setY(WINDOW_HEIGHT/3-0.01);
						} else if (worldtime[0].getElapsed() > 8) {
							Spw[0].getObject()->setSpeed(5);
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE3 : {
				if (!paused() && !gameover && (worldtime[0].getElapsed() > 8 || (worldtime[0].getElapsed() > 4 && Spw.isEmpty()))) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_ZONE2);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() < 5 && worldtime[1].getElapsed() > 0.1) {
							float x = randomFloat(-250, WINDOW_WIDTH+250);
							Spw.add(Spawner2D(new TriangleObj(x, -100, 100, 50, 7), TYPE_TRIANGLE, 999, 1, NULL_SHOT));
							float ang = -Spw[0].getObject()->angleTo(Point(xpos, ypos));
							Spw[0].getObject()->setSpeed(7+worldtime[0].getElapsed()/2);
							Spw[0].getObject()->setAngle(ang);
							Spw[0].getObject()->setDirection(ang);
							Spw[0].getObject()->getColorData()[0] = 0.0;
							Spw[0].getObject()->getColorData()[2] = 0.0;
							worldtime[1].resetTime();
						}
						for (int i = 0; i < Spw.size(); i++) {
							if (Spw[i].getObject()->getY() < 100 && Spw[i].getObject()->getY() < ypos) {
								float ang = Spw[i].getObject()->getAngle() + (-Spw[i].getObject()->angleTo(Point(xpos, ypos)) - Spw[i].getObject()->getAngle())/5;
								Spw[i].getObject()->setDirection(ang);
								Spw[i].getObject()->setAngle(ang);
							}
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE4 : {
				if (!paused() && !gameover && worldtime[0].getElapsed() > 7) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_ZONE5);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() < 3.5 && worldtime[1].getElapsed() > 0.5) {
							Spw.add(Spawner2D(new SquareObj(100, -150, 120, 1.7, 0.1), TYPE_SQUARE, 0.6, 2, SQUARE_AIMSHOT1));
							Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH-100, -150, 120, 1.7, 0.1), TYPE_SQUARE, 0.6, 2, SQUARE_AIMSHOT1));
							Spw.setBegin();
							for (int i = 0; i < 2; i++) {
								Spw.getIValue().getObject()->setSpeed(10);
								Spw.getIValue().getObject()->setAngle(270);
								Spw.getIValue().getObject()->setDirection(270);
								Spw.getIValue().getObject()->getColorData()[0] = 0.0;
								Spw.getIValue().getObject()->getColorData()[2] = 0.4;
								Spw.next();
							}
							worldtime[1].resetTime();
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE5 : {
				if (!paused() && !gameover && (worldtime[0].getElapsed() > 4.5 && Spw.isEmpty())) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_SHOOTER1);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() < 4 && worldtime[1].getElapsed() > 0.2) {
							Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -120, 120, 100, 8, 0.5), TYPE_TRIANGLE, randomFloat(2, 5), 8, SQUARE_AIMSHOT1));
							Spw[0].getObject()->getColorData()[0] = 0.4;
							Spw[0].getObject()->getColorData()[2] = 0.0;
							worldtime[1].resetTime();
						}
						Spw.setBegin();
						for (int i = 0; i < Spw.size(); i++) {
							Spw.getIValue().getObject()->setSpeed(7 + 7*cos(Spw.getIValue().getObject()->getTimeElapsed()*2));
							float a = 270 + 85*cos(Spw.getIValue().getObject()->getTimeElapsed());
							Spw.getIValue().getObject()->setAngle(a);
							Spw.getIValue().getObject()->setDirection(a);
							Spw.next();
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE6 : {
				if (!paused() && !gameover && worldtime[0].getElapsed() > 5) {
					switchLevel(ID_RAPIDSHOOTER);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() < 0.5 && Qu.isEmpty()) {
							Qu.add(QuadObj(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 550, WINDOW_HEIGHT+20, 1.5, 3));
							Qu.add(QuadObj(WINDOW_WIDTH/2, 0, 550, 50, 1.5, 3));
							Qu[0].getColorData()[1] = 0.0;
							Qu[0].getColorData()[2] = 0.0;
							Sq.add(SquareObj(WINDOW_WIDTH/2, -650, 500, 2));
							Sq[0].setSpeed(15);
							Sq[0].setDirection(270);
							Sq[0].setAngle(45);
						} else if (worldtime[0].getElapsed() < 1.6 && Qu.isEmpty()) {
							Qu.add(QuadObj(WINDOW_WIDTH/6, WINDOW_HEIGHT/2, 550, WINDOW_HEIGHT+20, 1.5, 3));
							Qu.add(QuadObj(WINDOW_WIDTH/6, 0, 550, 50, 1.5, 3));
							Qu[0].getColorData()[1] = 0.0;
							Qu[0].getColorData()[2] = 0.0;
							Qu.add(QuadObj(WINDOW_WIDTH*5/6, WINDOW_HEIGHT/2, 550, WINDOW_HEIGHT+20, 1.5, 3));
							Qu.add(QuadObj(WINDOW_WIDTH*5/6, 0, 550, 50, 1.5, 3));
							Qu[0].getColorData()[1] = 0.0;
							Qu[0].getColorData()[2] = 0.0;
							Sq.add(SquareObj(WINDOW_WIDTH/6, -650, 500, 2));
							Sq.add(SquareObj(WINDOW_WIDTH*5/6, -650, 500, 2));
							Sq.setBegin();
							for (int i = 0; i < 2; i++) {
								Sq.getIValue().setSpeed(15);
								Sq.getIValue().setDirection(270);
								Sq.getIValue().setAngle(45);
								Sq.next();
							}
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE6B : {
				if (!paused() && !gameover && worldtime[0].getElapsed() > 5.3) {
					switchArcade();
				} else if (!gameover) {
					if (!paused()) {
						bool rev;
						if (worldtime[0].getElapsed() < 2.6)
							rev = false;
						else
							rev = true;
						if (worldtime[0].getElapsed() > 0.5) {
							if (worldtime[0].getElapsed() < 1.2+2.1*rev && Qu.isEmpty()) {
								Qu.add(QuadObj(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 400, WINDOW_HEIGHT+20, 0.71, 3));
								Qu.add(QuadObj(WINDOW_WIDTH/2, rev*WINDOW_HEIGHT, 400, 50, 0.71, 3));
								Qu[0].getColorData()[1] = 0.0;
								Qu[0].getColorData()[2] = 0.0;
								Sq.add(SquareObj(WINDOW_WIDTH/2, rev*(WINDOW_HEIGHT+1000)-500, 350, 2));
								Sq[0].setSpeed(15);
								if (rev)
									Sq[0].setDirection(90);
								else
									Sq[0].setDirection(270);
								Sq[0].setAngle(45);
							} else if (worldtime[0].getElapsed() < 1.9+2.1*rev && Qu.isEmpty()) {
								Qu.add(QuadObj(WINDOW_WIDTH*3/10, WINDOW_HEIGHT/2, 400, WINDOW_HEIGHT+20, 0.71, 3));
								Qu.add(QuadObj(WINDOW_WIDTH*3/10, rev*WINDOW_HEIGHT, 400, 50, 0.71, 3));
								Qu[0].getColorData()[1] = 0.0;
								Qu[0].getColorData()[2] = 0.0;
								Qu.add(QuadObj(WINDOW_WIDTH*7/10, WINDOW_HEIGHT/2, 400, WINDOW_HEIGHT+20, 0.71, 3));
								Qu.add(QuadObj(WINDOW_WIDTH*7/10, rev*WINDOW_HEIGHT, 400, 50, 0.71, 3));
								Qu[0].getColorData()[1] = 0.0;
								Qu[0].getColorData()[2] = 0.0;
								Sq.add(SquareObj(WINDOW_WIDTH*3/10, rev*(WINDOW_HEIGHT+1000)-500, 350, 2));
								Sq.add(SquareObj(WINDOW_WIDTH*7/10, rev*(WINDOW_HEIGHT+1000)-500, 350, 2));
								Sq.setBegin();
								for (int i = 0; i < 2; i++) {
									Sq.getIValue().setSpeed(15);
									if (rev)
										Sq.getIValue().setDirection(90);
									else
										Sq.getIValue().setDirection(270);
									Sq.getIValue().setAngle(45);
									Sq.next();
								}
							} else if (worldtime[0].getElapsed() < 2.6+2.1*rev && Qu.isEmpty()) {
								Qu.add(QuadObj(WINDOW_WIDTH/10, WINDOW_HEIGHT/2, 400, WINDOW_HEIGHT+20, 0.71, 3));
								Qu.add(QuadObj(WINDOW_WIDTH/10, rev*WINDOW_HEIGHT, 400, 50, 0.71, 3));
								Qu[0].getColorData()[1] = 0.0;
								Qu[0].getColorData()[2] = 0.0;
								Qu.add(QuadObj(WINDOW_WIDTH*9/10, WINDOW_HEIGHT/2, 400, WINDOW_HEIGHT+20, 0.71, 3));
								Qu.add(QuadObj(WINDOW_WIDTH*9/10, rev*WINDOW_HEIGHT, 400, 50, 0.71, 3));
								Qu[0].getColorData()[1] = 0.0;
								Qu[0].getColorData()[2] = 0.0;
								Sq.add(SquareObj(WINDOW_WIDTH/10, rev*(WINDOW_HEIGHT+1000)-500, 350, 2));
								Sq.add(SquareObj(WINDOW_WIDTH*9/10, rev*(WINDOW_HEIGHT+1000)-500, 350, 2));
								Sq.setBegin();
								for (int i = 0; i < 2; i++) {
									Sq.getIValue().setSpeed(15);
									if (rev)
										Sq.getIValue().setDirection(90);
									else
										Sq.getIValue().setDirection(270);
									Sq.getIValue().setAngle(45);
									Sq.next();
								}
							}
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE8: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() > 1 && Spw[0].getObject()->getSpeed() > 5.5) {
							Spw.setBegin();
							for (int i = 0; i < Spw.size(); i++) {
								Spw.getIValue().getObject()->setSpeed(0);
								Spw.next();
							}
						} else if (worldtime[0].getElapsed() < 2) {
							Spw.setBegin();
							for (int i = 0; i < Spw.size(); i++) {
								Spw.getIValue().getObject()->setAngle(-Spw.getIValue().getObject()->angleTo(Point(xpos, ypos)));
								Spw.next();
							}
						} else if (worldtime[0].getElapsed() >= 2 && worldtime[0].getElapsed() < 6 && Spw[0].getSpawnTime() > 10) {
							Spw.setBegin();
							for (int i = 0; i < Spw.size(); i++) {
								Spw.getIValue().setSpawnTime(0.07);
								Spw.next();
							}
						} else if (worldtime[0].getElapsed() >= 6 && Spw[0].getObject()->getSpeed() < 1) {
							Spw.setBegin();
							for (int i = 0; i < Spw.size(); i++) {
								Spw.getIValue().setSpawnTime(999);
								Spw.getIValue().getObject()->setSpeed(5);
								Spw.next();
							}
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_BULLET2: {
				if (!paused() && !gameover && Spw.isEmpty() && worldtime[1].getElapsed() > 1.5) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_LEVEL_A);
				} else if (!gameover) {
					if (!paused()) {
						if (!Spw.isEmpty()) {
							if (Spw[0].getObject()->getAngle() > 90) {
								Spw[0].getObject()->setAngle(Spw[0].getObject()->getAngle()+5);
								if (Spw[0].getObject()->getAngle() > 450)
									Spw[0].getObject()->setAngle(89);
							} else {
								Spw[0].getObject()->setAngle(Spw[0].getObject()->getAngle()-5);
								if (Spw[0].getObject()->getAngle() < -270)
									Spw[0].getObject()->setAngle(91);
							}
							if (worldtime[0].getElapsed() > 18) {
								Spw[0].getObject()->setSpeed(5);
							} else if (Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5) {
								Spw[0].getObject()->setSpeed(0);
								Spw[0].getObject()->setY(WINDOW_HEIGHT/5-0.01);
							}
						}
						checkDamaged();
						drawObjects();
						if (Spw.isEmpty() && worldtime[1].getElapsed() > 2)
							worldtime[1].resetTime();
					}
					drawCursor();
				}
			} break;
			case ID_LEVEL_C: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (Spw[0].getObject()->getSpeed() > 5 && worldtime[0].getElapsed() > 1) {
							Spw.setBegin();
							for (int i = 0; i < 4; i++) {
								Spw.getIValue().getObject()->setSpeed(3);
								Spw.getIValue().getObject()->setDirection(90*i);
								Spw.next();
							}
						}
						Spw.setBegin();
						for (int i = 0; i < Spw.size(); i++) {
							Spw.getIValue().getObject()->setAngle(-Spw.getIValue().getObject()->angleTo(Point(xpos, ypos)));
							switch ((int)Spw.getIValue().getObject()->getDirection()) {
								case 0:
									if (Spw.getIValue().getObject()->getX() > WINDOW_WIDTH-50)
										Spw.getIValue().getObject()->setDirection(90);
								break;
								case 90:
									if (Spw.getIValue().getObject()->getY() < 50)
										Spw.getIValue().getObject()->setDirection(180);
								break;
								case 180:
									if (Spw.getIValue().getObject()->getX() < 50)
										Spw.getIValue().getObject()->setDirection(270);
								break;
								case 270:
									if (Spw.getIValue().getObject()->getY() > WINDOW_HEIGHT-50)
										Spw.getIValue().getObject()->setDirection(0);
								break;
							}
							Spw.next();
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_SHOOTER2: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					switchArcade();
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() > 7 && worldtime[0].getElapsed() < 7.1) {
							Spw.setBegin();
							for (int i = 0; i < Spw.size(); i++) {
								Spw.getIValue().getObject()->setSpeed(4);
								Spw.next();
							}
						} else if (worldtime[0].getElapsed() < 1 && Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5) {
							Spw.setBegin();
							for (int i = 0; i < Spw.size(); i++) {
								Spw.getIValue().getObject()->setSpeed(0);
								Spw.getIValue().getObject()->setY(WINDOW_HEIGHT/5-0.01);
								Spw.next();
							}
						}
						Spw.setBegin();
						for (int i = 0; i < Spw.size(); i++) {
							Spw.getIValue().getObject()->setAngle(-Spw.getIValue().getObject()->angleTo(Point(xpos, ypos)));
							Spw.next();
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_SHOOTER3: {
				if (!paused() && !gameover && (Spw.isEmpty() && worldtime[0].getElapsed() > 4)) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() > 0.9 && worldtime[0].getElapsed() < 1) {
							Spw.setBegin();
							for (int i = 0; i < Spw.size(); i++) {
								Spw.getIValue().getObject()->setSpeed(2);
								Spw.next();
							}
						}
						Spw.setBegin();
						for (int i = 0; i < Spw.size(); i++) {
							Spw.getIValue().getObject()->setAngle(worldtime[0].getElapsed()*360);
							Spw.next();
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_SHOOTER4: {
				if (!paused() && !gameover && Spw.isEmpty() && worldtime[1].getElapsed() > 2) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_SHOOTER1);
				} else if (!gameover) {
					if (!paused()) {
						if (!Spw.isEmpty()) {
							Spw[0].getObject()->setAngle(-Spw[0].getObject()->angleTo(Point(xpos, ypos)));
							if (worldtime[0].getElapsed() < 18.1) {
								if (worldtime[0].getElapsed() > 18) {
									Spw[0].getObject()->setSpeed(3.0);
									Spw[0].getObject()->setDirection(270);
								} else if ((Spw[0].getObject()->distanceSquaredTo(pointmark) < 1) || (Spw[0].getObject()->getSpeed() > 1.3 && Spw[0].getObject()->getY() >= WINDOW_HEIGHT/8)) {
									pointmark = Point(randomFloat(300, WINDOW_WIDTH-300), randomFloat(50, 100));
									Spw[0].getObject()->setSpeed(1.0);
									Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(pointmark));
								} 
							}
						}
						checkDamaged();
						drawObjects();
						if (Spw.isEmpty() && worldtime[1].getElapsed() > 3)
							worldtime[1].resetTime();
					}
					drawCursor();
				}
			} break;
			case ID_SHOOTER5: {
				if (!paused() && !gameover && (Spw.isEmpty() && worldtime[0].getElapsed() > 1)) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (!Spw.isEmpty()) {
							if (Spw[0].getObject()->getSpeed() > 3 && Spw[0].getObject()->getY() > WINDOW_HEIGHT/10) {
								Spw.setBegin();
								for (int i = 0; i < Spw.size(); i++) {
									Spw.getIValue().getObject()->setSpeed(1);
									Spw.getIValue().getSpawnTimer().setTime(randomFloat(0.0, 1500.0));
									Spw.next();
								}
							}
						}
						checkDamaged();
						drawObjects();
						if (Spw.isEmpty() && worldtime[0].getElapsed() > 2)
							worldtime[0].resetTime();
					}
					drawCursor();
				}
			} break;
			case ID_SHOOTER7: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						Spw[0].getObject()->setAngle(-Spw[0].getObject()->angleTo(Point(xpos, ypos)));
						if (Spw[0].getObject()->getSpeed() > 5 && worldtime[0].getElapsed() > 0.7) {
							Spw[0].getObject()->setSpeed(0);
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_SHOOTER8: {
				if (!paused() && !gameover && Spw.isEmpty() && worldtime[1].getElapsed() > 2) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (!Spw.isEmpty()) {
							if (worldtime[0].getElapsed() < 15.1) {
								if (worldtime[0].getElapsed() > 15) {
									Spw[0].getObject()->setSpeed(3.0);
									Spw[0].getObject()->setDirection(270);
								} else if (Spw[0].getObject()->distanceSquaredTo(pointmark) < 10) {
									if ((randomInt(0,1) == 0 && Spw[0].getObject()->getX() < WINDOW_WIDTH-400) || Spw[0].getObject()->getX() < 400)
										pointmark = Point(randomFloat(Spw[0].getObject()->getX()+160, Spw[0].getObject()->getX()+240), randomFloat(90, 180));
									else
										pointmark = Point(randomFloat(Spw[0].getObject()->getX()-160, Spw[0].getObject()->getX()-240), randomFloat(90, 180));
									Spw[0].getObject()->setSpeed(sqrt(Spw[0].getObject()->distanceSquaredTo(pointmark))/200);
									Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(pointmark));
								}
							}
						}
						checkDamaged();
						drawObjects();
						if (Spw.isEmpty() && worldtime[1].getElapsed() > 2.5)
							worldtime[1].resetTime();
					}
					drawCursor();
				}
			} break;
			case ID_SHOOTER8B: {
				if (!paused() && !gameover && Spw.isEmpty() && worldtime[1].getElapsed() > 2) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (!Spw.isEmpty()) {
							if (worldtime[0].getElapsed() < 19.1) {
								if (worldtime[0].getElapsed() > 19) {
									Ptt[Ptt.size()-1].setSpeed(3.0);
									Ptt[Ptt.size()-1].setDirection(270);
								} else if (Ptt[Ptt.size()-1].distanceSquaredTo(pointmark) < 10) {
									if ((randomInt(0,1) == 0 && Ptt[Ptt.size()-1].getX() < WINDOW_WIDTH-350) || Ptt[Ptt.size()-1].getX() < 350)
										pointmark = Point(randomFloat(Ptt[Ptt.size()-1].getX()+160, Ptt[Ptt.size()-1].getX()+240), randomFloat(90, 180));
									else
										pointmark = Point(randomFloat(Ptt[Ptt.size()-1].getX()-160, Ptt[Ptt.size()-1].getX()-240), randomFloat(90, 180));
									Ptt[Ptt.size()-1].setSpeed(sqrt(Ptt[Ptt.size()-1].distanceSquaredTo(pointmark))/200);
									Ptt[Ptt.size()-1].setDirection(-Ptt[Ptt.size()-1].angleTo(pointmark));
								}
							}
						}
						checkDamaged();
						drawObjects();
						if (Spw.isEmpty() && worldtime[1].getElapsed() > 2.5)
							worldtime[1].resetTime();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE10: {
				if (!paused() && !gameover && (defSpw.isEmpty() && worldtime[0].getElapsed() > 1.5)) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						checkDamaged();
						drawObjects();
						if (defSpw.isEmpty() && worldtime[0].getElapsed() > 2)
							worldtime[0].resetTime();
					}
					drawCursor();
				}
			} break;
			case ID_RAPIDSHOOTER: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_STORY2);
				} else if (!gameover) {
					if (!paused()) {
						Spw[0].getObject()->setAngle(-Spw[0].getObject()->angleTo(Point(xpos, ypos)));
						if (worldtime[0].getElapsed() < 1 && Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5) {
							Spw[0].getObject()->setSpeed(0);
							Spw[0].getObject()->setY(WINDOW_HEIGHT/5-0.01);
						}
						if (Spw[0].getSpawnTime() > 0.03)
							Spw[0].setSpawnTime(0.12-worldtime[0].getElapsed()/200);
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_RINGSHOOTER: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						Spw[0].getObject()->setAngle(-Spw[0].getObject()->angleTo(Point(xpos, ypos)));
						if ((Spw[0].getObject()->distanceSquaredTo(pointmark) < 4) || (Spw[0].getObject()->getSpeed() > 1 && Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5)) {
							pointmark = Point(randomFloat(400, WINDOW_WIDTH-400), randomFloat(50, 200));
							Spw[0].getObject()->setSpeed(0.8);
							Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(pointmark));
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_DEADLYHEARTS: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (Spw[0].getObject()->getY() > 150) {
							Spw.setBegin();
							for (int i = 0; i < 2; i++) {
								Spw.getIValue().getObject()->setSpeed(0);
								Spw.getIValue().getObject()->setY(150);
								Spw.next();
							}
							Spw[0].getSpawnTimer().setTime(750);
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_DRONEDFIGHTER: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						defSpw.setBegin();
						for (int i = 0; i < defSpw.size(); i++) {
							defSpw.getIValue().getObject()->setAngle(-defSpw.getIValue().getObject()->angleTo(Point(xpos, ypos)));
							defSpw.next();
						}
						if (Spw[0].getObject()->getY() <= 150 && defSpw[0].getObject()->getX() > WINDOW_WIDTH/2+250) {
							defSpw.setBegin();
							for (int i = 0; i < defSpw.size(); i++) {
								defSpw.getIValue().getObject()->setSpeed(0);
								defSpw.getIValue().getObject()->setX(WINDOW_WIDTH/2+250 - i*500);
								defSpw.next();
							}
							defSpw[0].getSpawnTimer().setTime(995);
						} else if (Spw[0].getObject()->getY() > 150) {
							Spw[0].getObject()->setSpeed(0);
							Spw[0].getObject()->setY(150);
							defSpw.setBegin();
							defSpw.getIValue().getObject()->setDirection(0);
							defSpw.getIValue().getObject()->setSpeed(4);
							defSpw.next();
							defSpw.getIValue().getObject()->setDirection(180);
							defSpw.getIValue().getObject()->setSpeed(4);
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE7 : {
				if (!paused() && !gameover && (worldtime[0].getElapsed() > 9 && Spw.isEmpty())) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() < 5 && worldtime[1].getElapsed() > 0.25) {
							float x = randomFloat(50, WINDOW_WIDTH-50);
							Spw.add(Spawner2D(new SquareObj(x, -100, 100, 6), TYPE_SQUARE, 999, 1, SQUARE_EXPLODE1));
							Spw[0].getObject()->setSpeed(4);
							Spw[0].getObject()->setAngle(90);
							Spw[0].getObject()->setDirection(270);
							Spw[0].getObject()->getColorData()[1] = 0.6;
							Spw[0].getObject()->getColorData()[2] = 0.4;
							worldtime[1].resetTime();
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ZONE7B : {
				if (!paused() && !gameover && (worldtime[0].getElapsed() > 9 && Spw.isEmpty()) && worldtime[2].getElapsed() > 3) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() < 5 && worldtime[1].getElapsed() > 0.1) {
							float x = randomFloat(50, WINDOW_WIDTH-50);
							Spw.add(Spawner2D(new SquareObj(x, -100, 80, 4), TYPE_SQUARE, 999, 1, SQUARE_EXPLODE2)); 
							Spw[0].getObject()->setSpeed(4);
							Spw[0].getObject()->setAngle(90);
							Spw[0].getObject()->setDirection(270);
							Spw[0].getObject()->getColorData()[1] = 0.5;
							Spw[0].getObject()->getColorData()[2] = 0.3;
							
							Spw.add(Spawner2D(new SquareObj(x, -100, 100, 4), TYPE_SQUARE, 1.0, 1.5, TRIANGLE_RANDOMSHOT2));
							Spw[0].getObject()->setSpeed(4);
							Spw[0].getObject()->setAngle(90);
							Spw[0].getObject()->setDirection(270);
							Spw[0].getObject()->getColorData()[0] = 0.6;
							Spw[0].getObject()->getColorData()[1] = 0.0;
							Spw[0].getObject()->getColorData()[2] = 0.6;
							Spw[0].getSpawnTimer().setTime(randomFloat(0, 1450));
							worldtime[1].resetTime();
						}
						checkDamaged();
						drawObjects();
						if (Spw.isEmpty() && worldtime[2].getElapsed() > 3.5)
							worldtime[2].resetTime();
					}
					drawCursor();
				}
			} break;
			case ID_RIPPLES : {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (Spw[0].getObject()->getSpeed() > 1 && Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5) {
							Spw[0].getObject()->setSpeed(0);
							Spw[0].getObject()->setY(WINDOW_HEIGHT/5-0.01);
							defSpw[0].getObject()->setSpeed(0);
							defSpw[0].getObject()->setY(WINDOW_HEIGHT/5-0.01);
							defSpw[0].getSpawnTimer().setTime(600);
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_DANMAKU1: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						Spw[0].getObject()->setAngle(worldtime[0].getElapsed()*360);
						if (Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5) {
							Spw[0].getObject()->setSpeed(0);
							Spw[0].getObject()->setY(WINDOW_HEIGHT/5-0.01);
							defSpw[0].getObject()->setSpeed(0);
							defSpw[0].getObject()->setY(WINDOW_HEIGHT/5-0.01);
						}
						patterner += 13;
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_RAIN: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if ((Spw[0].getObject()->distanceSquaredTo(pointmark) < 4) || (Spw[0].getObject()->getSpeed() > 2 && Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5)) {
							if ((randomInt(0,1) == 0 && Spw[0].getObject()->getX() < WINDOW_WIDTH-500) || Spw[0].getObject()->getX() < 500)
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()+150, Spw[0].getObject()->getX()+220), randomFloat(100, 200));
							else
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()-150, Spw[0].getObject()->getX()-220), randomFloat(100, 200));
							Spw[0].getObject()->setSpeed(sqrt(Spw[0].getObject()->distanceSquaredTo(pointmark))/300);
							Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(pointmark));
							Spw[0].getSpawnTimer().setTime(15000);
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_LASERBLOOM: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if ((Spw[0].getObject()->distanceSquaredTo(pointmark) < 4) || (Spw[0].getObject()->getSpeed() > 2 && Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5)) {
							if ((randomInt(0,1) == 0 && Spw[0].getObject()->getX() < WINDOW_WIDTH-400) || Spw[0].getObject()->getX() < 400)
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()+100, Spw[0].getObject()->getX()+200), randomFloat(90, 180));
							else
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()-100, Spw[0].getObject()->getX()-200), randomFloat(90, 180));
							Spw[0].getObject()->setSpeed(sqrt(Spw[0].getObject()->distanceSquaredTo(pointmark))/300);
							Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(pointmark));
						}
						
						Ptt.setBegin();
						Qu.setBegin();
						for(int i = 0; i < Ptt.size(); i++) {
							float ang = 7.0 - 3*Qu.getIValue().getTimeElapsed();
							if (ang > 0)
								Ptt.getIValue().rotate(ang);
							for(int j = 0; j < Ptt.getIValue().getAliveAmount(); j++) {
								Qu.getIValue().setSpeed(Qu.getIValue().getTimeElapsed()*Qu.getIValue().getTimeElapsed());
								Qu.next();
							}
							Ptt.next();
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_RADIANCE: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if ((Spw[0].getObject()->distanceSquaredTo(pointmark) < 100) || (worldtime[1].getElapsed() < 2 && Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5)) {
							if ((randomInt(0,1) == 0 && Spw[0].getObject()->getX() < WINDOW_WIDTH-700) || Spw[0].getObject()->getX() < 700)
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()+250, Spw[0].getObject()->getX()+500), randomFloat(90, 200));
							else
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()-250, Spw[0].getObject()->getX()-500), randomFloat(90, 200));
							worldtime[0].resetTime();
							Spw[0].getObject()->setSpeed(0);
							Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(pointmark));
							Spw[0].setSpawnTime(0.04);
						} else if (worldtime[0].getElapsed() > 2.0 && worldtime[0].getElapsed() < 2.5) {
							Spw[0].setSpawnTime(10);
						} else if (worldtime[0].getElapsed() > 2.5) {
							Spw[0].getObject()->setSpeed(sqrt(Spw[0].getObject()->distanceSquaredTo(pointmark))/10);
						}
						
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_SPIKECAGE: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if ((Spw[0].getObject()->distanceSquaredTo(pointmark) < 4) || (Spw[0].getObject()->getSpeed() > 5 && Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5)) {
							if ((randomInt(0,1) == 0 && Spw[0].getObject()->getX() < WINDOW_WIDTH-400) || Spw[0].getObject()->getX() < 400)
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()+100, Spw[0].getObject()->getX()+200), randomFloat(90, 180));
							else
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()-100, Spw[0].getObject()->getX()-200), randomFloat(90, 180));
							Spw[0].getObject()->setSpeed(sqrt(Spw[0].getObject()->distanceSquaredTo(pointmark))/300);
							Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(pointmark));
						}
							
						Tr.setBegin();
						for (int i = 0; i < Tr.size(); i++) {
							if (Tr.getIValue().getX() < 0 || Tr.getIValue().getX() > WINDOW_WIDTH) {
								Tr.getIValue().setDirection(180-Tr.getIValue().getDirection());
								Tr.getIValue().setAngle(Tr.getIValue().getDirection());
							}
							if (Tr.getIValue().getY() < 0 || Tr.getIValue().getY() > WINDOW_HEIGHT) {
								Tr.getIValue().setDirection(-Tr.getIValue().getDirection());
								Tr.getIValue().setAngle(Tr.getIValue().getDirection());
							}
							Tr.next();
						}
						checkDamaged();
						if (Spw[0].getObject()->isMaterialised() && Spw[0].isDamaged())
							Spw[0].getSpawnTimer().setTime(Spw[0].getSpawnTimer().getElapsed()*1000 + 9000);
						drawObjects();
					}
					drawCursor();
				}
			} break;
			case ID_ICESPIRIT: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if ((Ptt[0].distanceSquaredTo(pointmark) < 4) || (Ptt[0].getSpeed() > 3 && Ptt[0].getY() >= WINDOW_HEIGHT/5)) {
							if ((randomInt(0,1) == 0 && Ptt[0].getX() < WINDOW_WIDTH-400) || Ptt[0].getX() < 400)
								pointmark = Point(randomFloat(Ptt[0].getX()+100, Ptt[0].getX()+200), randomFloat(80, 200));
							else
								pointmark = Point(randomFloat(Ptt[0].getX()-100, Ptt[0].getX()-200), randomFloat(80, 200));
							Ptt[0].setSpeed(sqrt(Ptt[0].distanceSquaredTo(pointmark))/300);
							Ptt[0].setDirection(-Ptt[0].angleTo(pointmark));
						}
						
						if (Spw[0].getHealth() > 300) {
							
							defSpw.setBegin();
							if (defSpw.size() > 4) {
								defSpw[0].getObject()->setSpeed(defSpw[0].getObject()->getSpeed()-0.05);
								if (worldtime[0].getElapsed() > 1.7) {
									defSpw.getIValue().getObject()->setAngle(-defSpw.getIValue().getObject()->angleTo(Point(xpos, ypos)));
									defSpw.getIValue().getObject()->setDirection(-defSpw.getIValue().getObject()->angleTo(Point(xpos, ypos)));
									defSpw.getIValue().getObject()->setSpeed(7.5);
									worldtime[0].resetTime();
								}
								defSpw.next();
							} else
								worldtime[0].resetTime();
							
							for (int i = 0; i < 4; i++) {
								defSpw.getIValue().getObject()->getColorData()[1] = 1.0 - (540 + SHOT_THRESHOLD - Spw[0].getHealth())/200;
								defSpw.next();
							}
						} else {
							defSpw.setBegin();
							defSpw.getIValue().getObject()->setSpeed(defSpw.getIValue().getObject()->getSpeed()-0.08);
							if (worldtime[1].getElapsed() > 0.91) {
								defSpw.getIValue().getObject()->setAngle(-defSpw.getIValue().getObject()->angleTo(Point(xpos, ypos)));
								defSpw.getIValue().getObject()->setDirection(-defSpw.getIValue().getObject()->angleTo(Point(xpos, ypos)));
								defSpw.getIValue().getObject()->setSpeed(7.5);
								worldtime[1].resetTime();
							}
							defSpw.next();
							defSpw.getIValue().getObject()->setSpeed(defSpw.getIValue().getObject()->getSpeed()-0.05);
							if (worldtime[0].getElapsed() > 1.7) {
								defSpw.getIValue().getObject()->setAngle(-defSpw.getIValue().getObject()->angleTo(Point(xpos, ypos)));
								defSpw.getIValue().getObject()->setDirection(-defSpw.getIValue().getObject()->angleTo(Point(xpos, ypos)));
								defSpw.getIValue().getObject()->setSpeed(7.5);
								worldtime[0].resetTime();
							}
						}
						
						if (Spw[0].getHealth() > 300 && Spw[0].getHealth() <= 300 + SHOT_THRESHOLD) {
							Spw[0].damage(SHOT_THRESHOLD);
							Spw[0].getSpawnTimer().setTime(999999);
							worldtime[1].resetTime();
							Qu.add(QuadObj(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT, 998, 999));
							Qu[0].getColorData()[0] = 0.5;
							Qu[0].getColorData()[1] = 0.4;
							Qu[0].getColorData()[2] = 0.8;
							Qu.add(QuadObj(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1, 1));
						}
						
						Tr.setBegin();
						for (int i = 0; i < Tr.size(); i++) {
							if (Tr.getIValue().getSpeed() < 0.05) {
								if (Tr.getIValue().getSpeed() != 0)
									Tr.getIValue().setSpeed(0);
							} else
								Tr.getIValue().setSpeed(Tr.getIValue().getSpeed()-0.15);
							Tr.next();
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_EARTHSPIRIT: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if ((Ptt[0].distanceSquaredTo(pointmark) < 4) || (Ptt[0].getSpeed() > 3 && Ptt[0].getY() >= WINDOW_HEIGHT/5)) {
							if ((randomInt(0,1) == 0 && Ptt[0].getX() < WINDOW_WIDTH-400) || Ptt[0].getX() < 400)
								pointmark = Point(randomFloat(Ptt[0].getX()+100, Ptt[0].getX()+200), randomFloat(60, 180));
							else
								pointmark = Point(randomFloat(Ptt[0].getX()-100, Ptt[0].getX()-200), randomFloat(60, 180));
							Ptt[0].setSpeed(sqrt(Ptt[0].distanceSquaredTo(pointmark))/400);
							Ptt[0].setDirection(-Ptt[0].angleTo(pointmark));
						}
						
						defSpw.setBegin();
						for (int i = 0; i < defSpw.size() - 4; i++) {
							defSpw.next();
						}
						
						if (Spw[0].getHealth() > 360) {
							for (int i = 0; i < 4; i++) {
								defSpw.getIValue().getObject()->getColorData()[2] = 1.0 - (540 + SHOT_THRESHOLD - Spw[0].getHealth())/200;
								defSpw.next();
							}
						} else {
							if (worldtime[0].getElapsed() > 3) {
								Tr.add(TriangleObj(xpos, WINDOW_HEIGHT-90, 270, 250, 2, 4));
								Tr[0].setAngle(90);
								Tr.add(TriangleObj(xpos, WINDOW_HEIGHT+1300, 270, 250, 7, 1.9));
								Tr[0].setAngle(90);
								Tr[0].setDirection(90);
								Tr[0].setSpeed(9);
								Tr.setBegin();
								for (int i = 0; i < 2; i++) {
									Tr.getIValue().getColorData()[0] = 0.4;
									Tr.getIValue().getColorData()[1] = 0.3;
									Tr.getIValue().getColorData()[2] = 0.0;
									Tr.next();
								}
								worldtime[0].resetTime();
							}
						}
						
						if (Spw[0].getHealth() > 360 && Spw[0].getHealth() <= 360 + SHOT_THRESHOLD) {
							Spw[0].damage(SHOT_THRESHOLD);
							Qu.add(QuadObj(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT, 998, 999));
							Qu[0].getColorData()[0] = 0.5;
							Qu[0].getColorData()[1] = 0.3;
							Qu[0].getColorData()[2] = 0.1;
							Qu.add(QuadObj(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1, 1));
						}
						
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_SHOOTER6: {
				if (!paused() && !gameover && Spw.isEmpty() && worldtime[1].getElapsed() > 3.5) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (!Spw.isEmpty()) {
							if (worldtime[0].getElapsed() > 18) {
								Spw.setBegin();
								for (int i = 0; i < Spw.size(); i++) {
									Spw.getIValue().getObject()->setSpeed(5);
									Spw.next();
								}
							} else if (worldtime[0].getElapsed() > 0.8 && Spw[0].getObject()->getSpeed() > 4) {
								Spw.setBegin();
								for (int i = 0; i < Spw.size(); i++) {
									Spw.getIValue().getObject()->setSpeed(0);
									Spw.getIValue().getObject()->setDirection(270);
									Spw.getIValue().getSpawnTimer().setTime(randomFloat(0.0, 900.0));
									Spw.next();
								}
							}
						}
						
						checkDamaged();
						drawObjects();
						if (Spw.isEmpty() && worldtime[1].getElapsed() > 4)
							worldtime[1].resetTime();
					}
					drawCursor();
				}
			} break;
			case ID_DANMAKU2: {
				if (!paused() && !gameover && Spw.size() < 2) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if ((Ptt[0].distanceSquaredTo(pointmark) < 100) || (worldtime[1].getElapsed() < 2 && Ptt[0].getY() >= WINDOW_HEIGHT/5)) {
							if ((randomInt(0,1) == 0 && Ptt[0].getX() < WINDOW_WIDTH-500) || Ptt[0].getX() < 500)
								pointmark = Point(randomFloat(Ptt[0].getX()+200, Ptt[0].getX()+350), randomFloat(80, 210));
							else
								pointmark = Point(randomFloat(Ptt[0].getX()-200, Ptt[0].getX()-350), randomFloat(80, 210));
							worldtime[0].resetTime();
							Ptt[0].setSpeed(0);
							Ptt[0].setDirection(-Ptt[0].angleTo(pointmark));
						} else if (worldtime[1].getElapsed() > 2) {
							Ptt[0].setSpeed(sqrt(Ptt[0].distanceSquaredTo(pointmark))/20);
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_PETAL: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						Spw[0].getObject()->setAngle(worldtime[0].getElapsed()*360);
						if (Spw[0].getObject()->getSpeed() > 3 && worldtime[0].getElapsed() > 1.2) {
							Spw[0].getObject()->setSpeed(0);
							Spw[0].spawnObject();
						}
						if (Spw[0].getSpawnTime() > 1.5 && worldtime[1].getElapsed() > 6) {
							Spw[0].setSpawnTime(1.15);
							Qu.add(QuadObj(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1, 3));
						}
						normalDraw();
						Ptt.setBegin();
						//Slingshot pattern
						int i = 0;
						while(i < Ptt.size()) {
							int j = 0;
							while (j < Ptt.getIValue().getObjNum() - 1 && Ptt.getIValue().getObjData()[j] == nullptr) {
								j++;
							}
							if (Ptt.getIValue().getObjData()[j]->getLifeTimer().getElapsed() < 2) {
								i++;
								Ptt.next();
							} else {
								Ptt.removeIValue();
							}
						}
					}
					drawCursor();
				}
			} break;
			case ID_DASHER: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (worldtime[0].getElapsed() > 1) {
							if (Sq.isEmpty() && worldtime[2].getElapsed() > 0.3) {
								worldtime[1].resetTime();
								worldtime[2].resetTime();
								Sq.add(SquareObj(randomFloat(300, WINDOW_WIDTH-300), randomFloat(100, WINDOW_HEIGHT-100), 50, 4.05, 6));
								Sq[0].getColorData()[0] = 1.0; Sq[0].getColorData()[1] = 0.2; Sq[0].getColorData()[2] = 0.2;
								patterner++;
							} else if (worldtime[1].getElapsed() < 4) {
								if (Sq.size() < 8 && worldtime[2].getElapsed() > 0.3) {
									Sq.add(SquareObj(randomFloat(300, WINDOW_WIDTH-300), randomFloat(100, WINDOW_HEIGHT-100), 50, 4.05, 6));
									switch (patterner%8) {
										case 1: Sq[0].getColorData()[0] = 0.9; Sq[0].getColorData()[1] = 0.5; Sq[0].getColorData()[2] = 0.2;
										break;
										case 2: Sq[0].getColorData()[0] = 0.9; Sq[0].getColorData()[1] = 0.9; Sq[0].getColorData()[2] = 0.2;
										break;
										case 3: Sq[0].getColorData()[0] = 0.2; Sq[0].getColorData()[1] = 1.0; Sq[0].getColorData()[2] = 0.2;
										break;
										case 4: Sq[0].getColorData()[0] = 0.2; Sq[0].getColorData()[1] = 0.9; Sq[0].getColorData()[2] = 0.9;
										break;
										case 5: Sq[0].getColorData()[0] = 0.2; Sq[0].getColorData()[1] = 0.2; Sq[0].getColorData()[2] = 1.0;
										break;
										case 6: Sq[0].getColorData()[0] = 0.9; Sq[0].getColorData()[1] = 0.2; Sq[0].getColorData()[2] = 0.9;
										break;
									}
									patterner++;
									worldtime[2].resetTime();
								}
							} else {
								if (worldtime[2].getElapsed() > 0.3) {
									pointmark = Point(Sq[Sq.size()-1].getX(), Sq[Sq.size()-1].getY());
									Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(pointmark));
									Spw[0].getObject()->setAngle(Spw[0].getObject()->getDirection());
									Spw[0].getSpawnTimer().setTime(998800);
									patterner++;
									worldtime[2].resetTime();
								}
							}
							float spd = sqrt(Spw[0].getObject()->distanceSquaredTo(pointmark))/5;
							if (spd > 0.1)
								Spw[0].getObject()->setSpeed(spd);
							else
								Spw[0].getObject()->setSpeed(0);
						}
						
						
						Sq.setBegin();
						for (int i = 0; i < Sq.size(); i++) {
							Sq.getIValue().setAngle(Sq.getIValue().getAngle() + 3);
							Sq.next();
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ASTRALCAGE: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (Spw[0].getObject()->getSpeed() > 3 && worldtime[0].getElapsed() > 2.21) {
							defSpw.setBegin();
							for (int i = 0; i < 4; i++) {
								defSpw.getIValue().getObject()->setDirection(defSpw.getIValue().getObject()->getDirection() - 90);
								defSpw.getIValue().getObject()->setSpeed(25);
								defSpw.next();
							}
							for (int i = 0; i < 8; i++) {
								defSpw.getIValue().getObject()->setSpeed(0);
								defSpw.next();
							}
							Spw[0].getObject()->setSpeed(0);
						} else if (worldtime[0].getElapsed() > 3.5) {
							if (Ptt[0].distanceSquaredTo(pointmark) < 4) {
								float x, y, factor;
								factor = randomFloat(10, 180);
								if ((randomInt(0,1) == 0 && Ptt[0].getY() < WINDOW_HEIGHT-300) || Ptt[0].getY() < 300)
									y = Ptt[0].getY() + factor;
								else
									y = Ptt[0].getY() - factor;
								if ((randomInt(0,1) == 0 && Ptt[0].getX() < WINDOW_WIDTH-300) || Ptt[0].getX() < 300)
									x = randomFloat(Ptt[0].getX()+(10 + factor*2/3), Ptt[0].getX()+150);
								else
									x = randomFloat(Ptt[0].getX()-(10 + factor*2/3), Ptt[0].getX()-150);
								pointmark = Point(x, y);
								Ptt[0].setSpeed(1.35);
								Ptt[0].setDirection(-Ptt[0].angleTo(pointmark));
							}
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_TIMESORCERER: {
				if (!paused() && !gameover && Spw.size() < 3) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (Ptt[0].getSpeed() > 3 && worldtime[0].getElapsed() > 1) {
							Ptt[0].setSpeed(0);
							worldtime[1].resetTime();
						}
						if (worldtime[0].getElapsed() > 1.2) {
							if (Spw[0].getSpawnTime() > 50 && worldtime[1].getElapsed() < 2.7) {
								Spw[0].spawnObject();
								Spw[0].setSpawnTime(0.01);
							} else if (Spw[0].getSpawnTime() < 1) {
								Spw[0].setSpawnTime(60);
							}
							if (Sq.isEmpty())
								worldtime[1].resetTime();
							else {
								Spw.setBegin();
								Spw.next();
								Sq.setBegin();
								for (int i = 0; i < Sq.size(); i++) {
									if (Sq.getIValue().getSpeed() < 0) {
										if (Sq.getIValue().getColorData()[0] > 0.5) {
											Sq.getIValue().getColorData()[0] = 0.0;
											Sq.getIValue().getColorData()[1] = 0.0;
										} else if (Spw.getIValue().getObject()->isIn(Sq.getIValue())) {
											Sq.getIValue().setTime(999000);
										}
									}
									if (worldtime[1].getElapsed() > 2 && worldtime[1].getElapsed() < 4)
										Sq.getIValue().setSpeed(14 - worldtime[1].getElapsed()*3.5);
									else if (worldtime[1].getElapsed() >= 4 && worldtime[1].getElapsed() < 7)
										Sq.getIValue().setSpeed(6 - worldtime[1].getElapsed()*1.5);
									
									Sq.next();
								}
							}
						}
						Spw[0].getObject()->getColorData()[2] = 0.6 + 0.2*sin(timeeffect.getElapsed()*3);
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_SPIRALMADNESS: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (Ptt[0].distanceSquaredTo(pointmark) < 4) {
							if (((randomInt(0,1) == 0) && Ptt[0].getX() < WINDOW_WIDTH-400) || Ptt[0].getX() < 400)
								pointmark = Point(randomFloat(Ptt[0].getX()+180, Ptt[0].getX()+250), randomFloat(70, 180));
							else
								pointmark = Point(randomFloat(Ptt[0].getX()-180, Ptt[0].getX()-250), randomFloat(70, 180));
							Ptt[0].setSpeed(sqrt(Ptt[0].distanceSquaredTo(pointmark))/280);
							Ptt[0].setDirection(-Ptt[0].angleTo(pointmark));
							Spw[0].getObject()->setSpeed(Ptt[0].getSpeed());
							Spw[0].getObject()->setDirection(Ptt[0].getDirection());
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ID_ARROWMASTER: {
				if (!paused() && !gameover && Spw.size() < 3) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (Spw[0].getObject()->distanceSquaredTo(pointmark) < 10) {
							if ((randomInt(0,1) == 0 && Spw[0].getObject()->getX() < WINDOW_WIDTH-400) || Spw[0].getObject()->getX() < 400)
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()+100, Spw[0].getObject()->getX()+200), randomFloat(90, 180));
							else
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()-100, Spw[0].getObject()->getX()-200), randomFloat(90, 180));
							Ptt[Ptt.size()-1].setSpeed(sqrt(Spw[0].getObject()->distanceSquaredTo(pointmark))/300);
							Ptt[Ptt.size()-1].setDirection(-Spw[0].getObject()->angleTo(pointmark));
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			case ZONE_TEST: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (Spw[0].getObject()->distanceSquaredTo(pointmark) < 10) {
							if ((randomInt(0,1) == 0 && Spw[0].getObject()->getX() < WINDOW_WIDTH-400) || Spw[0].getObject()->getX() < 400)
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()+100, Spw[0].getObject()->getX()+200), randomFloat(90, 180));
							else
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()-100, Spw[0].getObject()->getX()-200), randomFloat(90, 180));
							Spw[0].getObject()->setSpeed(sqrt(Spw[0].getObject()->distanceSquaredTo(pointmark))/280);
							Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(pointmark));
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;
			/**case ZONE_TEST: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(0);
				} else if (!gameover) {
					if (!paused()) {
						if (Spw[0].getObject()->distanceSquaredTo(pointmark) < 10) {
							if ((randomInt(0,1) == 0 && Spw[0].getObject()->getX() < WINDOW_WIDTH-400) || Spw[0].getObject()->getX() < 400)
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()+100, Spw[0].getObject()->getX()+200), randomFloat(90, 180));
							else
								pointmark = Point(randomFloat(Spw[0].getObject()->getX()-100, Spw[0].getObject()->getX()-200), randomFloat(90, 180));
							Spw[0].getObject()->setSpeed(sqrt(Spw[0].getObject()->distanceSquaredTo(pointmark))/300);
							Spw[0].getObject()->setDirection(-Spw[0].getObject()->angleTo(pointmark));
						}
						normalDraw();
					}
					drawCursor();
				}
			} break;**/
			case ID_HELP: {
				glVertexPointer (2, GL_FLOAT, 0, helptext);
				glLineWidth(2);
				glDrawArrays(GL_LINES, 0, helpmark[9]/2);
				drawCursor();
			} break;
			case ID_WIN: {
				glLineWidth(7);
				glVertexPointer (2, GL_FLOAT, 0, wintext);
				glColor4f(1.0,1.0,1.0,delaytime.getElapsed());
				glDrawArrays(GL_LINES, 0, winmark[2]/2);
				glLineWidth(2);
				glDrawArrays(GL_LINES, winmark[2]/2, (winmark[3] - winmark[2])/2);
			} break;
			case ID_STORY1: {
				glLineWidth(STORY_LINESIZE);
				glVertexPointer (2, GL_FLOAT, 0, story1text);
				glColor4f(1.0,1.0,1.0,worldtime[0].getElapsed());
				glDrawArrays(GL_LINES, 0, story1mark[2]/2);
				drawCursor();
				if (!paused() && !gameover && worldtime[0].getElapsed() > 3)
					switchLevel(ID_ZONE1);
			} break;
			case ID_STORY2: {
				glLineWidth(STORY_LINESIZE);
				glVertexPointer (2, GL_FLOAT, 0, story2text);
				glColor4f(1.0,1.0,1.0,worldtime[0].getElapsed());
				glDrawArrays(GL_LINES, 0, story2mark[1]/2);
				drawCursor();
				if (!paused() && !gameover && worldtime[0].getElapsed() > 3)
					switchLevel(ID_ZONE1B);
			} break;
			case ID_STORY3: {
				glLineWidth(STORY_LINESIZE);
				glVertexPointer (2, GL_FLOAT, 0, story3text);
				glColor4f(1.0,1.0,1.0,worldtime[0].getElapsed());
				glDrawArrays(GL_LINES, 0, story3mark[1]/2);
				drawCursor();
				if (!paused() && !gameover && worldtime[0].getElapsed() > 3)
					switchLevel(ID_WIN);
			} break;
			case ID_STORY4: {
				glLineWidth(STORY_LINESIZE);
				glVertexPointer (2, GL_FLOAT, 0, story4text);
				glColor4f(1.0,1.0,1.0,worldtime[0].getElapsed());
				glDrawArrays(GL_LINES, 0, story4mark[1]/2);
				drawCursor();
				if (!paused() && !gameover && worldtime[0].getElapsed() > 3)
					switchLevel(ID_WIN);
			} break;
			case ID_STORY5: {
				glLineWidth(STORY_LINESIZE);
				glVertexPointer (2, GL_FLOAT, 0, story5text);
				glColor4f(1.0,1.0,1.0,worldtime[0].getElapsed());
				glDrawArrays(GL_LINES, 0, story5mark[1]/2);
				drawCursor();
				if (!paused() && !gameover && worldtime[0].getElapsed() > 3)
					switchLevel(ID_WIN);
			} break;
			case ID_STORY6: {
				glLineWidth(STORY_LINESIZE);
				glVertexPointer (2, GL_FLOAT, 0, story6text);
				glColor4f(1.0,1.0,1.0,worldtime[0].getElapsed());
				glDrawArrays(GL_LINES, 0, story6mark[2]/2);
				drawCursor();
				if (!paused() && !gameover && worldtime[0].getElapsed() > 3)
					switchLevel(ID_WIN);
			} break;
			case ID_STORY7: {
				glLineWidth(STORY_LINESIZE);
				glVertexPointer (2, GL_FLOAT, 0, story7text);
				glColor4f(1.0,1.0,1.0,worldtime[0].getElapsed());
				glDrawArrays(GL_LINES, 0, story7mark[1]/2);
				drawCursor();
				if (!paused() && !gameover && worldtime[0].getElapsed() > 3)
					switchLevel(ID_WIN);
			} break;
			case ID_STORY8: {
				glLineWidth(STORY_LINESIZE);
				glVertexPointer (2, GL_FLOAT, 0, story8text);
				glColor4f(1.0,1.0,1.0,worldtime[0].getElapsed());
				glDrawArrays(GL_LINES, 0, story8mark[1]/2);
				drawCursor();
				if (!paused() && !gameover && worldtime[0].getElapsed() > 3)
					switchLevel(ID_WIN);
			} break;
			case ID_STORY9: {
				glLineWidth(STORY_LINESIZE);
				glVertexPointer (2, GL_FLOAT, 0, story9text);
				glColor4f(1.0,1.0,1.0,worldtime[0].getElapsed());
				glDrawArrays(GL_LINES, 0, story9mark[1]/2);
				drawCursor();
				if (!paused() && !gameover && worldtime[0].getElapsed() > 3)
					switchLevel(ID_WIN);
			} break;
			case ID_STORY10: {
				glLineWidth(STORY_LINESIZE);
				glVertexPointer (2, GL_FLOAT, 0, story10text);
				glColor4f(1.0,1.0,1.0,worldtime[0].getElapsed());
				glDrawArrays(GL_LINES, 0, story10mark[1]/2);
				drawCursor();
				if (!paused() && !gameover && worldtime[0].getElapsed() > 3)
					switchLevel(ID_WIN);
			} break;
			default: //ID_GAMEOVER
				if (delaytime.getElapsed() > 7) {
					switchLevel(ID_MAINMENU);
				}
				else if (delaytime.getElapsed() > 0.5) { 
					glLineWidth(7);
					glVertexPointer (2, GL_FLOAT, 0, gameovertext);
					glDrawArrays(GL_LINES, 0, gameovermark[1]/2);
					if (!storymode) {
						glLineWidth(4);
						glDrawArrays(GL_LINES, gameovermark[1]/2, (gameovermark[3]-gameovermark[1])/2);
					}
				}
		}
		
		///Draw title
		if (!disabled) {
			drawTitle();
			moveObjects();
		}
		
		///Draw timer
		if (!storymode && !disabled && !gameover && (gamemode > 0) && (gamemode <= ZONE_LIMIT)) { //strict limiter
			drawTimer();
		}
		
		///Draw borders
		glLineWidth(2);
		glColor4f(1.0,1.0,1.0,1.0);
		glVertexPointer (2, GL_FLOAT, 0, borders);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		
		//Check gameover
		iter = 0;
		Sq.setBegin();
		while ((!disabled) && (iter < Sq.size()) && (!gameover)) {
			if (Sq.getIValue().isMaterialised() && Sq.getIValue().isIn(Point(xpos, ypos))) {
				gameOver();
			}
			Sq.next();
			iter++;
		}
		iter = 0;
		Tr.setBegin();
		while ((!disabled) && (iter < Tr.size()) && (!gameover)) {
			if (Tr.getIValue().isMaterialised() && Tr.getIValue().isIn(Point(xpos, ypos))) {
				gameOver();
			}
			Tr.next();
			iter++;
		}
		iter = 0;
		Qu.setBegin();
		while ((!disabled) && (iter < Qu.size()) && (!gameover)) {
			if (Qu.getIValue().isMaterialised() && Qu.getIValue().isIn(Point(xpos, ypos))) {
				gameOver();
			}
			Qu.next();
			iter++;
		}
		iter = 0;
		defSpw.setBegin();
		while ((!disabled) && (iter < defSpw.size()) && (!gameover)) {
			if (defSpw.getIValue().getObject()->isMaterialised() && defSpw.getIValue().getObject()->isIn(Point(xpos, ypos))) {
				gameOver();
			}
			defSpw.next();
			iter++;
		}
		iter = 0;
		subSpw.setBegin();
		while ((!disabled) && (iter < subSpw.size()) && (!gameover)) {
			if (subSpw.getIValue().getObject()->isMaterialised() && subSpw.getIValue().getObject()->isIn(Point(xpos, ypos))) {
				gameOver();
			}
			subSpw.next();
			iter++;
		}
		iter = 0;
		Spw.setBegin();
		while ((!disabled) && (iter < Spw.size()) && (!gameover)) {
			if (Spw.getIValue().getObject()->isMaterialised() && Spw.getIValue().getObject()->isIn(Point(xpos, ypos))) {
				gameOver();
			}
			Spw.next();
			iter++;
		}
						
		if (gameover) { //gameover
			if (delaytime.getElapsed() > 2) {
				switchLevel(ID_GAMEOVER);
				gameover = false;
				spacepressed = false;
				delaytime.resetTime();
			}
		}
		
		//Memindahkan buffer ke tempat yang akan digunakan
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	//Destroy window
	glfwDestroyWindow(window);
	
	//Terminasi GLFW
	glfwTerminate();

	return 0;
}
