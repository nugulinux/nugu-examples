#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "nugu_auth_helper.hh"

int main(int argc, char** argv)
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    app.setOrganizationName("empire");
    app.setOrganizationDomain("death_squadron");

    qmlRegisterType<NuguAuthHelper>("NuguAuth", 1, 0, "NuguAuth");
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
