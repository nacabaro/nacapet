#include "lines.h"
#include "memory/memory.h"

#include "SPIFFS.h"

const char lineHeader[5] = "NPET"; 
const uint8_t headerSize = 4;

struct Egg_t* lines_getAvailableLines(uint8_t* count) {
    File root = SPIFFS.open("/lines");
    File lineFile = root.openNextFile();

    uint8_t allocCount = 0;
    char header[5]; 

    while (lineFile) {
        printf("[LINES] Opening file %s\n", lineFile.name());

        uint8_t readBytes = lineFile.readBytes(header, headerSize);
        if (strncmp(header, lineHeader, headerSize) == 0 && readBytes == headerSize) {
            allocCount++;
        }

        lineFile = root.openNextFile();
    }

    struct Egg_t* availableLines = (struct Egg_t*) malloc(sizeof(struct Egg_t) * allocCount);

    root.close();

    printf("[LINES] Found %i lines.\n", allocCount);
    allocCount = 0;

    root = SPIFFS.open("/lines");
    lineFile = root.openNextFile("r");

    while (lineFile) {
        uint16_t bytesRead = lineFile.readBytes(header, headerSize);
        bytesRead += lineFile.read(&availableLines[allocCount].id, 1);
        bytesRead += lineFile.readBytes(availableLines[allocCount].name, 16);

        bytesRead += lineFile.read(&availableLines[allocCount].eggSprite.spriteWidth, 1);
        bytesRead += lineFile.read(&availableLines[allocCount].eggSprite.spriteHeight, 1);
        bytesRead += lineFile.read(&availableLines[allocCount].eggSprite.spriteNumber, 1); // Se coloca el cursor al principio de los datos de sprite

        availableLines[allocCount].eggSprite.spriteNumber = 1; // Inutil, pero es que necesito hacer la lectura

        availableLines[allocCount].eggSprite.spriteData = memory_allocate(
            availableLines[allocCount].eggSprite.spriteNumber,
            availableLines[allocCount].eggSprite.spriteWidth,
            availableLines[allocCount].eggSprite.spriteHeight
        );

        uint8_t highByte;
        uint8_t lowByte;

        for (int i = 0; i < availableLines[allocCount].eggSprite.spriteWidth * availableLines[allocCount].eggSprite.spriteHeight; i++) {
            bytesRead += lineFile.read(&highByte, 1);
            bytesRead += lineFile.read(&lowByte, 1);

            uint16_t pixel = (highByte << 8) | lowByte;

            availableLines[allocCount].eggSprite.spriteData[0][i] = pixel;
        }

        strcpy(availableLines[allocCount].fileName, lineFile.name());
        
        lineFile.close();
        
        allocCount++;
        lineFile = root.openNextFile();
    }

    root.close();

    *count = allocCount;
    return availableLines;
}

void lines_testLines() {
    uint8_t countedLines;
    struct Egg_t* availableLines = lines_getAvailableLines(&countedLines);

    char fullPath[8 + strlen(availableLines[0].fileName)];
    snprintf(fullPath, 20, "/lines/%s", availableLines[0].fileName);

    lines_freeEggList(availableLines, countedLines);
    
    printf("[DEBUG] fullPath=%s\n", fullPath);

    availableLines = (Egg_t*) malloc(sizeof(Egg_t));

    struct Line_t* singleLine = lines_getSingleLine(fullPath, availableLines);
        
    lines_freeEggList(availableLines, 1);

    struct LineCare_t* lineCareData = lines_getLineCareMistakes(availableLines->fileName);

    for (int i = 0; i < lineCareData->numCareMistakesData; i++) {
        printf(
            "[DEBUG] Chara %d changes into chara %d\n", 
            lineCareData->careMistakeData[i].currentChara, 
            lineCareData->careMistakeData[i].nextChara
        );
    }
}