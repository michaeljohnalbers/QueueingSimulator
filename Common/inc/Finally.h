#pragma once

/**
 * @file Finallly.h
 * @brief Class that runs a lambda on destruction (helpful for destruction on
 * exceptions.
 *
 * @author Michael Albers (though almost common pattern by now)
 */

#include <functional>

namespace QS
{
  class Finally
  {
    public:

    /**
     * Default constructor.
     */
    Finally() = delete;

    /**
     * Constructor.
     *
     * @param theCallback
     *          function to run on destruction.
     */
    Finally(std::function<void(void)> theCallback) :
      myCallback(theCallback)
    {
    }

    /**
     * Copy constructor.
     */
    Finally(const Finally&) = delete;

    /**
     * Move constructor.
     */
    Finally(Finally&&) = default;

    /**
     * Destructor. Runs callback function.
     */
    ~Finally()
    {
      myCallback();
    }

    /**
     * Copy assignment operator.
     */
    Finally& operator=(const Finally&) = default;

    /**
     * Move assignment operator.
     */
    Finally& operator=(Finally&&) = default;

    protected:

    std::function<void(void)> myCallback;
  };
}
