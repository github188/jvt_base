#ifndef OSA_POSIX_TIME_H_
#define OSA_POSIX_TIME_H_


#include <time.h>

namespace OSA {

/**
 * \brief Class to contain a time
 *
 * This class wraps a time, with a resolution of nanoseconds.
 * It is useful to get the current time and to make comparisons between times.
 */
class Time
{
	/**
	 * \brief Time in the form returned by gettimeofday()
	 */
	struct timespec time_;

	/**
	 * Type of clock.
	 */
	clockid_t clockType_;
public:
	Time(clockid_t clockType = CLOCK_MONOTONIC);
	virtual ~Time(){}
	void add(time_t sec, long nsec);
	void set(time_t sec, long nsec);
	void resetToCurrentTime();
	bool operator< (const Time& ref) const;
	bool operator> (const Time& ref) const;
	bool operator== (const Time& ref) const;
	void getResolution (time_t* sec, long* nsec);


	/**
	 * \brief Method to get the number of seconds
	 *
	 * @return Number of seconds
	 */
	inline time_t getSeconds() const {
		return time_.tv_sec;
	}

	/**
	 * \brief Method to get the number of microseconds
	 *
	 * @return Number of microseconds
	 */
	inline long getNSeconds() const {
		return time_.tv_nsec;
	}
};

} /* onposix */

#endif /* TIME_HPP_ */

