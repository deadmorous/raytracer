/// \file
/// \brief Implementation of the JsonFileReader class.

#include "json_file_reader.h"
#include "cxx_exception.h"
#include "json_parser.h"
#include <QFile>

namespace raytracer {

REGISTER_GENERATOR(JsonFileReader)

QVariant JsonFileReader::read(const QString& fileName)
{
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly))
        throw cxx::exception(QObject::tr("Unable to open input file %1").arg(fileName).toStdString());
    QString contents = QString::fromUtf8(f.readAll());
    return JsonParser(contents)();
}

} // end namespace raytracer
