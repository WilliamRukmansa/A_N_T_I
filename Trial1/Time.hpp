#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>
#include <fstream>
using namespace std;

class Time {
	private:
		std::chrono::steady_clock::time_point ms;
		std::chrono::milliseconds elapsed; //mark waktu
	public:
		Time(); //ctor, inisialisasi waktu
		Time(const Time&); //cctor
		virtual ~Time(); //dtor
		Time& operator=(const Time&); //assignment
		friend ofstream& operator << (ofstream& out, Time &T);
		friend ifstream& operator >> (ifstream& in, Time &T);
		
		double getElapsed() const; //get time elapsed in seconds
		void resetTime(); //reset time back to 0
		void setTime(long ms); //set time with milliseconds after
};

std::string getNowTime(); //includes endl

#endif
