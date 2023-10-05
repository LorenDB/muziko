#include "Muziko.h"

Muziko::Muziko(QObject *parent)
    : QObject{parent}
{
    auto model = new SongsModel{this};
    model->setInstrument("Harmonica");

    m_songsModels.push_back(model);
}

Muziko *Muziko::instance()
{
    if (!s_instance)
        s_instance = new Muziko;
    return s_instance;
}

Muziko *Muziko::create(QQmlEngine *engine, QJSEngine *)
{
    // The instance has to exist before it is used. We cannot replace it.
    Q_ASSERT(s_instance);

    // The engine has to have the same thread affinity as the singleton.
    Q_ASSERT(engine->thread() == s_instance->thread());

    // There can only be one engine accessing the singleton.
    static QJSEngine *s_engine = engine;
    if (s_engine)
        Q_ASSERT(engine == s_engine);
    else
        s_engine = engine;

    // Explicitly specify C++ ownership so that the engine doesn't delete
    // the instance.
    QJSEngine::setObjectOwnership(s_instance, QJSEngine::CppOwnership);
    return s_instance;
}

SongsModel *Muziko::songs() const
{
    return m_songsModels.value(m_currentModel, nullptr);
}
