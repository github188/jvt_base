#include <iostream>
#include <new>
#include <cstdlib>

#include "osa_log.h"

namespace OSA {

// Definition (and initialization) of static attributes
Logger* Logger::m_ = 0;

#ifdef LOG_MULTITHREAD

PosixMutex Logger::lock_;

inline void Logger::lock()
{
	lock_.lock();
}

inline void Logger::unlock()
{
    lock_.unlock();
}
#else
inline void Logger::lock(){}
inline void Logger::unlock(){}
#endif





/**
 * \brief Constructor.
 *
 * It is a private constructor, called only by getInstance() and only the
 * first time. It is called inside a lock, so lock inside this method
 * is not required.
 * It only initializes the initial time. All configuration is done inside the
 * configure() method.
 */
Logger::Logger():
		logFile_(""),
		latestMsgPrintedOnFile_(false),
		latestMsgPrintedOnConsole_(false)
{
	gettimeofday(&initialTime_, NULL);
}

/**
 * \brief Method to configure the logger. 
 *
 * This method is called by the LOG_FILE() macro.
 * @param outputFile Name of the file used for logging
 */
void Logger::setFile (const std::string& outputFile)
{
		Logger::lock();
		latestMsgPrintedOnFile_ = false;
		latestMsgPrintedOnConsole_ = false;

		// Compute a new file name, if needed
		if (outputFile != logFile_){

			if (logFile_ != "")
				out_.close();
			std::ostringstream oss;
			time_t currTime;
			time(&currTime);
			struct tm *currTm = localtime(&currTime);
			oss << outputFile << "_" <<
					currTm->tm_mday << "_" <<
					currTm->tm_mon << "_" <<
					(1900 + currTm->tm_year) << "_" <<
					currTm->tm_hour << "-" <<
					currTm->tm_min << "-" <<
					currTm->tm_sec << ".log";
			logFile_ = oss.str().c_str();
		}

		// Open a new stream:
		out_.open(logFile_.c_str(), std::ios::app);

		Logger::unlock();
}



/**
 * \brief Destructor.
 *
 * It only closes the file, if open, and cleans memory.
 */

Logger::~Logger()
{
	Logger::lock();
	if (logFile_ != "")
		out_.close();
	delete m_;
	Logger::unlock();

}

/**
 * \brief Method to get a reference to the object (i.e., Singleton)
 *
 * This is a static method.
 * @return Reference to the object.
 */
Logger& Logger::getInstance()
{
	if (m_ == 0){
	    Logger::lock();
	    if (m_ == 0)
		m_ = new Logger;
	    Logger::unlock();
	}
	return *m_;
}

/**
 * @brief Method used to print messages on console.
 *
 * This method is called by the DEBUG(), WARNING() and ERROR() macros.
 * @param severitylevel Severity of the debug message
 * @param file Source file where the method has been called (set equal to __FILE__
 * 	      by the DEBUG macro)
 * @param line Number of line in the source code where the method has been
 * called (automatically set equal to __LINE__ by the DEBUG macro)
 * @param message Message to be logged
 */
void Logger::printOnConsole(const std::string& file,
			const int line,
			const std::string& message)
{

	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);

	Logger::lock();
	
	std::cout <<
	    (currentTime.tv_sec - initialTime_.tv_sec) <<
	    ":" << message << "\t\t[" << file << ":" << line << "]" <<
		std::endl;

	latestMsgPrintedOnConsole_ = true;

	Logger::unlock();
}


/**
 * @brief Method used to print messages on file.
 *
 * This method is called by the DEBUG(), WARNING() and ERROR() macros.
 * @param severitylevel Severity of the debug message
 * @param file Source file where the method has been called (set equal to __FILE__
 * 	      by the DEBUG macro)
 * @param line Number of line in the source code where the method has been
 * called (automatically set equal to __LINE__ by the DEBUG macro)
 * @param message Message to be logged
 */
void Logger::printOnFile(const std::string& file,
			const int line,
			const std::string& message)
{

	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);

	Logger::lock();
	
	latestMsgPrintedOnFile_ = false;
	
	if (logFile_ != "") {
		out_ <<
		    (currentTime.tv_sec - initialTime_.tv_sec) <<
		    ":" << message << "\t\t[" << file << ":" << line << "]" <<
			std::endl;
		latestMsgPrintedOnFile_ = true;
	}

	Logger::unlock();
}

} /* onposix */

