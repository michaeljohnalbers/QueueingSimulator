#pragma once

/**
 * @file SpatialHash.h
 * @brief Defines the spatial hashing class.
 *
 * @author Michael Albers
 */

#include <cstdint>
#include <set>
#include <vector>
#include "Eigen/Core"

namespace QS
{
  class Actor;

  /**
   * Implementation of the T-Collide spatial hashing algorithm described in
   * "T-Collide: A Temporal, Real-Time Collision Detection Technique for
   * Bounded Objects" by E. Hastings, J. Mesit and R. Guha.
   */
  class SpatialHash
  {
    public:

    /**
     * Default constructor.
     */
    SpatialHash() = delete;

    /**
     * Constructor. Units aren't required for the parameters, but they should
     * all be in the same units.
     *
     * @param theWorldWidth
     *          width of the world
     * @param theWorldLength
     *          length of the world
     * @param theAverageActorSize
     *          average diameter of all Actors in the world
     * @throw std::invalid_argument
     *          if theAverageActorSize is &le; 0.
     */
    SpatialHash(float theWorldWidth,
                float theWorldLength,
                float theAverageActorSize);

    /**
     * Copy constructor.
     */
    SpatialHash(const SpatialHash&) = default;

    /**
     * Move constructor.
     */
    SpatialHash(SpatialHash&&) = default;

    /**
     * Destructor.
     */
    ~SpatialHash() = default;

    /**
     * Returns all Actors in the cell(s) that provided point/radius hashes to.
     * The returned list contains no duplicate Actors, even if the Actor hashes
     * to multiple cells.
     *
     * @param thePosition
     *          position in the world
     * @param theRadius
     *          radius around the point
     * @return list of Actors
     */
    std::set<const Actor*> getActors(Eigen::Vector2f thePosition,
                                     float theRadius) noexcept;

    /**
     * Returns the size of each cell. Cells are square (even if the world
     * isn't).
     *
     * @return cell size
     */
    uint32_t getCellSize() const noexcept;

    /**
     * Returns the number of cells.
     *
     * @return number of cells
     */
    uint32_t getNumberCells() const noexcept;

    /**
     * Hashes the Actor into a cell. The Actor pointer is saved into that cell.
     *
     * @param theActor
     *          Actor to hash
     */
    void hashActor(const Actor *theActor) noexcept;

    /**
     * Copy assignment operator.
     */
    SpatialHash& operator=(const SpatialHash&) = default;

    /**
     * Move assignment operator.
     */
    SpatialHash& operator=(SpatialHash&&) = default;

    /**
     * Removes the Actor from the hash.
     *
     * @param theActor
     *          Actor to remove
     */
    void removeActor(const Actor *theActor) noexcept;

    protected:

    private:

    /**
     * Coordinates for a single cell.
     */
    class Cell
    {
      public:
      /** X coordinate. */
      uint32_t myX;
      /** Y coordinate. */
      uint32_t myY;

      /**
       * Equality operator
       */
      bool operator==(const Cell &theRHS)
      {
        return (myX == theRHS.myX && myY == theRHS.myY);
      }
    };

    /**
     * Ensures that the givel Cell values never go too large. This is needed
     * since when a point is on the boundary of a cell the next cell is
     * calculated. When that boundary is the world edge, it could give an
     * invalid cell.
     *
     * @param theCell
     *          cell coordinate
     */
    void constrain(Cell &theCell) const;

    /**
     * Returns all buckets the circle overlaps.
     *
     * @param thePoint
     *          circle center
     * @param theRadius
     *          circle radius
     * @return set of buckets
     */
    std::set<uint32_t> getBucketsForCircle(Eigen::Vector2f thePoint,
                                           float theRadius) const noexcept;

    /**
     * Calculates which cell the point falls in. This is a hash function.
     *
     * @param thePoint
     *          point to hash
     * @return cell identifier
     */
    Cell calculateCellForPoint(Eigen::Vector2f thePoint) const noexcept;

    /**
     * Calculates which bucket the point falls in. This is a hash function.
     *
     * @param thePoint
     *          point to hash
     * @return bucket number
     */
    uint32_t calculateBucketForPoint(Eigen::Vector2f thePoint) const noexcept;

    /**
     * Spatial buckets. Using set in order to make sure bucket contents are
     * unique.
     */
    std::vector<std::set<const Actor*>> myBuckets;

    /** Size of each cell. */
    uint32_t myCellSize;

    /** Number of columns in the grid. */
    uint32_t myNumberColumns;

    /** Number of rows in the grid. */
    uint32_t myNumberRows;
  };
}
