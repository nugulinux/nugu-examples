#include <QAbstractEventDispatcher>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThreadPool>

#include "adaptor/display_adaptor.hpp"
#include "adaptor/nugu_adaptor.hpp"
#include "nugu_worker.hpp"

static const char* GLIB_EVENT_DISPATCHER = "GlibEventDispatcher";

static void checkAndActivateNuguWorker(NuguAdaptor* nugu)
{
    if (auto event_dispatcher = QCoreApplication::eventDispatcher()) {
        std::string event_dispatcher_name = event_dispatcher->metaObject()->className();

        if (event_dispatcher_name.find(GLIB_EVENT_DISPATCHER) == std::string::npos) {
            NuguWorker* nugu_worker = new NuguWorker();
            QThreadPool::globalInstance()->start(nugu_worker);
            QObject::connect(nugu, &NuguAdaptor::finish, nugu_worker, &NuguWorker::finish);
        }
    }
}

int main(int argc, char* argv[])
{
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();
    auto nugu = NuguAdaptor::getInstance();

    // activate Nugu Worker if event dispatcher is not glib
    checkAndActivateNuguWorker(nugu);

    QObject::connect(nugu, &NuguAdaptor::finish, &app, &QGuiApplication::quit);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // register DisplayAdaptor
    qmlRegisterType<DisplayAdaptor>("Template", 1, 0, "Template");

    // setup qwebchannel.js
    QFile file(":/scripts/qwebchannel.js");

    if (file.open(QIODevice::ReadOnly))
        context->setContextProperty("JsWebChannel", file.readAll());

    context->setContextProperty("_nugu", nugu);
    context->setContextProperty("_voicechrome", nugu->getVoiceChromeAdaptor());
    engine.load(url);
    nugu->setup();

    int ret = app.exec();
    NuguAdaptor::destroyInstance();

    return ret;
}
