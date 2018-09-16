#include "CppRPiGPIO/CppRPiGPIO.h"
#include <sys/signal.h> // Needed for signal actions
#include <cstring>

#define FAN_PIN     18  // "P" pin of the NPN transistor, used to trigger the fan
#define MAX_TEMP    55  // Maximum temperature in Celsius, after which we trigger the fan
#define MIN_TEMP    45  // Minimum temperature in Celsius, below which we turn off the fan
#define SLEEP_TIME   5  // Time to wait between measures
#define MAX_BUFFER 100  // Max line length for vcgencmd reading

bool running = true;

int getCPUTemp();

void sighandler(int sig)
{
    running = false;
}

int main(void)
{
    // Kill signals management
    struct sigaction action;
    memset(&action, 0, sizeof(action));  // Empty mask by default
    action.sa_handler   = sighandler;    // Set the handler function
    action.sa_flags     = 0;             // Remove any flag from sa_flag
    sigaction(SIGINT, &action, NULL);
    
    int temperature = 0;
    
    // Init fan GPIO
    RPiGPIO *gpioFan;
    gpioFan = new RPiGPIO(FAN_PIN);
    gpioFan->SetDirection(OUT);
    
    while (running)
    {
        temperature = getCPUTemp();
        if (temperature > MAX_TEMP)
        {
            gpioFan->SetValue(true);
            sleep(SLEEP_TIME);
        }
        else if (temperature < MIN_TEMP)
        {
            gpioFan->SetValue(false);
        }
    }
    
    if (gpioFan)
    {
        delete gpioFan;
        gpioFan = nullptr;
    }
    
    return 0;
}

// Get RPi CPU temperature using vcgencmd
int getCPUTemp()
{
    std::string data;
    int temperature = 0;
    FILE * stream;
    char buffer[MAX_BUFFER];
    stream = popen("vcgencmd measure_temp", "r");
    if (stream) 
    {    
        if (fgets(buffer, MAX_BUFFER, stream) != NULL) 
        {
           // Output is "temp=xx.x'C", this recover the integer part of the temperature
           data.append(std::string(1, buffer[5]));
           data.append(std::string(1, buffer[6]));
        }
        pclose(stream);
	try
	{
            temperature = std::stoi(data);
	}
	catch(const std::exception& e)
	{
	    std::cerr << "Failed to read temperature from vgencmd: " << e.what() << std::endl;
	}
    }

    return temperature;
}

