///Compiling shortcut (Linux): g++ -o message message.cpp -lGL -lGLU -lglfw ///
#include <cstdlib>
#include <GLFW/glfw3.h>
#include <iostream>
#include "randomer.hpp"
using namespace std;

#include <fstream>
#include "Lists.hpp"
#include "Time.hpp"
#include "Letter.hpp"
#include "Spawner2D.hpp"
#include "SquareObj.hpp"
#include "QuadObj.hpp"
#include "TriangleObj.hpp"

///Global variables
int linesize;
double WINDOW_HEIGHT;
double WINDOW_WIDTH;
#define CURSOR_HEIGHT 4
#define CURSOR_WIDTH 4

float sqrtTwo = sqrt(2);

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
#define SHOT_DELAY 0.1
#define SHOT_DAMAGE 3

bool storymode = false;
bool autoshoot = false;
int rounds = -1;

//Texts but user too lazy to make a typedef instead lol
float pausetext[1000];
int pausemark[3];
const string pause1 = "PAUSED";
const string pause2 = "Press P to resume";

float gameovertext[1200];
int gameovermark[4];
const string gameover1 = "GAME OVER";

float mainmenutext[3000];
int mainmenumark[6];
const string mainmenu1 = "A N T I";
const string mainmenu2 = "Press S to start NEW game";
const string mainmenu3 = "Press L to load checkpoint";
const string mainmenu4 = "Press A to start arcade";
const string mainmenu5 = "Press H for help. Press Esc to quit";

float timetext[1500];
int timemark[2];

float wintext[1000];
int winmark[3];
const string wintext1 = "END OF GAME";
const string wintext2 = "Thanks for playing";

float helptext[4500];
int helpmark[8];
const string help1 = "Dodge all obstacles. Move with mouse.";
const string help2 = "Press P to pause.";
const string help3 = "Press and hold spacebar to shoot enemies.";
const string help4 = "Press T to toggle autoshoot.";
const string help5 = "This game may not be completable by you.";
const string help6 = "Do not stay too near to the edge.";
const string help7 = "Do not panic. Unless you must.";

///Title texts
float title_LevelA[300];
int title_LevelA_mark[2];
const string title_LevelA_str = "Level A";

float title_Level1[300];
int title_Level1_mark[2];
const string title_Level1_str = "Rapid Shooter";

///Story texts
#define STORY_LINESIZE 6
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
const string story7_str1 = "When will this end...";

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
List<QuadObj> Shots;

List<Spawner2D> defSpw;
List<Spawner2D> Spw;

#define ZONE_LIMIT 987

/** Making levels:
 * add into changeID/levelID
 * createLevel()
 * make level in main()
 * 
 * specific levels:
 * drawTitle
 * save zone
 * **/
 
///Stages
#define ID_GAMEOVER -1
#define ID_MAINMENU 0
#define ID_HELP 998
#define ID_WIN 999

//Story
#define ID_STORY1 997
#define ID_STORY2 996
#define ID_STORY3 995
#define ID_STORY4 994
#define ID_STORY5 993
#define ID_STORY6 992
#define ID_STORY7 991
#define ID_STORY8 990
#define ID_STORY9 989
#define ID_STORY10 988

//Easy
#define ID_ZONE1 1
#define ID_BULLET1 5
#define ID_ZONE2 2
#define ID_SHOOTER1 6
#define ID_ZONE3 3
#define ID_ZONE4 8
#define ID_ZONE5 9
#define ID_ZONE6 10
#define ID_ZONE6B 11
#define ID_SHOOTER2 12
#define ID_ZONE7 13
#define ID_SHOOTER3 14

#define ID_LEVEL1 801

//Medium
#define ID_ZONE1B 4

#define ID_LEVEL_A 802

//Hard
#define ID_BULLET2 7

#define ID_DANMAKU1 803

//Touhou

List<int> changeID;
List<int> levelID;
int patterner;

/* convert pixel to openGL coordinates */
void convertPointData(float *arr, int start, int end) {
	for (int i = start; i < end; i += 2) {
		arr[i] = widthConvert(arr[i]);
	}
	for (int i = start+1; i < end; i += 2) {
		arr[i] = heightConvert(arr[i]);
	}
}

///spawn shots
#define NULL_SHOT 0
#define SQUARE_ANGLESHOT1 1
#define SQUARE_ANGLESHOT2 2
#define SQUARE_ANGLESHOT3 3
#define QUAD_ANGLESHOT1 4
#define SQUARE_RAPIDSHOT1 5
#define SQUARE_AIMSHOT1 6
#define SQUARE_AIMSHOT2 7
#define SQUARE_PATTERNSHOT1 8
#define SQUARE_RANDOMSHOT1 9

#define LBOUND_EXPLOSION 80
#define SQUARE_EXPLODE1 81

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
			Sq[0].getColorData()[2] = (float)randomInt(0,20)/20;
		} break;
		case SQUARE_ANGLESHOT3 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 50, 6));
			Sq[0].setSpeed(6);
			Sq[0].setAngle(spawner->getAngle());
			Sq[0].setDirection(spawner->getAngle());
			Sq[0].getColorData()[1] = (float)randomInt(0,20)/20;
			Sq[0].getColorData()[2] = (float)randomInt(0,20)/20;
		} break;
		case SQUARE_PATTERNSHOT1 : {
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
		case SQUARE_RANDOMSHOT1 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 20, 15));
			Sq[0].setSpeed(2);
			float ang = randomFloat(0, 360);
			Sq[0].setAngle(ang);
			Sq[0].setDirection(ang);
		} break;
		case QUAD_ANGLESHOT1 : {
			Qu.add(QuadObj(spawner->getX(), spawner->getY(), 100, 20, 6, 0.5));
			Qu[0].setSpeed(4);
			Qu[0].setAngle(spawner->getAngle());
			Qu[0].setDirection(spawner->getAngle());
			Qu[0].getColorData()[0] = 0.0;
			Qu[0].getColorData()[1] = 0.0;
		} break;
		case SQUARE_RAPIDSHOT1 : {
			Sq.add(SquareObj(spawner->getX(), spawner->getY(), 30, 6));
			float a = randomFloat(-45,45);
			Sq[0].setSpeed(7);
			Sq[0].setAngle(spawner->getAngle()+a);
			Sq[0].setDirection(spawner->getAngle()+a);
		} break;
		case SQUARE_EXPLODE1 : {
			for (int i = 0; i < 12; i++) {
				Sq.add(SquareObj(spawner->getX(), spawner->getY(), 20, 10));
				Sq[0].setSpeed(4.2);
				Sq[0].setAngle(spawner->getAngle()+(float)i/12*360);
				Sq[0].setDirection(spawner->getAngle()+(float)i/12*360);
			}
		} break;
	}
	spawntimer.resetTime();
}

//Level init
void createLevel() {
	switch (gamemode) {
		case ID_LEVEL_A : { ///Level A
			for (int i = 0; i < 10; i++) {
				if (i%2 == 0)
					defSpw.add(Spawner2D(new SquareObj(-400, WINDOW_HEIGHT/9*(i), 150 - 10*(i), 999), TYPE_SQUARE, 999, 1, NULL_SHOT));
				else
					defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH+400, WINDOW_HEIGHT/9*(i), 50 + 10*(i-1), 999), TYPE_SQUARE, 999, 1, NULL_SHOT));
			}
			defSpw.setBegin();
			for (int i = 0; i < 10; i++) {
				defSpw.getIValue().getObject()->setSpeed(5);
				if (i%2 == 0)
					defSpw.getIValue().getObject()->setDirection(0);
				else
					defSpw.getIValue().getObject()->setDirection(180);
				defSpw.next();
			}
			float x = randomFloat(1,3.2);
			for (int i = 0; i < 10; i++) {
				defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/9*(i), -100, 50, 999, 300), TYPE_SQUARE, 999, 1, NULL_SHOT));
			}
			defSpw.setBegin();
			for (int i = 0; i < 10; i++) {
				defSpw.getIValue().getObject()->setSpeed(x);
				defSpw.getIValue().getObject()->setDirection(90);
				defSpw.next();
			}
			
			Spw.add(Spawner2D(new SquareObj(-150, -150, 180, 999), TYPE_SQUARE, 999, 250, NULL_SHOT));
			Spw[0].getObject()->getColorData()[0] = 0.0;
			Spw[0].getObject()->getColorData()[1] = 0.1;
			Spw[0].getObject()->getColorData()[2] = 0.7;
			
			for (int i = 0; i < 9; i++) {
				defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/9*(i+0.5), -100, 50, 999, 300), TYPE_SQUARE, 999, 1, NULL_SHOT));
			}
			
			float y = randomFloat(1,3.2);
			defSpw.setBegin();
			for (int i = 0; i < 9; i++) {
				defSpw.getIValue().getObject()->setSpeed(y);
				defSpw.getIValue().getObject()->setDirection(270);
				defSpw.next();
			}
			worldn = 1;
		} break;
		case ID_ZONE1: {
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -100, 1, 7), TYPE_SQUARE, 0.32, 0, SQUARE_ANGLESHOT2));
			worldn = 2;
		} break;
		case ID_ZONE1B: {
			defSpw.add(Spawner2D(new SquareObj(-100, WINDOW_HEIGHT/2, 1, 7), TYPE_SQUARE, 0.32, 0, SQUARE_ANGLESHOT2));
			defSpw.add(Spawner2D(new SquareObj(WINDOW_WIDTH+100, WINDOW_HEIGHT/2, 1, 7), TYPE_SQUARE, 0.32, 0, SQUARE_ANGLESHOT2));
			worldn = 2;
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
		case ID_BULLET2: {
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -180, 75, 15, 1), TYPE_SQUARE, 0.01, 180, QUAD_ANGLESHOT1));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.0;
			Spw[0].getObject()->getColorData()[2] = 0.0;
			worldn = 1;
		} break;
		case ID_SHOOTER2: {
			Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/3, -180, 150, 130, 10, 1), TYPE_TRIANGLE, 1, 30, QUAD_ANGLESHOT1));
			Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH*2/3, -180, 150, 130, 10, 1), TYPE_TRIANGLE, 1, 30, QUAD_ANGLESHOT1));
			Spw.setBegin();
			for (int i = 0; i < 2; i++) {
				Spw.getIValue().getObject()->setSpeed(6);
				Spw.getIValue().getObject()->setDirection(270);
				Spw.getIValue().getObject()->getColorData()[0] = 0.0;
				Spw.getIValue().getObject()->getColorData()[2] = 0.4;
				Spw.next();
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
		case ID_LEVEL1 : {
			Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -180, 200, 180, 999, 1), TYPE_TRIANGLE, 0, 250, SQUARE_RAPIDSHOT1));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.0;
			worldn = 1;
		} break;
		case ID_DANMAKU1 : {
			defSpw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -180, 10, 10, 999, 1.2), TYPE_TRIANGLE, 0.02, 1, SQUARE_RANDOMSHOT1));
			defSpw[0].getObject()->setSpeed(6);
			defSpw[0].getObject()->setDirection(270);
			defSpw[0].getObject()->getColorData()[0] = 0.1;
			defSpw[0].getObject()->getColorData()[1] = 0.6;
			defSpw[0].getObject()->getColorData()[2] = 0.5;
			Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH/2, -180, 150, 999, 1), TYPE_SQUARE, 0.02, 360, SQUARE_PATTERNSHOT1));
			Spw[0].getObject()->setSpeed(6);
			Spw[0].getObject()->setDirection(270);
			Spw[0].getObject()->getColorData()[0] = 0.1;
			Spw[0].getObject()->getColorData()[1] = 0.6;
			Spw[0].getObject()->getColorData()[2] = 0.5;
			worldn = 1;
		} break;
		default:
			worldn = 1;
	}
	for (int i = 0; i < worldn; i++)
		worldtime[i].resetTime();
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
				Shots.getIValue().markTime();
				Shots.next();
			}
			defSpw.setBegin();
			for (int i = 0; i < defSpw.size(); i++) {
				defSpw.getIValue().getSpawnTimer().markTime();
				defSpw.getIValue().getObject()->markTime();
				defSpw.next();
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
		Shots.getIValue().continueTime();
		Shots.next();
	}
	defSpw.setBegin();
	for (int i = 0; i < defSpw.size(); i++) {
		defSpw.getIValue().getSpawnTimer().continueTime();
		defSpw.getIValue().getObject()->continueTime();
		defSpw.next();
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
	switch (gamemode) {
		case ID_LEVEL_A:
			glVertexPointer (2, GL_FLOAT, 0, title_LevelA);
			glDrawArrays(GL_LINES, 0, title_LevelA_mark[1]/2);
		break;
		case ID_LEVEL1 :
			glVertexPointer (2, GL_FLOAT, 0, title_Level1);
			glDrawArrays(GL_LINES, 0, title_Level1_mark[1]/2);
		break;
	}
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
				case ID_LEVEL1 :
					Record << "Rapid_Shooter";
				break;
				default:
					Record << "Zone";
			}
			Record << endl;
			Record << "Rounds completed: " << rounds << endl;
			Record.close();
		}
		Record.open("record.txt", ios::out | ios::app);
		Record << "World: ";
		switch (gamemode) {
			case ID_LEVEL_A :
				Record << "Level_A";
			break;
			case ID_LEVEL1 :
				Record << "Rapid Shooter";
			break;
			default:
				Record << "Zone";
		}
		Record << endl;
		Record << "Rounds completed: " << rounds << endl;
		Record << "Survival time: " << timescore << endl << endl;
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
	storymode = false;
	delaytime.resetTime();
}

//Fungsi clear level
void clearLevel(int gmode) {
	if (changeID.find(gmode) == -1 || changeID.find(gamemode) == -1) {
		Sq.removeAll();
		Tr.removeAll();
		Qu.removeAll();
		Shots.removeAll();
	}
	defSpw.removeAll();
	Spw.removeAll();
	worldn = 0;
	glColor4f(1.0,1.0,1.0,1.0);
	glVertexPointer (2, GL_FLOAT, 0, borders);
	glDrawArrays(GL_QUADS, 0, 4);
}

//Switch levels
void switchLevel(int gmode) {
	clearLevel(gmode);
	gamemode = gmode;
	createLevel();
	patterner = 0;
	if ((gamemode <= 0) || (gamemode > ZONE_LIMIT))
		autoshoot = false;
	timeeffect.resetTime();
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
			default:
				hitcheck = false;
		}
		if (hitcheck) {
			ofstream saveFile;
			saveFile.open("savedata");
			saveFile << checkl << endl;
			saveFile.close();
		}
	} else if (changeID.find(gmode) != -1 || levelID.find(gmode) != -1) {
		rounds++;
	}
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
						storymode = true;
						switchLevel(ID_STORY1);
						ofstream saveFile;
						saveFile.open("savedata");
						checkl = ID_STORY1;
						saveFile << checkl << endl;
						saveFile.close();
					} break;
					case GLFW_KEY_L :
						storymode = true;
						switchLevel(checkl);
					break;
					case GLFW_KEY_A :
						switchLevel(changeID[randomInt(0,changeID.size()-1)]);
						timeeffect.resetTime();
					break;
					case GLFW_KEY_H :
						switchLevel(ID_HELP);
					break;
					case GLFW_KEY_ESCAPE :
						glfwSetWindowShouldClose(window, GLFW_TRUE);
					break;
				}
			break;
			case ID_GAMEOVER :
			case ID_HELP :
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
					if (autoshoot)
						spacepressed = false;
					autoshoot = !autoshoot;
				}
				if (key == GLFW_KEY_P && !storymode) {
					if (!disabled && !gameover) {
						disabled = true;
					} else {
						unpause();
					}
				} else if ((key == GLFW_KEY_SPACE || autoshoot) && !gameover && !disabled) {
					spacepressed = true;
				}
		}
	} else if (key == GLFW_KEY_SPACE && act == GLFW_RELEASE)
		spacepressed = false;
}

void enterCallback(GLFWwindow* window, int entered) {
	if (!entered && !disabled && !storymode && !gameover && (gamemode > 0) && (gamemode <= ZONE_LIMIT)) { //Strict limited
		disabled = true;
	}
}

void drawCursor() {
	glColor3f(1.0,0.0,0.0);
	cursorpoint[0] = widthConvert(xpos-CURSOR_WIDTH);
	cursorpoint[1] = heightConvert(ypos-CURSOR_HEIGHT);
	cursorpoint[2] = widthConvert(xpos);
	cursorpoint[3] = heightConvert(ypos-CURSOR_HEIGHT*sqrtTwo);
	cursorpoint[4] = widthConvert(xpos+CURSOR_WIDTH);
	cursorpoint[5] = heightConvert(ypos-CURSOR_HEIGHT);
	cursorpoint[6] = widthConvert(xpos+CURSOR_WIDTH*sqrtTwo);
	cursorpoint[7] = heightConvert(ypos);
	cursorpoint[8] = widthConvert(xpos+CURSOR_WIDTH);
	cursorpoint[9] = heightConvert(ypos+CURSOR_HEIGHT);
	cursorpoint[10] = widthConvert(xpos);
	cursorpoint[11] = heightConvert(ypos+CURSOR_HEIGHT*sqrtTwo);
	cursorpoint[12] = widthConvert(xpos-CURSOR_WIDTH);
	cursorpoint[13] = heightConvert(ypos+CURSOR_HEIGHT);
	cursorpoint[14] = widthConvert(xpos-CURSOR_WIDTH*sqrtTwo);
	cursorpoint[15] = heightConvert(ypos);
	
	glVertexPointer (2, GL_FLOAT, 0, cursorpoint);
	glDrawArrays(GL_POLYGON, 0, 8);
}

void drawObjects() {
	///Check shot damage
	int i = 0;
	bool check;
	Shots.setBegin();
	while (i < Shots.size()) {
		check = false;
		int j = 0;
		Spw.setBegin();
		while (j < Spw.size() && !check) {
			if (Spw.getIValue().getObject()->isMaterialised() && Spw.getIValue().getObject()->isIn(Shots.getIValue())) {
				check = true;
				Spw.getIValue().damage(SHOT_DAMAGE);
			} else {
				j++;
				Spw.next();
			}
		}
		if (check) {
			Shots.removeAt(i);
		} else {
			i++;
			Shots.next();
		}
	}
	
	///Removal check then draw
	i = 0;
	defSpw.setBegin();
	while (i < defSpw.size()) {
		if (defSpw.getIValue().getObject()->isDead()) {
			if (defSpw.getIValue().getSpawnType() > LBOUND_EXPLOSION)
				defSpw.getIValue().spawnObject();
			defSpw.removeAt(i);
		} else {
			if (defSpw.getIValue().getObject()->isMaterialised()) {
				defSpw.getIValue().getObject()->redraw();
				glVertexPointer (2, GL_FLOAT, 0, defSpw.getIValue().getObject()->getPointData());
				glColor4f(defSpw.getIValue().getObject()->getColorData()[0],defSpw.getIValue().getObject()->getColorData()[1],defSpw.getIValue().getObject()->getColorData()[2],1.0);
				switch (defSpw.getIValue().getObjType()) {
					case TYPE_SQUARE:
					case TYPE_QUAD:
						glDrawArrays(GL_QUADS, 0, 4);
					break;
					case TYPE_TRIANGLE:
						glDrawArrays(GL_TRIANGLES, 0, 3);
					break;
				}
				if (defSpw.getIValue().getSpawnTimer().getElapsed() > defSpw.getIValue().getSpawnTime())
					defSpw.getIValue().spawnObject();
				}
			defSpw.next();
			i++;
		}
	}
	
	i = 0;
	Spw.setBegin();
	while (i < Spw.size()) {
		if (Spw.getIValue().getObject()->isDead()) {
			Spw.removeAt(i);
		} else if (Spw.getIValue().getHealth() <= 0) {
			if (Spw.getIValue().getSpawnType() > LBOUND_EXPLOSION)
				Spw.getIValue().spawnObject();
			Spw.removeAt(i);
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
			Qu.removeAt(i);
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
			Sq.removeAt(i);
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
			Tr.removeAt(i);
		} else {
			Tr.getIValue().redraw();
			glVertexPointer (2, GL_FLOAT, 0, Tr.getIValue().getPointData());
			if (Tr.getIValue().isMaterialised())
				glColor4f(Tr.getIValue().getColorData()[0],Tr.getIValue().getColorData()[1],Tr.getIValue().getColorData()[2],1.0);
			else
				glColor4f(Tr.getIValue().getColorData()[0],Tr.getIValue().getColorData()[1],Tr.getIValue().getColorData()[2],0.3);
			glDrawArrays(GL_QUADS, 0, 4);
			Tr.next();
			i++;
		}
	}
	
	i = 0;
	Shots.setBegin();
	while (i < Shots.size()) {
		if (Shots.getIValue().isDead()) {
			Shots.removeAt(i);
		} else {
			Shots.getIValue().redraw();
			glVertexPointer (2, GL_FLOAT, 0, Shots.getIValue().getPointData());
			glColor4f(0,1,1,1);
			glDrawArrays(GL_QUADS, 0, 4);
			Shots.next();
			i++;
		}
	}
}

void moveObjects() {
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
		Shots.getIValue().move();
		Shots.next();
	}
	defSpw.setBegin();
	for (int i = 0; i < defSpw.size(); i++) {
		defSpw.getIValue().getObject()->move();
		defSpw.next();
	}
	Spw.setBegin();
	for (int i = 0; i < Spw.size(); i++) {
		Spw.getIValue().getObject()->move();
		Spw.next();
	}
}

void switchArcade() {
	if (rounds%10 == 8)
		switchLevel(levelID[randomInt(0,levelID.size()-1)]);
	else {
		int x;
		do {
			if (rounds < 9) //switcher for later use
				x = randomInt(0,changeID.size()-1);
			else
				x = randomInt(0,changeID.size()-1);
		} while (changeID[x] == gamemode);
		switchLevel(changeID[x]);
	}
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
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TestDriver", glfwGetPrimaryMonitor(), NULL);
	
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
	convertPointData(wintext, 0, winmark[2]);
	
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
	convertPointData(mainmenutext, 0, mainmenumark[5]);
	
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
	convertPointData(helptext, 0, helpmark[7]);
	
	//Title texts
	linesize = 2;
	
	title_LevelA_mark[0] = 0;
	title_LevelA_mark[1] = title_LevelA_mark[0];
	convertStringToLetterData(title_LevelA_str, 10, 10, linesize, title_LevelA, title_LevelA_mark[1], true);
	convertPointData(title_LevelA, 0, title_LevelA_mark[1]);
	
	title_Level1_mark[0] = 0;
	title_Level1_mark[1] = title_Level1_mark[0];
	convertStringToLetterData(title_Level1_str, 10, 10, linesize, title_Level1, title_Level1_mark[1], true);
	convertPointData(title_Level1, 0, title_Level1_mark[1]);
	
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
	gamemode = ID_MAINMENU;
	xpos = WINDOW_WIDTH/2;
	ypos = WINDOW_HEIGHT/2;
	
	int zeasy = 0, znormal = 0, zhard = 0, ztouhou = 0;
	int leasy = 0, lnormal = 0, lhard = 0, ltouhou = 0;
	
	changeID.add(ID_ZONE1); zeasy++;
	changeID.add(ID_ZONE2); zeasy++;
	changeID.add(ID_SHOOTER1); zeasy++;
	changeID.add(ID_ZONE3); zeasy++;
	changeID.add(ID_ZONE4); zeasy++;
	changeID.add(ID_ZONE5); zeasy++;
	//changeID.add(ID_ZONE6); story only
	changeID.add(ID_ZONE6B); zeasy++;
	changeID.add(ID_SHOOTER2); zeasy++;
	changeID.add(ID_ZONE7); zeasy++;
	changeID.add(ID_SHOOTER3); zeasy++;
	
	levelID.add(ID_LEVEL1); leasy++;
	
	changeID.add(ID_BULLET1); znormal++;
	changeID.add(ID_ZONE1B); znormal++;
	changeID.add(ID_ZONE2); znormal++;
	
	levelID.add(ID_LEVEL_A); lnormal++;
	
	changeID.add(ID_BULLET2); zhard++;
	
	levelID.add(ID_DANMAKU1); lhard++;
	
	//ignore this
	ltouhou = ltouhou + ztouhou + zhard + znormal + zeasy + lhard + lnormal + leasy;
	
	ifstream loadFile;
	loadFile.open("savedata");
	loadFile >> checkl;
	loadFile.close();
	
	if (checkl == 0)
		checkl = ID_STORY1;
	
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
			if (abs(newypos - ypos) + abs(newxpos - xpos) < 5) {
				xpos = newxpos;
				ypos = newypos;
			}else if (abs(newypos - ypos) + abs(newxpos - xpos) < 30) {
				xpos = newxpos - (newxpos - xpos)*10/11;
				ypos = newypos - (newypos - ypos)*10/11;
			} else if (abs(newypos - ypos) + abs(newxpos - xpos) < 50) {
				xpos = newxpos - (newxpos - xpos)*2/3;
				ypos = newypos - (newypos - ypos)*2/3;
			} else if (abs(newypos - ypos) + abs(newxpos - xpos) < 120) {
				xpos = newxpos - (newxpos - xpos)*4/5;
				ypos = newypos - (newypos - ypos)*4/5;
			} else {
				xpos = xpos + 60*cos(atan2((newypos - ypos),(newxpos - xpos)));
				ypos = ypos + 60*sin(atan2((newypos - ypos),(newxpos - xpos)));
			}
		}
		
		switch (gamemode) {
			case ID_MAINMENU: 
				glVertexPointer (2, GL_FLOAT, 0, mainmenutext);
				glLineWidth(7);
				glDrawArrays(GL_LINES, 0, mainmenumark[1]/2);
				glLineWidth(3);
				glDrawArrays(GL_LINES, mainmenumark[1]/2, (mainmenumark[5]-mainmenumark[1])/2);
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
						drawObjects();
						drawCursor();
					}
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
						drawObjects();
						drawCursor();
					}	
				}
			} break;
			case ID_LEVEL_A: { ///Level A
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (storymode) {
						switchLevel(ID_STORY3);
					} else {
						switchArcade();
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
						
						if (Spw[0].getHealth() <= 100 && Spw[0].getHealth() > 100 - SHOT_DAMAGE) {
							Spw[0].damage(SHOT_DAMAGE);
							defSpw.setBegin();
							for (int i = 0; i < 19; i++) {
								defSpw.getIValue().getObject()->setTime(301000);
								defSpw.next();
							}
							glColor4f(1.0,1.0,1.0,1.0);
							glVertexPointer (2, GL_FLOAT, 0, borders);
							glDrawArrays(GL_QUADS, 0, 4);
						}
						drawObjects();
						drawCursor();
					}
				}
			} break;
			case ID_BULLET1: {
				if (!paused() && !gameover && worldtime[0].getElapsed() > 6) {
					if (storymode)
						switchLevel(ID_BULLET2);
					else
						switchArcade();
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
						drawObjects();
						drawCursor();
					}	
				}
			} break;
			case ID_ZONE2 : {
				if (!paused() && !gameover && worldtime[0].getElapsed() > 4) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_ZONE4);
				} else if (!gameover) {
					if (!paused()) {
						if ((rounds > 6) && worldtime[1].getElapsed() > 0.1) {
							float x = randomFloat(0.8,1.5);
							Sq.add(SquareObj(xpos + randomDouble(-200, 200), ypos + randomDouble(-250, 250), randomInt(150, 300), x+0.5, x));
							worldtime[1].resetTime();
						} else if (worldtime[1].getElapsed() > 0.25) {
							float x = randomFloat(1.4,2);
							Sq.add(SquareObj(xpos + randomDouble(-200, 200), ypos + randomDouble(-200, 200), randomInt(150, 300), x+0.5, x));
							worldtime[1].resetTime();
						}
						Sq.setBegin();
						for (int i = 0; i < Sq.size(); i++) {
							if (!Sq.getIValue().isMaterialised())
								Sq.getIValue().setAngle(Sq.getIValue().getTimeElapsed()*100);
							Sq.next();
						}
						drawObjects();
						drawCursor();
					}	
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
						drawObjects();
						drawCursor();
					}	
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
						drawObjects();
						drawCursor();
					}	
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
							Spw.add(Spawner2D(new SquareObj(100, -150, 120, 1.7), TYPE_SQUARE, 0.6, 1, SQUARE_AIMSHOT1));
							Spw.add(Spawner2D(new SquareObj(WINDOW_WIDTH-100, -150, 120, 1.7), TYPE_SQUARE, 0.6, 1, SQUARE_AIMSHOT1));
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
						drawObjects();
						drawCursor();
					}	
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
							Spw.add(Spawner2D(new TriangleObj(WINDOW_WIDTH/2, -120, 120, 100, 8, 0.5), TYPE_TRIANGLE, randomFloat(3, 5), 8, SQUARE_AIMSHOT1));
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
						drawObjects();
						drawCursor();
					}	
				}
			} break;
			case ID_ZONE6 : {
				if (!paused() && !gameover && worldtime[0].getElapsed() > 5) {
					switchLevel(ID_LEVEL1);
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
						drawObjects();
						drawCursor();
					}	
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
						drawObjects();
						drawCursor();
					}	
				}
			} break;
			case ID_BULLET2: {
				if (!paused() && !gameover && Spw.isEmpty()) {
					if (!storymode)
						switchArcade();
					else
						switchLevel(ID_LEVEL_A);
				} else if (!gameover) {
					if (!paused()) {
						if (Spw[0].getObject()->getAngle() > 90) {
							Spw[0].getObject()->setAngle(Spw[0].getObject()->getAngle()+5);
							if (Spw[0].getObject()->getAngle() > 380)
								Spw[0].getObject()->setAngle(89);
						} else {
							Spw[0].getObject()->setAngle(Spw[0].getObject()->getAngle()-5);
							if (Spw[0].getObject()->getAngle() < -200)
								Spw[0].getObject()->setAngle(91);
						}
						if (worldtime[0].getElapsed() > 12) {
							Spw[0].getObject()->setSpeed(5);
						} else if (Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5) {
							Spw[0].getObject()->setSpeed(0);
							Spw[0].getObject()->setY(WINDOW_HEIGHT/5-0.01);
						} 
						drawObjects();
						drawCursor();
					}	
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
						} else if (Spw[0].getObject()->getY() >= WINDOW_HEIGHT/5) {
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
						drawObjects();
						drawCursor();
					}	
				}
			} break;
			case ID_SHOOTER3: {
				if (!paused() && !gameover && (Spw.isEmpty() && worldtime[0].getElapsed() > 5)) {
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
						drawObjects();
						drawCursor();
					}	
				}
			} break;
			case ID_LEVEL1: {
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
						Spw[0].setSpawnTime(0.12-worldtime[0].getElapsed()/200);
						drawObjects();
						drawCursor();
					}	
				}
			} break;
			case ID_ZONE7 : {
				if (!paused() && !gameover && (worldtime[0].getElapsed() > 9 || (worldtime[0].getElapsed() > 9 && Spw.isEmpty()))) {
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
						drawObjects();
						drawCursor();
					}	
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
						drawObjects();
						drawCursor();
					}	
				}
			} break;
			case ID_HELP: {
				glVertexPointer (2, GL_FLOAT, 0, helptext);
				glLineWidth(2);
				glDrawArrays(GL_LINES, 0, helpmark[7]/2);
				drawCursor();
			} break;
			case ID_WIN: {
				glLineWidth(7);
				glVertexPointer (2, GL_FLOAT, 0, wintext);
				glColor4f(1.0,1.0,1.0,delaytime.getElapsed());
				glDrawArrays(GL_LINES, 0, winmark[2]/2);
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
				if (delaytime.getElapsed() > 0.5) { 
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
		
		///Shooting
		if (!gameover && gamemode > 0 && gamemode < ZONE_LIMIT && spacepressed && delaytime.getElapsed() > SHOT_DELAY) {
			QuadObj shot(xpos, ypos-7, 8, 20, 3);
			shot.setDirection(90);
			shot.setSpeed(10);
			Shots.add(shot);
			delaytime.resetTime();
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
