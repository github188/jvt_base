#include "osa_time.h"

#include <stdexcept>

namespace OSA {

/**
 * \brief Default constructor.
 *
 * It initializes the class with the current time (calling clock_gettime()
 * @param clockType: the type of the clock:
 * <ul>
 * <li> CLOCK_REALTIME: System-wide time.
 * <li> CLOCK_MONOTONIC (default): Monotonic time since some unspecified starting point.
 * It cannot be set.
 * <li> CLOCK_PROCESS_CPUTIME_ID: Per-process cpu time.
 * <li> CLOCK_THREAD_CPUTIME_ID: Per-thread  cpu time.
 * </ul>
 * This parameter affects the initial value and the result of resetToCurrentTime()
 * @exception std::runtime_error, thrown by resetToCurrentTime()
 */
Time::Time(clockid_t clockType): clockType_(clockType)
{
	resetToCurrentTime();
}

/**
 * \brief Method to add seconds and nseconds to the current value
 *
 * @param sec Number of seconds to be added
 * @param usec Number of nseconds to be added
 */
void Time::add(time_t sec, long nsec)
{
	time_.tv_nsec += nsec;
	time_.tv_sec += sec;
}

/**
 * \brief Method to set the time to a specific value
 *
 * @param sec Number of seconds to be set
 * @param usec Number of nseconds to be set
 */
void Time::set(time_t sec, long nsec)
{
	time_.tv_nsec = nsec;
	time_.tv_sec = sec;
}

/**
 * \brief Method to reset the class to the current time.
 *
 * This method sets the time equal to the value returned by gettimeofday().
 * @exception std::runtime_error in case of error
 */
void Time::resetToCurrentTime()
{
	if (clock_gettime(clockType_, &time_) != 0)
		throw std::runtime_error("Can't get current time");
}

/**
 * \brief Operator to compare two values
 */
bool Time::operator< (const Time& ref) const
{
	if (time_.tv_sec < ref.time_.tv_sec)
		return true;
	else if ((time_.tv_sec == ref.time_.tv_sec) &&
		 (time_.tv_nsec < ref.time_.tv_nsec))
			return true;
	return false;
}

/**
 * \brief Operator to compare two values
 */
bool Time::operator> (const Time& ref) const
{
	if (time_.tv_sec > ref.time_.tv_sec)
		return true;
	else if ((time_.tv_sec == ref.time_.tv_sec) &&
		 (time_.tv_nsec > ref.time_.tv_nsec))
			return true;
	return false;
}

/**
 * \brief Operator to compare two values
 * @return true if the times expressed by the classes are equal; false otherwise
 */
bool Time::operator== (const Time& ref) const
{
	if ((time_.tv_sec == ref.time_.tv_sec) &&
		(time_.tv_nsec == ref.time_.tv_nsec))
			return true;
	return false;
}

/**
 * \brief Method to get timer resolition.
 *
 * @param sec: number of seconds of resolution.
 * @param nsec: number of seconds of resolution.
 * @exception std::runtime_error in case of error
 */
void Time::getResolution (time_t* sec, long* nsec)
{
	timespec ret;
	if (clock_getres(clockType_, &ret) != 0)
		throw std::runtime_error("Can't get time resoultion");
	*sec = ret.tv_sec;
	*nsec = ret.tv_nsec;
}


} /* onposix */

