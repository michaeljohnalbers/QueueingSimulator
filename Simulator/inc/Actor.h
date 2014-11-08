#ifndef _Actor_h_
#define _Actor_h_

/**
 * @file Actor.h
 * @brief Actor class
 *
 * Models a single actor in the simulation.
 * 
 * @author Michael Albers
 */

#include <cstdint>
#include <ctime>
#include <tuple>
#include <vector>

/**
 * Class that models a single actor in the simulation. Each Actor has a rank
 * and current position within the world. The Actor's rank indicates its queue
 * postion. Rank must be unique amonst Actors. The actor with rank 1 is first
 * in line, rank 2 is second in line and so on.
 */
class Actor
{
  // ************************************************************
  // Public
  // ************************************************************
  public:

  /** Position. X & Y coordinate. */
  typedef std::tuple<int32_t, int32_t> Position;

  /** Constant used to access the row member of Position variables. */
  static const int32_t ROW = 0;

  /** Constant used to access the column member of Position variables. */
  static const int32_t COL = 1;

  /**
   * Default constructor
   */
  Actor() = delete;

  /**
   * Copy constructor.
   * @param theActor Object to copy.
   */
  Actor(const Actor &theActor);

  /**
   * Constructor.
   * @param theRank Rank of the actor.
   */
  Actor(int32_t theRank);

  /**
   * Constructor.
   * @param theRank Rank of the actor.
   * @param theStartTime Start time.
   */
  Actor(int32_t theRank, std::time_t &theStartTime);

  /**
   * Destructor.
   */
  ~Actor();

  /**
   * Assignment operator.
   * @param Object to copy
   * @return This object.
   */
  Actor& operator=(const Actor&) = delete;

  /**
   * Returns the current position of the Actor.
   * @return Current position.
   */
  Position getCurrentPosition() const;

  /**
   * Returns the Actor's end time.
   * @return Actor's end time.
   */
  std::time_t getEndTime() const;

  /**
   * Returns the list of all positions the actor has been.
   * @return List of all positions the actor has been.
   */
  const std::vector<Position>& getPreviousPositions() const;

  /**
   * Returns the Actor's rank.
   * @return Actor's rank.
   */
  int32_t getRank() const;

  /**
   * Updates the current position of the Actor while saving the current
   * position.
   * @param theNewPosition New position of the Actor.
   */
  void setCurrentPosition(const Position &theNewPosition);

  /**
   * Sets the end time for the Actor to the current time.
   */
  void setEndTime();

  // ************************************************************
  // Protected
  // ************************************************************
  protected:

  // ************************************************************
  // Protected
  // ************************************************************
  private:

  /** Current position of the Actor in the world.*/
  Position myCurrentPosition;

  static const Position InvalidPosition;

  /** List of all previous positions of the Actor. First entry is first
  position, etc.*/
  std::vector<Position> myPreviousPositions;

  /** Actor's rank. Lower number indicates higher rank.*/
  std::int32_t myRank;

  /** Time the Actor exited the world. */
  std::time_t myEndTime;
};

#endif
