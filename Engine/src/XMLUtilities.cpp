/**
 * @file XMLUtilities.cpp
 * @brief Definition of XMLUtilities class
 *
 * @author Michael Albers
 */

#include "XMLUtilities.h"

void QS::XMLUtilities::dispose(Utf16Char* p)
{
  XMLString::release(&p);
}

void QS::XMLUtilities::dispose(char* p)
{
  XMLString::release(&p);
}

QS::ZStr<char> QS::XMLUtilities::cStr(Utf16Char const* s)
{
  return ZStr<char>(XMLString::transcode(s), &dispose);
}

QS::ZStr<QS::XMLUtilities::Utf16Char> QS::XMLUtilities::uStr(char const* s)
{
  return ZStr<Utf16Char>(XMLString::transcode(s), &dispose);
}

QS::ZStr<QS::XMLUtilities::Utf16Char>
QS::XMLUtilities::uStr(const std::string &theString)
{
  return ZStr<Utf16Char>(XMLString::transcode(theString.c_str()), &dispose);
}

std::string QS::XMLUtilities::getAttribute(const Attributes &theAttrs,
                                           const std::string &theAttrName)
{
  auto attrName = XMLUtilities::uStr(theAttrName);
  auto attrValueRaw = theAttrs.getValue(attrName.get());
  if (attrValueRaw == nullptr)
  {
    throw std::invalid_argument{"No attribute with name '" + theAttrName +
        "' found."};
  }

  auto attrValue = XMLUtilities::cStr(attrValueRaw);
  return attrValue.get();
}

std::string QS::XMLUtilities::getLocation(const SAXParseException &theException)
{
  std::string location;
  location += "line: " + std::to_string(theException.getLineNumber()) +
    ", column: " + std::to_string(theException.getColumnNumber());
  return location;
}

std::string QS::XMLUtilities::getLocation(const XMLException &theException)
{
  std::string location;
  location += "line: " + std::to_string(theException.getSrcLine());
  return location;
}
