#pragma once

/**
 * @file ActorUpdateCallback.h
 * @brief Defines an interface to be used after each Actor is updated.
 *
 * @author Michael Albers
 */

namespace QS
{
  class Actor;

  /**
   * Defines a callback function to be called every time an Actor is updated.
   */
  class ActorUpdateCallback
  {
    public:

    /**
     * Callback for after an Actor has been updated.
     *
     * @param theActor
     *          Actor which as been updated
     */
    virtual void actorUpdate(const Actor *theActor) noexcept = 0;
  };
}
