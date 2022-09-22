#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "adaptor/display_adaptor.hpp"
#include "adaptor/nugu_adaptor.hpp"

int main(int argc, char* argv[])
{
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();
    auto nugu = NuguAdaptor::getInstance();

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
