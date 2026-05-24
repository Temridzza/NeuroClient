#include <iostream>
#include <gtest/gtest.h>

#include "ChatGPTConfig.h"
#include "../src/ChatGPTConfig.cpp"
#include <QString>

static void printResult(const Result &result) {
    std::cout << "success: " << result.success << "\n";

    if (!result.success) {
        std::cout << "error: " << result.error.toStdString() << "\n";
        return;
    }

    const QVariant &d = result.data;

    switch (d.type()) {
        case QVariant::String:
            std::cout << "data: " << d.toString().toStdString() << "\n";
            break;

        case QVariant::Bool:
            std::cout << "data: " << d.toBool() << "\n";
            break;

        case QVariant::StringList: {
            auto list = d.toStringList();
            std::cout << "stringlist: ";
            for (auto &s : list) std::cout << s.toStdString() << " ";
            std::cout << "\n";
            break;
        }

        case QVariant::List: {
            QVariantList list = d.toList();
            std::cout << "variant list: ";
            for (const auto &v : list) {
                std::cout << v.toString().toStdString() << " ";
            }
            std::cout << "\n";
            break;
        }

        default:
            if (d.canConvert<QJsonArray>()) {
                QJsonArray arr = d.toJsonArray();
                std::cout << "json array: "
                          << QString(QJsonDocument(arr).toJson()).toStdString() << "\n";
            }
            else {
                std::cout << "test data: <unknown type>\n";
            }
    }
}

#define CFG ChatGPTConfig::instance()


TEST(ConfigTest, getApiKey) {
    printResult(CFG.getApiKey());
}

TEST(ConfigTest, setApiKey) {
    QString key = "test-apiKey";
    printResult(CFG.setApiKey(key));
}

TEST(ConfigTest, setApiKeyPath) {
    printResult(CFG.setApiKeyPath("/home/temridzza/.config/chatgpt/api.key"));
}

TEST(ConfigTest, getCurrentModel) {
    printResult(CFG.getCurrentModel());
}

TEST(ConfigTest, setCurrentModel) {
    QString str = "meta-llama/llama-3.3-70b-instruct:free";
    printResult(CFG.setCurrentModel(str));
}

TEST(ConfigTest, getSelectedModels) {
    printResult(CFG.getSelectedModels());
}

TEST(ConfigTest, addSelectedModels) {
    QString str = "meta-llama/llama-3.3-70b-instruct:free";
    CFG.addSelectedModels(str);
    printResult(CFG.getSelectedModels());
}

TEST(ConfigTest, delSelectedModels) {
    QString str = "meta-llama/llama-3.3-70b-instruct:free21";
    CFG.delSelectedModels(str);
    printResult(CFG.getSelectedModels());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
