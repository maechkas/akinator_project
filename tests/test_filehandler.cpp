#include <gtest/gtest.h>
#include "FileHandler.h"

TEST(FileHandlerTest, WriteReadFile) {//смотрим, что запись и прочтение файла работают корректно
    FileHandler handler("./test_dir", "default.json");

    // Записываем
    EXPECT_TRUE(handler.writeFile("test.txt", "hello world"));

    // Читаем
    auto content = handler.readFile("test.txt");
    ASSERT_TRUE(content.has_value());
    EXPECT_EQ(*content, "hello world");
}

TEST(FileHandlerTest, FileExists) { //Проверяет файл, если нет то фолс, потом создает и тут фвйл есть тру
    FileHandler handler("./test_dir2", "save.json");

    // Файла нет анлаки
    EXPECT_FALSE(handler.fileExists("missing.txt"));

    // Создаём файл
    handler.writeFile("exists.txt", "data");
    EXPECT_TRUE(handler.fileExists("exists.txt"));
}

TEST(FileHandlerTest, ListFiles) { // проверяет что listSaveFiles функция верно возвращает вск именя файлов из папки
    FileHandler handler("./test_dir3", "default.txt");

    handler.writeFile("file1.txt", "1");
    handler.writeFile("file2.json", "2");

    auto files = handler.listSaveFiles();
    EXPECT_EQ(files.size(), 2);
    EXPECT_NE(std::find(files.begin(), files.end(), "file1.txt"), files.end());
}
