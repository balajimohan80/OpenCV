#ifndef __TIMER_HPP__
#define __TIMER_HPP__
#include<iostream>
#include<chrono>

class Timer {
private:
	std::chrono::steady_clock::time_point Start;
	std::chrono::steady_clock::time_point End;
	static constexpr std::intmax_t Resolution_In_Micro_Sec = 1000000l;
	static constexpr std::intmax_t Resolution_In_Milli_Sec = 1000l;
	static constexpr std::intmax_t Resolution_In_Sec = 1l;
	static constexpr std::intmax_t Numerator = 1l;
	std::string F_Name;
	bool Is_Timer_Stopped;
	bool Is_Print_Or_Not;

public:
	Timer(const std::string &Str) :F_Name(Str), Is_Print_Or_Not(true) {
		Start_Time();
	}
	Timer() : Is_Print_Or_Not(false)  {
		Start_Time();
	}

	inline void Stop_Time() {
		End = std::chrono::steady_clock::now();
		Is_Timer_Stopped = true;
	}

	inline void Start_Time() {
		Start = std::chrono::steady_clock::now();
		Is_Timer_Stopped = false;
	}

	inline double Get_Compute_Time_ms() {
		if (!Is_Timer_Stopped) {
			Stop_Time();
		}
		std::chrono::duration<double, std::ratio<Numerator, Resolution_In_Milli_Sec>> Milli_Sec(End - Start);
		return Milli_Sec.count();
	}

	inline double Get_Compute_Time_micros() {
		if (!Is_Timer_Stopped) {
			Stop_Time();
		}
		std::chrono::duration<double, std::ratio<Numerator, Resolution_In_Micro_Sec>> Micro_Sec(End - Start);
		return Micro_Sec.count();
	}

	inline int64_t Get_Compute_Time_Sec() {
		if (!Is_Timer_Stopped) {
			Stop_Time();
		}
		std::chrono::duration<int64_t, std::ratio<Numerator, 1>> Sec(std::chrono::duration_cast<std::chrono::duration<int64_t, std::ratio<Numerator, Resolution_In_Sec>>>(End - Start));
		return Sec.count();
	}

	inline void Print_Compute_Time() {
		if (!Is_Timer_Stopped) {
			Stop_Time();
		}
		std::chrono::duration<double, std::ratio<Numerator, Resolution_In_Micro_Sec>>  Micro_Sec(End - Start);
		std::chrono::duration<double, std::ratio<Numerator, Resolution_In_Milli_Sec>>  Milli_Sec(End - Start);
		std::chrono::duration<int64_t, std::ratio<Numerator, 1>> Sec(std::chrono::duration_cast<std::chrono::duration<int64_t, std::ratio<Numerator, Resolution_In_Sec>>>(End - Start));

		std::cout << F_Name << " " << "( " << Micro_Sec.count() << " microSec) ( " << Milli_Sec.count() << " milliSec) ( " << Sec.count() << " Sec)\n";
	}

	~Timer() {
		if (Is_Print_Or_Not)
			Print_Compute_Time();
	}
};

#endif
