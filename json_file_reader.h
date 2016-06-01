#ifndef JSONFILEREADER_H
#define JSONFILEREADER_H

#include "serial.h"

/// \file
/// \brief Declaration of the JsonFileReader class.

namespace raytracer {

/// \brief Reads JSON-like-formatted file into a QVariant.
///
/// E.g., the following is valid:
/// \code
/// {
///     a: 1,
///     b: [1, 2, 3]
/// }
class JsonFileReader : public FileReader
{
    DECL_GENERATOR(JsonFileReader)
public:
    QVariant read(const QString& fileName);
};

} // end namespace raytracer

#endif // JSONFILEREADER_H
