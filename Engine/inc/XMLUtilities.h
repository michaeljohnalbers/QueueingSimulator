/**
 * @file XMLUtilities.cpp
 * @brief Helper class for working with Xercese.
 *
 * @author Michael Albers
 * Transcode code (ZStr class and conversion functions) taken from:
 * https://alfps.wordpress.com/2010/05/27/cppx-xerces-strings-simplified-by-ownership-part-i/
 */

#include <memory>

#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/dom/DOM.hpp"
#include "xercesc/sax2/Attributes.hpp"

XERCES_CPP_NAMESPACE_USE

namespace QS
{
  /**
   * Class used to contain either XMLCh array or char array.
   */
  template<class CharType>
  class ZStr      // Zero-terminated string.
  {
    public:
    ZStr(CharType const* s, void (*deleter)(CharType*)) :
      myArray(const_cast<CharType*>(s), deleter)
    {
      if (nullptr == deleter)
      {
        throw std::invalid_argument("Null deleter provided.");
      }
    }

    CharType const* get() const
    {
      return myArray.get();
    }

    private:
    std::shared_ptr<CharType> myArray;
  };

  /**
   * Helper class to make Xerces transcode less painful.
   */
  class XMLUtilities
  {
    public:
    using Utf16Char = ::XMLCh;

    /**
     * Deleter for Utf16Char array.
     *
     * @param p
     *          array to delete
     */
    static void dispose(Utf16Char* p)
    {
      XMLString::release(&p);
    }

    /**
     * Deleter for char array.
     *
     * @param p
     *          array to delete
     */
    static void dispose(char* p)
    {
      XMLString::release(&p);
    }

    /**
     * Trancodes from Utf16Char to char. Returns object to manage memory.
     *
     * @param s
     *         string to transcode.
     * @return Transcoded string
     */
    static ZStr<char> cStr(Utf16Char const* s)
    {
      return ZStr<char>(XMLString::transcode(s), &dispose);
    }

    /**
     * Trancodes from char to Utf16Char. Returns object to manage memory.
     *
     * @param s
     *         string to transcode.
     * @return Transcoded string
     */
    static ZStr<Utf16Char> uStr(char const* s)
    {
      return ZStr<Utf16Char>(XMLString::transcode(s), &dispose);
    }

    /**
     * Trancodes from std::string to Utf16Char (just saves an c_str call).
     * Returns object to manage memory.
     *
     * @param s
     *         string to transcode.
     * @return Transcoded string
     */
    static ZStr<Utf16Char> uStr(const std::string &theString)
    {
      return ZStr<Utf16Char>(XMLString::transcode(theString.c_str()), &dispose);
    }

    /**
     * Returns the value for the attribute with the given name.
     *
     * @param theAttrs
     *          attributes from element
     * @param theAttrName
     *          name of the attribute to retrieve
     * @throws std::invalid_argument
     *          if no attribute with the given name exists
     */
    static std::string getAttribute(const Attributes &theAttrs,
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

    /**
     * Returns location information as a string
     *
     * @param theException
     *          exception from which to get location information
     * @return location string
     */
    static std::string getLocation(const SAXParseException &theException)
    {
      std::string location;
      location += "line: " + std::to_string(theException.getLineNumber()) +
        ", column: " + std::to_string(theException.getColumnNumber());
      return location;
    }

    /**
     * Returns location information as a string
     *
     * @param theException
     *          exception from which to get location information
     * @return location string
     */
    static std::string getLocation(const XMLException &theException)
    {
      std::string location;
      location += "line: " + std::to_string(theException.getSrcLine());
      return location;
    }
  };
}
