#ifndef SERIAL_H
#define SERIAL_H

#include <QVariant>
#include <memory>
#include <functional>

#include "common.h"
#include "factory.h"

/// \file
/// \brief Declaration of serialization utilities.

namespace raytracer {

// Conversion to common simple types from variant

template< class T > inline T fromVariant(const QVariant& v);

template<> inline float fromVariant(const QVariant& v)
{
    Q_ASSERT(v.canConvert<float>());
    return v.value<float>();
}

template<> inline quint64 fromVariant(const QVariant& v)
{
    Q_ASSERT(v.canConvert<quint64>());
    return v.value<quint64>();
}

template< int n >
inline fsmx::MX< fsmx::Data<n,1,float> > vectorFromVariant(const QVariant& v)
{
    Q_ASSERT(v.type() == QVariant::List);
    QVariantList lst = v.toList();
    Q_ASSERT(lst.size() == n);
    fsmx::MX< fsmx::Data<n,1,float> > result;
    for (int i=0; i<n; ++i)
        result[i] = fromVariant<float>(lst[i]);
    return result;
}

template<>
inline v3f fromVariant(const QVariant& v)
{
    return vectorFromVariant<3>(v);
}

template<>
inline v2f fromVariant(const QVariant& v)
{
    return vectorFromVariant<2>(v);
}

template<> inline int fromVariant(const QVariant& v)
{
    Q_ASSERT(v.canConvert<int>());
    return v.toInt();
}

template<> inline QString fromVariant(const QVariant& v)
{
    Q_ASSERT(v.canConvert<QString>());
    return v.toString();
}



/// \brief Interface of a readable object.
///
/// Also contains a number of utility static functions to read from QVariant.
struct Readable
{
    virtual ~Readable() {}

    /// \brief Reads data from QVariant.
    virtual void read(const QVariant& v) = 0;



    // Utility static functions to read from QVariant

    static QVariantMap safeVariantMap(const QVariant& v)
    {
        Q_ASSERT(v.canConvert<QVariantMap>());
        return v.toMap();
    }

    static void readProperty(
            const QVariantMap& m,
            const QString& name,
            std::function<void(const QVariant&)> func)
    {
        QVariantMap::const_iterator it = m.find(name);
        Q_ASSERT (it != m.end());
        func(it.value());
    }

    static void readProperty(
            const QVariant& v,
            const QString& name,
            std::function<void(const QVariant&)> func)
    {
        Q_ASSERT(v.canConvert<QVariantMap>());
        readOptionalProperty(v.toMap(), name, func);
    }

    static QVariant readProperty(
            const QVariantMap& m,
            const QString& name)
    {
        QVariantMap::const_iterator it = m.find(name);
        Q_ASSERT (it != m.end());
        return it.value();
    }

    template< class T >
    static void readProperty(
            T& dst,
            const QVariantMap& m,
            const QString& name)
    {
        dst = fromVariant<T>(readProperty(m, name));
    }

    static QVariant readProperty(
            const QVariant& v,
            const QString& name)
    {
        return readProperty(safeVariantMap(v), name);
    }

    template< class Interface >
    static void readTypedProperty(
            std::shared_ptr<Interface>& dst,
            const QVariantMap& m,
            const QString& name)
    {
        readProperty(m, name, [&dst](const QVariant& v) {
            dst = readTypedInstance<Interface>(v);
        });
    }

    template< class T >
    static void readProperty(
            T& dst,
            const QVariant& v,
            const QString& name)
    {
        dst = fromVariant<T>(readProperty(v, name));
    }

    static bool readOptionalProperty(
            const QVariantMap& m,
            const QString& name,
            std::function<void(const QVariant&)> func)
    {
        QVariantMap::const_iterator it = m.find(name);
        if (it == m.end())
            return false;
        func(it.value());
        return true;
    }

    static bool readOptionalProperty(
            const QVariant& v,
            const QString& name,
            std::function<void(const QVariant&)> func)
    {
        return v.canConvert<QVariantMap>() ?
                    readOptionalProperty(v.toMap(), name, func) :
                    false;
    }

    template< class T >
    static bool readOptionalProperty(
            T& dst,
            const QVariantMap& m,
            const QString& name)
    {
        return readOptionalProperty(m, name, [&dst](const QVariant& v) {
            dst = fromVariant<T>(v);
        });
    }

    template< class T >
    static bool readOptionalProperty(
            T& dst,
            const QVariant& v,
            const QString& name)
    {
        return readOptionalProperty(v, name, [&dst](const QVariant& v) {
            dst = fromVariant<T>(v);
        });
    }

    template< class Interface >
    static std::shared_ptr<Interface> readTypedInstance(const QString& typeId, const QVariant& v)
    {
        auto result = Interface::newInstance(typeId);
        result->read(v);
        return result;
    }

    template< class Interface >
    static std::shared_ptr<Interface> readTypedInstanceFromVariant(const QVariant& v)
    {
        Q_ASSERT (v.type() == QVariant::List);
        QVariantList lst = v.toList();
        Q_ASSERT(lst.size() == 2);
        return readTypedInstance<Interface>(lst[0].toString(), lst[1]);
    }

    template< class Interface >
    static std::shared_ptr<Interface> readTypedInstance(const QVariant& v)
    {
        return readTypedInstanceFromVariant<Interface>(v);
    }

    template< class Interface >
    static void readTypedInstances(
            std::vector< std::shared_ptr<Interface> >& dst,
            const QVariant& v)
    {
        Q_ASSERT(v.type() == QVariant::List);
        QVariantList lst = v.toList();
        dst.resize(lst.size());
        std::transform(
                    lst.begin(), lst.end(), dst.begin(),
                    readTypedInstanceFromVariant<Interface>);
    }

    template< class Interface >
    static bool readOptionalTypedProperty(
            std::shared_ptr<Interface>& dst,
            const QVariantMap& m,
            const QString& name)
    {
        return readOptionalProperty(m, name, [&dst](const QVariant& v) {
            dst = readTypedInstance<Interface>(v);
        });
    }

    template< class Interface >
    static bool readOptionalTypedProperty(
            std::shared_ptr<Interface>& dst,
            const QVariant& v,
            const QString& name)
    {
        return readOptionalTypedProperty(dst, safeVariantMap(v), name);
    }
};


/// \brief File reader interface.
struct FileReader :
    public FactoryMixin<FileReader>
{
    virtual ~FileReader() {}

    /// \brief Reads contents of the specified file into a QVariant.
    virtual QVariant read(const QString& fileName) = 0;
};

} // end namespace raytracer

#endif // SERIAL_H
