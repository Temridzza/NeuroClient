#include <QCoreApplication>
#include <gtest/gtest.h>

#include <QtCore/qcoreapplication.h>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    qputenv("QML_DISABLE_DEBUGGER", "1"); // <-- отключаем QML debugger
    // int argc = 0;
    // char* argv[] = {};
    QGuiApplication app(argc, argv);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}