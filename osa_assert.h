#ifndef OSA_ASSERT_H_
#define OSA_ASSERT_H_

#include <iostream>
#include <cstdlib>

/**
 * \brief It prints the failed message and aborts the execution.
 *
 * This function is not meant to be used directly. Use VERIFY_ASSERTION.
 * @param expr	The expression that has been evaluated.
 * @param file	The file containing the evaluated expression.
 * @param line	The line that contains the evaluated expression.
 */
inline void AssertionFailedMsg(const char* expr, const char* file, int line)
{
	std::cerr << "Assertion \"" << expr << "\" failed [" << file << ":"
			  << line << "]" << std::endl;
	std::abort();
}

/** \brief Macro to evaluate an expression.
 *
 * If the expression evaluates to false then the AssertionFailedMsg function is
 * called.
 */
#define VERIFY_ASSERTION(expr) \
	(expr ? static_cast<void>(0) : AssertionFailedMsg(#expr, __FILE__,\
					__LINE__))

#endif /* ASSERT_HPP_ */

