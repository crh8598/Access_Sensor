#ifndef MAV_SDK_H
#define MAV_SDK_H

#include <chrono>
#include <future>
#include <cstdint>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/calibration/calibration.h>
#include <iostream>
#include <thread>
#include <curses.h>
#include <cmath>
#include <stdio.h>

using namespace mavsdk;
using namespace std::this_thread;
using namespace std::chrono;
using namespace std;


#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour
#define SAMPLING_COUNT 1000.0f

std::function<void(Calibration::Result, Calibration::ProgressData)>
create_calibration_callback(std::promise<void>& calibration_promise);

void calibrate_magnetometer(Calibration& calibration);
void calibrate_accelerometer(Calibration& calibration);

float calc_mag_bias(Telemetry& telemetry,Calibration& calibration,std::string calibration_set);
void usage(std::string bin_name);
void component_discovered(ComponentType component_type);
bool are_arguments_valid(int argc, char** /* argv */);
void print_usage(const std::string& bin_name);

#endif
