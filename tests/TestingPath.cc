#include <gtest/gtest.h>
#include <ThreadSafeDeque.h>
#include <Utilities.h>
#include <filesystem>

TEST(PathTests, TestingTheDirectoriesExistance)
{
    char testPath[] {"/rosi/movies"};
    std::filesystem::remove_all("/tmp/rosi/movies");
    createPath(testPath, "/tmp/");
    EXPECT_EQ(std::filesystem::exists("/tmp/rosi/movies"), true);
}

TEST(PathTests, TestingTheDirectoriesExistance_2)
{
    char testPath[] {"rosi"};
    char testPath2[] {"rosi/documents"};
    std::filesystem::remove_all("/tmp/rosi/documents");
    createPath(testPath, "/tmp/");
    createPath(testPath2, "/tmp/");
    EXPECT_EQ(std::filesystem::exists("/tmp/rosi/documents"), true);
}

TEST(PathTests, TestingTheDirectoriesExistance_3)
{
    char testPath[] {"/rosi/"};
    char testPath2[] {"/rosi/documents"};
    std::filesystem::remove_all("/tmp/rosi/documents");
    createPath(testPath, "/tmp/");
    createPath(testPath2, "/tmp/");
    EXPECT_EQ(std::filesystem::exists("/tmp/rosi/documents"), true);
}