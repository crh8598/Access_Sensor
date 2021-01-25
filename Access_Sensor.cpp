//
// Simple example to demonstrate how to use the MAVSDK.
//
// Author: Julian Oes <julian@oes.ch>

#include "mav_sdk.h"
#include <sys/socket.h>

using namespace mavsdk;
using namespace std::this_thread;
using namespace std::chrono;
using namespace std;


/* for calibrate magnetometer sensor. 
reference : https://github.com/mavlink/MAVSDK/blob/develop/examples/calibrate/calibrate.cpp
*/

int main(int argc, char** argv)
{
     if (!are_arguments_valid(argc, argv)) {
        const auto binary_name = argv[0];
        print_usage(binary_name);
        return 1;
    }

    Mavsdk mavsdk;
    std::string connection_url;
    std::string usb_connection = "serial:///dev/ttyACM0";

    ConnectionResult connection_result;

    bool discovered_system = false;
    
    connection_result = mavsdk.add_any_connection(usb_connection);
    

    if (connection_result != ConnectionResult::Success) {
        std::cout << ERROR_CONSOLE_TEXT << "Connection failed: " << connection_result
                  << NORMAL_CONSOLE_TEXT << std::endl;
        return 1;

    }

    std::cout << "Waiting to discover system..." << std::endl;
    mavsdk.subscribe_on_new_system([&mavsdk, &discovered_system]() {
        const auto system = mavsdk.systems().at(0);

        if (system->is_connected()) {
            std::cout << "Discovered system" << std::endl;
            discovered_system = true;
        }
    });

    // We usually receive heartbeats at 1Hz, therefore we should find a system after around 2
    // seconds.
    sleep_for(seconds(2));
    std::cout<<(argv[1])<<std::endl;

    if (!discovered_system) {
        std::cout << ERROR_CONSOLE_TEXT << "No system found, exiting." << NORMAL_CONSOLE_TEXT
                  << std::endl;
        return 1;
    }


//--------------------------------  Access to px4  -------------------------------------------------------------

    
    const auto system = mavsdk.systems().at(0);
    auto calibration = Calibration(system);
    auto telemetry =  Telemetry(system);

    // get bias for yaw drift by average filter 
    float bias = calc_mag_bias(telemetry,calibration,argv[1])+0.1;
    if(bias == 0) 
    {
        telemetry.~Telemetry();
        
        return -1;
    }
    // bias =0.0f;
    // Register a callback so we get told when components (camera, gimbal) etc are found.
    system->register_component_discovered_callback(component_discovered);

// start reading mag sensor value
    try {
        
        auto deg = telemetry.attitude_euler().yaw_deg;
        auto pre_deg = telemetry.attitude_euler().yaw_deg;
        float deg_integ = 0.0f;
        while(telemetry.health().is_magnetometer_calibration_ok)
        {
            
            deg= telemetry.attitude_euler().yaw_deg; 
            if(deg < 0) deg += 360.0f;

            if(deg_integ > 0) deg-=bias;
            else  deg+=bias;
            std::cout<<"\r"<<"deg : "<<deg<<"  , intrg : "<<deg_integ;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            deg_integ += deg-pre_deg;
            deg_integ = max(-2.0f,min(2.0f,deg_integ));
            pre_deg = deg;
        }

    }        
    catch(std::exception msg){
        std::cout<<msg.what()<<endl;
        return 0;

    }



    // We are relying on auto-disarming but let's keep watching the telemetry for a bit longer.
    sleep_for(seconds(3));
    std::cout << "Finished..." << std::endl;
    
    return 0;
}


