/**
 * @file SpatialHash.cpp
 * @brief Definition of SpatialHash
 *
 * @author Michael Albers
 */

#include <cmath>
#include <string>
#include "Actor.h"
#include "SpatialHash.h"

QS::SpatialHash::SpatialHash(float theWorldWidth,
                             float theWorldLength,
                             float theAverageActorSize)
{
  if (theAverageActorSize <= 0.0)
  {
    throw std::invalid_argument("Invalid average Actor size, " +
                                std::to_string(theAverageActorSize) +
                                ", it must be greater than 0.");
  }

  myCellSize = std::ceil(theAverageActorSize * 5.0f);

  myNumberColumns = std::ceil(theWorldWidth / static_cast<float>(myCellSize));
  myNumberRows = std::ceil(theWorldLength / static_cast<float>(myCellSize));

  myBuckets.resize(myNumberColumns * myNumberRows);
}

QS::SpatialHash::Cell QS::SpatialHash::calculateCellForPoint(
  Eigen::Vector2f thePoint) const noexcept
{
  Cell cell{static_cast<uint32_t>(thePoint.x() / myCellSize),
            static_cast<uint32_t>(thePoint.y() / myCellSize)};
  constrain(cell);
  return cell;
}

uint32_t QS::SpatialHash::calculateBucketForPoint(Eigen::Vector2f thePoint)
  const noexcept
{
  auto cell = calculateCellForPoint(thePoint);
  return cell.myY * myNumberColumns + cell.myX;
}

void QS::SpatialHash::constrain(Cell &theCell) const
{
  if (theCell.myX >= myNumberRows)
  {
    theCell.myX = myNumberRows-1;
  }

  if (theCell.myY >= myNumberColumns)
  {
    theCell.myY = myNumberColumns-1;
  }
}

std::set<const QS::Actor*> QS::SpatialHash::getActors(
  Eigen::Vector2f thePosition,
  float theRadius) noexcept
{
  std::set<const Actor*> actors;
  auto buckets = getBucketsForCircle(thePosition, theRadius);
  for (auto bucket : buckets)
  {
    std::set<const Actor*> &bucketActors = myBuckets[bucket];
    for (auto bucketActor : bucketActors)
    {
      actors.insert(bucketActor);
    }
  }
  return actors;
}

std::set<uint32_t> QS::SpatialHash::getBucketsForCircle(
  Eigen::Vector2f thePoint,
  float theRadius) const noexcept
{
  std::set<uint32_t> buckets;

  // Using a bounding box for hashing as it more easily finds the overlap
  // buckets than hashing points around the circle. Correctly hashing a circle
  // is difficult. The trade-off is the bounding box will occasionally hash
  // the circle into a cell it doesn't actually overlap.

  Eigen::Vector2f min(thePoint.x() - theRadius, thePoint.y() - theRadius);
  Cell cellMin = calculateCellForPoint(min);

  Eigen::Vector2f max(thePoint.x() + theRadius, thePoint.y() + theRadius);
  Cell cellMax = calculateCellForPoint(max);

  if (cellMin == cellMax)
  {
    // First case, circle is only in one cell.
    buckets.insert(calculateBucketForPoint(min));
  }
  else if (cellMin.myX == cellMax.myX ||
           cellMin.myY == cellMax.myY)
  {
    // Case 2, circle overlaps two cells vertically or horizontally. Both
    // points can be hashed since the buckets set will ensure unique bucket
    // numbers.
    buckets.insert(calculateBucketForPoint({min.x(), min.y()}));
    buckets.insert(calculateBucketForPoint({max.x(), max.y()}));
  }
  else
  {
    // Case 3, circle overlaps four cells. It may not actually since the
    // bounding box defined by min/max covers more space than the circle,
    // but in the worst case you add an extra cell's worth of Actors. But
    // this will only happen if the circumference sits exactly on the corner
    // of the four cells. This will be exceedingly rare.
    // lower left
    buckets.insert(calculateBucketForPoint({min.x(), min.y()}));

    // lower right
    buckets.insert(calculateBucketForPoint({max.x(), min.y()}));

    // upper right
    buckets.insert(calculateBucketForPoint({max.x(), max.y()}));

    // upper left
    buckets.insert(calculateBucketForPoint({min.x(), max.y()}));
  }

  return buckets;
}

uint32_t QS::SpatialHash::getCellSize() const noexcept
{
  return myCellSize;
}

uint32_t QS::SpatialHash::getNumberCells() const noexcept
{
  return myBuckets.size();
}

void QS::SpatialHash::hashActor(
  const Actor *theActor) noexcept
{
  Eigen::Vector2f position = theActor->getPosition();
  float radius = theActor->getRadius();
  auto buckets = getBucketsForCircle(position, radius);

  for (auto bucket : buckets)
  {
    myBuckets[bucket].insert(theActor);
  }
}

void QS::SpatialHash::removeActor(const Actor *theActor) noexcept
{
  for (auto &bucket: myBuckets)
  {
    if (bucket.size() == 0)
    {
      continue;
    }

    bucket.erase(theActor);
  }
}
