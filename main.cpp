#include <iostream>
#include <fstream>
#include <string>
#include <cstdio> // Для удаления файлов
#include <hpdf.h>

// Функция создания файла
void createFile() {
    std::string filename;
    std::cout << "Введите имя нового текстового файла (с расширением .txt): ";
    std::cin >> filename;

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Ошибка при создании файла.\n";
        return;
    }
    std::cout << "Введите текст (введите \"exit\" для завершения):\n";
    std::string line;
    std::cin.ignore(); // Игнорируем символ новой строки после ввода имени файла
    while (std::getline(std::cin, line)) {
        if (line == "exit") break;
        file << line << '\n';
    }
    file.close();
    std::cout << "Файл " << filename << " создан.\n";
}

// Функция редактирования файла
void editFile() {
    std::string filename;
    std::cout << "Введите имя существующего текстового файла для редактирования: ";
    std::cin >> filename;

    std::ifstream fileIn(filename);
    if (!fileIn) {
        std::cerr << "Ошибка: файл не найден.\n";
        return;
    }

    std::cout << "Содержимое файла:\n";
    std::string line;
    while (std::getline(fileIn, line)) {
        std::cout << line << '\n';
    }
    fileIn.close();

    std::ofstream fileOut(filename, std::ios::app); // Режим добавления текста
    if (!fileOut) {
        std::cerr << "Ошибка при открытии файла для записи.\n";
        return;
    }

    std::cout << "Введите текст для добавления (введите \"exit\" для завершения):\n";
    std::cin.ignore();
    while (std::getline(std::cin, line)) {
        if (line == "exit") break;
        fileOut << line << '\n';
    }
    fileOut.close();
    std::cout << "Изменения сохранены.\n";
}

// Функция удаления файла
void deleteFile() {
    std::string filename;
    std::cout << "Введите имя файла для удаления: ";
    std::cin >> filename;

    if (std::remove(filename.c_str()) == 0) {
        std::cout << "Файл " << filename << " успешно удален.\n";
    } else {
        std::cerr << "Ошибка: файл " << filename << " не найден или не может быть удален.\n";
    }
}

// Функция конвертации TXT в PDF
void convertTxtToPdf() {
    std::string txtFilename, pdfFilename;
    std::cout << "Введите имя текстового файла (с расширением .txt): ";
    std::cin >> txtFilename;
    std::cout << "Введите имя выходного PDF-файла (с расширением .pdf): ";
    std::cin >> pdfFilename;

    std::ifstream txtFile(txtFilename);
    if (!txtFile) {
        std::cerr << "Ошибка открытия текстового файла.\n";
        return;
    }

    HPDF_Doc pdf = HPDF_New(nullptr, nullptr);
    if (!pdf) {
        std::cerr << "Ошибка создания PDF-файла.\n";
        return;
    }

    HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    float fontSize = 12;
    float margin = 50;
    float yPosition = 800;

    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);
    HPDF_Page_SetFontAndSize(page, font, fontSize);

    std::string line;
    while (std::getline(txtFile, line)) {
        if (yPosition < margin) { // Если текст достигает нижнего края страницы
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetFontAndSize(page, font, fontSize);
            yPosition = 800; // Сброс позиции для новой страницы
        }

        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, margin, yPosition, line.c_str());
        HPDF_Page_EndText(page);

        yPosition -= fontSize + 2; // Расстояние между строками
    }

    if (HPDF_SaveToFile(pdf, pdfFilename.c_str()) != HPDF_OK) {
        std::cerr << "Ошибка сохранения PDF-файла.\n";
        HPDF_Free(pdf);
        return;
    }

    HPDF_Free(pdf);
    txtFile.close();
    std::cout << "Файл " << pdfFilename << " успешно создан.\n";
}

// Главное меню
int main() {
    int choice;
    do {
        std::cout << "\nТекстовый редактор\n";
        std::cout << "1. Создать файл\n";
        std::cout << "2. Редактировать файл\n";
        std::cout << "3. Удалить файл\n";
        std::cout << "4. Конвертировать txt в pdf\n";
        std::cout << "5. Выход\n";
        std::cout << "Выберите опцию: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                createFile();
                break;
            case 2:
                editFile();
                break;
            case 3:
                deleteFile();
                break;
            case 4:
                convertTxtToPdf();
                break;
            case 5:
                std::cout << "Выход из программы.\n";
                break;
            default:
                std::cout << "Неверный выбор, попробуйте снова.\n";
        }
    } while (choice != 5);

    return 0;
}