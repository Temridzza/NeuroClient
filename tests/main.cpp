#include <QCoreApplication>
#include <gtest/gtest.h>

#include <QtCore/qcoreapplication.h>

int main(int argc, char *argv[])
{
    qputenv("QML_DISABLE_DEBUGGER", "1"); // <-- отключаем QML debugger
    QCoreApplication app(argc, argv);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}