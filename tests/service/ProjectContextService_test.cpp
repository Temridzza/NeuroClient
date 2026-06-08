#include <gtest/gtest.h>
#include <iostream>

#include "ProjectNode.h"
#include "ProjectContextService.h"

// TEST(ProjectContextService, buildTree)
// {
//     ProjectContextService service;

//     auto result = service.buildTree("/home/temridzza/Documents");

//     EXPECT_FALSE(result.empty());

//     for (const auto& node : result) {
//         std::cout
//             << node.name.toStdString()
//             << " | "
//             << node.path.toStdString()
//             << "\n";
//     }
// }

// TEST(ProjectContextService, collectFiles)
// {
//     ProjectContextService service;

//     service.buildTree("/home/temridzza/Documents");

//     service.collectFiles({
//         "script.py"
//     });
//     std::cout << "sd";
//     SUCCEED();
// }

// TEST(ProjectContextService, collectMultipleFiles)
// {
//     ProjectContextService service;

//     service.buildTree("/home/temridzza/Documents");

//     service.collectFiles({
//         "test.txt",
//         "notes.md",
//         "project/main.cpp"
//     });

//     SUCCEED();
// }

// TEST(ProjectContextService, collectEmptyFilesList)
// {
//     ProjectContextService service;

//     service.buildTree("/home/temridzza/Documents");

//     service.collectFiles({});

//     SUCCEED();
// }

// TEST(ProjectContextService, collectInvalidFile)
// {
//     ProjectContextService service;

//     service.buildTree("/home/temridzza/Documents");

//     service.collectFiles({
//         "does_not_exist.txt"
//     });

//     SUCCEED();
// }