#ifndef FACTORY_H
#define FACTORY_H

#include <memory>
#include <map>
#include <functional>
#include <QString>

namespace raytracer
{

/// @brief Factory mixin class
template<typename Interface>
class FactoryMixin
{
public:
    typedef std::shared_ptr<Interface> Ptr;
    typedef std::function<Ptr()> GeneratorFunc;

    virtual ~FactoryMixin() {}

    static Ptr newInstance(const QString& typeId)
    {
        return generator(typeId)();
    }

private:
    template <class Impl>
    friend class GeneratorRegistrator;

    /// @brief Returns generator for @a typeId.
    ///
    /// @note The typeId and the associated generator must have been registered.
    static GeneratorFunc generator(const QString& typeId)
    {
        typename Registry::const_iterator it
            = registry().find(typeId);
        Q_ASSERT(it != registry().end());
        return it->second;
    }

    /// @brief Registers typeId and associated generator.
    static void addGenerator(QString typeId, GeneratorFunc g)
    {
        Q_ASSERT(registry().find(typeId) == registry().end());
        registry()[typeId] = g;
    }


    typedef std::map<QString, GeneratorFunc> Registry;

    static Registry& registry()
    {
        static Registry registry;
        return registry;
    }
};

template <class Implementation>
class GeneratorRegistrator
{
public:
    explicit GeneratorRegistrator(const QString& typeId)
    {
        Implementation::addGenerator(typeId, Implementation::newInstance);
    }
};

#define DECL_GENERATOR(ClassName) \
    public: \
    static  Ptr newInstance() { return std::make_shared<ClassName>(); }

#define REGISTER_GENERATOR(ClassName) \
    static raytracer::GeneratorRegistrator<ClassName> \
        ClassName##Registrator(#ClassName);

} // raytracer

#endif // FACTORY_H
