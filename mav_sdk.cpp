#include "mav_sdk.h"


/* for calibrate magnetometer sensor. 
reference : https://github.com/mavlink/MAVSDK/blob/develop/examples/calibrate/calibrate.cpp
*/

std::function<void(Calibration::Result, Calibration::ProgressData)>
create_calibration_callback(std::promise<void>& calibration_promise)
{
    return [&calibration_promise](
               const Calibration::Result result, const Calibration::ProgressData progress_data) {
        switch (result) {
            case Calibration::Result::Success:
                std::cout << "--- Calibration succeeded!" << std::endl;
                calibration_promise.set_value();
                break;
            case Calibration::Result::Next:
                if (progress_data.has_progress) {
                    std::cout << "    Progress: " << progress_data.progress << std::endl;
                }
                if (progress_data.has_status_text) {
                    std::cout << "    Instruction: " << progress_data.status_text << std::endl;
                }
                break;
            default:
                std::cout << "--- Calibration failed with message: " << result << std::endl;
                calibration_promise.set_value();
                break;
        }
    };
}
void calibrate_magnetometer(Calibration& calibration)
{
    std::cout << "Calibrate magnetometer..." << std::endl;

    std::promise<void> calibration_promise;
    auto calibration_future = calibration_promise.get_future();

    calibration.calibrate_magnetometer_async(create_calibration_callback(calibration_promise));

    calibration_future.wait();
}

void calibrate_accelerometer(Calibration& calibration)
{
    std::cout<<"Calibtrate Accelometer...."<<std::endl;
    std::promise<void> calibration_promise;
    auto calibration_future = calibration_promise.get_future();
    calibration.calibrate_accelerometer_async(create_calibration_callback(calibration_promise));
    calibration_future.wait();
}

/* 
calculate average bias for the magnetometer(compass) sensor from a 1000 samples
when you get average bias, plz don't move pixhawk 
*/
float calc_mag_bias(Telemetry& telemetry,Calibration& calibration,std::string calibration_set)
{
   
    while(!(telemetry.health().is_magnetometer_calibration_ok)){
        std::cout<<"Start calibrate Magnetometer Sensor"<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        if (calibration_set == "0"){
            calibrate_magnetometer(calibration);
            std::cout<<"calibration finished. px4 be reboot in 10 sec"<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(13));
        }
    }

    if (calibration_set == "2"){
        std::cout<<"Start calibrate Accelerometer"<<std::endl;        
        calibrate_accelerometer(calibration);
        std::cout<<"calibration finished. px4 be reboot in 10 sec"<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
        
    }
    
    float bias = 0;
    {
        float count = 0.0f;       
        auto deg = telemetry.attitude_euler().yaw_deg, pre_deg= telemetry.attitude_euler().yaw_deg;
        if(telemetry.health().is_magnetometer_calibration_ok)
        {
            std::cout<<" Start calc mag_sensor bias \n Please don't move robot"<<std::endl;
            while(!(count==SAMPLING_COUNT))
            {
                deg = telemetry.attitude_euler().yaw_deg;
                if(deg<0.0f) deg = 360+deg;  
                bias = bias + std::abs(deg-pre_deg);
                pre_deg = deg;
                count++;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            bias /= SAMPLING_COUNT; 
            std::cout<<"Bias is : "<<bias<<std::endl;
        }
        else {
            std::cout<<"Can't get sensor data!"<<std::endl;
        }
        if( std::isnan(bias)) 
        {
            std::cout<<"sensor value be currupted. please re-access to px4"<<std::endl;
            return 0;
        }
    }

    return bias;

}

void usage(std::string bin_name)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Usage : " << bin_name << " <connection_url>" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
}

void component_discovered(ComponentType component_type)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Discovered a component with type "
              << unsigned(component_type) << std::endl;
}

bool are_arguments_valid(int argc, char** /* argv */)
{
    return argc == 2;
}

void print_usage(const std::string& bin_name)
{
    std::cout << "Usage : " << bin_name << " <connection_url>" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
}
