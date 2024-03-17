#include "logger.h"

int main()
{
    // define log level
    // set the output stream default: console
    // setting the file name will enable logging in file
#ifdef LOGGING_ENABLED
    Logger::setLogLevel(LogLevel::DEBUG);
    Logger::setOutputStream(std::cout);
    Logger::setLogFile("log.log");
#endif

    // Log messages
    int value = 42;
    double pi = 3.14159;
    const char* message = "Hello, world!";
    uint32_t data = 0xDEADBEEF;

    try
    {
        LOG_INFO("Starting program...");

        LOG_DEBUG("The value is: " + std::to_string(value));
        LOG_DEBUG("The value is: " << value);
        LOG_DEBUG("The approximate value of pi is: " << pi);
        LOG_DEBUG("Message: " << message);
        LOG_DEBUG("Data: " << data);
        LOG_DEBUG("Data: " << std::hex << data);

        LOG_WARNING("Approximate value of pi: " + std::to_string(pi));
        LOG_ERROR("An error occurred! " << pi);
        LOG_CRITICAL("Some critical issue " << pi);

        LOG_INFO("Program end...");
    }
    catch (const std::exception& e) 
    {
        LOG_ERROR("An error occurred! " << e.what());
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}