#include "Core/Objects/utils.h"
#include "Core/engine.h"
#include <ostream>
#include <iomanip>
#include <time.h>
#include <Windows.h>
#include <iostream>
using namespace Awesome;

void Utils::Log(string a, kLogColors color,kLogOutput logOut) {

	switch (logOut)
	{
	case Awesome::kWindow_OGL: {
		Engine * tmpEngine = Engine::engine_instance();
		tmpEngine->add_log(TimeHourMinutesSeconds() + a, color);
	}
		break;
	case Awesome::kWindow_CMD: {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		//string b = TimeHourMinutesSeconds();
		std::cout << TimeHourMinutesSeconds() << a;
		FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), kColor_Default);
	}
		break;

	}
	
}

string Utils::TimeHourMinutesSeconds() {
	time_t timer = time(NULL);
	struct tm timeinfo;
	//time(&timer);
	//timeinfo = localtime(&timer);
	localtime_s(&timeinfo, &timer);
	string time = "\n[" + std::to_string(timeinfo.tm_hour) + ":" + std::to_string(timeinfo.tm_min) + ":" + std::to_string(timeinfo.tm_sec) + "] ";
	return time;
}


string Utils::ReadFromFile(string file) {
	std::ifstream f;
	f.open(file.c_str(), std::ios::in | std::ios::binary);
	//printf("\nShaderFromFile loaded ->%s\n", filePath.c_str());
	//	printf("\nF %s",f.get());
	if (!f.is_open()) {
		Utils::Log("ERORR::FILE_NOT_FOUND", kLogColors::kColor_Red, kLogOutput::kWindow_CMD);
		return NULL;
	}
	else {
		//read whole file into stringstream buffer
		std::stringstream buffer;
		buffer << f.rdbuf();
		string info = buffer.str();
		f.close();
		//printf("%s\n", info.c_str());
		return info;
	}

}

std::string Utils::limit_float_precision(float val)
{
	std::ostringstream oss;

	oss << std::fixed << std::setfill('0') << std::setprecision(2) << val;

	return oss.str();
}

std::string Utils::limit_float_precision(vec3 val) {
	std::ostringstream oss;
	
	oss << std::fixed << std::setfill('0') << std::setprecision(2) << val.x<< " " << std::fixed << std::setfill('0') << std::setprecision(2) << val.y << " " << std::fixed << std::setfill('0') << std::setprecision(2) << val.z;

	return oss.str();
}
std::string Utils::limit_float_precision(vec2 val) {
	std::ostringstream oss;

	oss << std::fixed << std::setfill('0') << std::setprecision(2) << val.x << " " << std::fixed << std::setfill('0') << std::setprecision(2) << val.y;

	return oss.str();
}

